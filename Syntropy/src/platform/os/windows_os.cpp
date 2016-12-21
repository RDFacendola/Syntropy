#include "platform/os/windows_os.h"

#ifdef _WIN64

#pragma comment(lib, "DbgHelp.lib")

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#pragma warning(pop)

#include <string>
#include <mutex>
#include <thread>

#include "syntropy.h"
#include "platform/system.h"

namespace syntropy
{
    namespace platform
    {

        //////////////// WINDOWS DEBUGGER :: IMPLEMENTATION ////////////////

        /// \brief Implementation details.
        /// The pimpl paradigm helps avoiding polluting everything with Windows macros... eww
        struct WindowsDebugger::Implementation
        {
            /// \brief Maximum symbol length.
            static const size_t kMaxSymbolLength = 1024;

            /// \brief Contains a properly-sized SYMBOL_INFO structure that accounts for the actual maximum symbol length.
            union SymbolInfo
            {
                char buffer[sizeof(SYMBOL_INFO) + kMaxSymbolLength * sizeof(TCHAR)];        ///< \brief Raw buffer.

                SYMBOL_INFO symbol;                                                         ///< \brief Actual symbol info.
            };

            /// \brief Default constructor.
            Implementation();

            /// \brief Default destructor.
            ~Implementation();

            /// \brief Get the current stacktrace.
            /// \param caller Code that issued the call to this method.
            diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller, CONTEXT context) const;

            /// \brief Get the stackframe from context.
            /// \param context Contains the current context.
            /// \param stackframe Contains the current stackframe.
            void GetStackFrame(const CONTEXT& context, STACKFRAME64& stackframe) const;

            /// \brief Get a StackTraceElement from a stack frame.
            /// \param stackframe Stackframe to convert.
            /// \return Returns the StackTraceElement associated to the provided stackframe.
            diagnostics::StackTraceElement GetStackTraceElement(const STACKFRAME64& stackframe) const;
            
            mutable std::mutex mutex_;              ///< \brief Used for synchronization. Symbol loading and stack walking are not thread safe!

            HANDLE process_;                        ///< \brief Current process handle.

            bool has_symbols_;                      ///< \brief Whether the symbols for this process were loaded correctly.
        };

        WindowsDebugger::Implementation::Implementation()
            : process_(GetCurrentProcess())
        {
            has_symbols_ = SymInitialize(process_, nullptr, true) != 0;

            if (has_symbols_)
            {
                SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
            }
        }

        WindowsDebugger::Implementation::~Implementation()
        {
            if (has_symbols_)
            {
                SymCleanup(process_);
            }
        }

        diagnostics::StackTrace WindowsDebugger::Implementation::GetStackTrace(diagnostics::StackTraceElement caller, CONTEXT context) const
        {
            std::unique_lock<std::mutex> lock(mutex_);                  // Stackwalking is not thread-safe

            diagnostics::StackTrace stacktrace;

            STACKFRAME64 stackframe;

            stacktrace.elements_.reserve(64);                           // An educated guess of the stack trace depth

            GetStackFrame(context, stackframe);

            // Stack walking
            bool keep_walking;
            int frames_to_discard = 1;

            diagnostics::StackTraceElement current_element;

            do
            {
                keep_walking = StackWalk64(IMAGE_FILE_MACHINE_AMD64,
                                           process_,
                                           GetCurrentThread(),
                                           &stackframe,
                                           &context,
                                           nullptr,
                                           SymFunctionTableAccess64,
                                           SymGetModuleBase64,
                                           nullptr) != 0;

                if (frames_to_discard == 0)
                {
                    stacktrace.elements_.emplace_back(std::move(GetStackTraceElement(stackframe)));
                }
                else
                {
                    --frames_to_discard;
                }
            } while (keep_walking);

            // Preserve the caller symbol in case PDB are not available.
            stacktrace.elements_.front() = std::move(caller);

            return stacktrace;
        }

        void WindowsDebugger::Implementation::GetStackFrame(const CONTEXT& context, STACKFRAME64& stackframe) const
        {
            memset(&stackframe, 0, sizeof(stackframe));

            // https://msdn.microsoft.com/it-it/library/windows/desktop/ms680646(v=vs.85).aspx
            stackframe.AddrPC.Offset = context.Rip;
            stackframe.AddrPC.Mode = AddrModeFlat;
            stackframe.AddrStack.Offset = context.Rsp;
            stackframe.AddrStack.Mode = AddrModeFlat;
            stackframe.AddrFrame.Offset = context.Rbp;
            stackframe.AddrFrame.Mode = AddrModeFlat;
        }

        diagnostics::StackTraceElement WindowsDebugger::Implementation::GetStackTraceElement(const STACKFRAME64& stackframe) const
        {
            syntropy::diagnostics::StackTraceElement element;

            IMAGEHLP_LINE64 line_info;
            SymbolInfo symbol_info;

            DWORD displacement;

            line_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

            symbol_info.symbol.SizeOfStruct = sizeof(SYMBOL_INFO);
            symbol_info.symbol.MaxNameLen = kMaxSymbolLength;

            // File and line
            if (SymGetLineFromAddr64(process_, stackframe.AddrPC.Offset, &displacement, &line_info))
            {
                element.file_ = line_info.FileName;
                element.line_ = line_info.LineNumber;
            }
            else
            {
                element.line_ = 0;
            }

            // Symbol name
            if (SymFromAddr(process_, stackframe.AddrPC.Offset, nullptr, &symbol_info.symbol))
            {
                element.function_ = symbol_info.symbol.Name;
            }

            return element;
        }

        //////////////// WINDOWS DEBUGGER ////////////////

        diagnostics::Debugger& WindowsDebugger::GetInstance()
        {
            static WindowsDebugger instance;
            return instance;
        }

        WindowsDebugger::WindowsDebugger()
            : implementation_(std::make_unique<Implementation>())
        {

        }
        
        bool WindowsDebugger::IsDebuggerAttached() const
        {
            return IsDebuggerPresent() != 0;
        }

        diagnostics::StackTrace WindowsDebugger::GetStackTrace(diagnostics::StackTraceElement caller) const
        {
            CONTEXT context;

            RtlCaptureContext(&context);

            return implementation_->GetStackTrace(std::move(caller), context);
        }

        //////////////// WINDOWS SYSTEM ////////////////

        System& WindowsSystem::GetInstance()
        {
            static WindowsSystem instance;
            return instance;
        }

        CPUInfo WindowsSystem::GetCPUInfo() const
        {
            CPUInfo cpu_info;

            LARGE_INTEGER frequency;
            SYSTEM_INFO system_info;

            GetSystemInfo(&system_info);
            QueryPerformanceFrequency(&frequency);

            cpu_info.cores_ = static_cast<uint32_t>(system_info.dwNumberOfProcessors);
            cpu_info.frequency_ = static_cast<uint64_t>(frequency.QuadPart) * 1000;

            switch (system_info.wProcessorArchitecture)
            {
                case PROCESSOR_ARCHITECTURE_AMD64:
                {
                    cpu_info.architecture_ = CPUArchitecture::kx64;
                    break;
                }
                case PROCESSOR_ARCHITECTURE_INTEL:
                {
                    cpu_info.architecture_ = CPUArchitecture::kx86;
                    break;
                }
                case PROCESSOR_ARCHITECTURE_ARM:
                {
                    cpu_info.architecture_ = CPUArchitecture::kARM;
                    break;
                }
                default:
                {
                    cpu_info.architecture_ = CPUArchitecture::kUnknown;
                    break;
                }
            }

            return cpu_info;
        }

        StorageInfo WindowsSystem::GetStorageInfo() const
        {
            StorageInfo storage_info;

            unsigned long drive_mask = GetLogicalDrives();
            wchar_t unit_path[] = L"A:\\";

            ULARGE_INTEGER size, available_space;
            DriveInfo drive_info;

            while (drive_mask != 0) 
            {
                if ((drive_mask & 1) && GetDriveType(unit_path) == DRIVE_FIXED) 
                {
                    GetDiskFreeSpaceEx(unit_path, NULL, &size, &available_space);

                    // Fill current drive info
                    drive_info.label_ = syntropy::to_string(unit_path);
                    drive_info.total_space_ = static_cast<uint64_t>(size.QuadPart);
                    drive_info.available_space_ = static_cast<uint64_t>(available_space.QuadPart);

                    storage_info.drives_.emplace_back(std::move(drive_info));

                }

                drive_mask >>= 1;
                ++unit_path[0];
            }

            return storage_info;
        }

        MemoryInfo WindowsSystem::GetMemoryInfo() const
        {
            MemoryInfo memory_info;

            SYSTEM_INFO system_info;
            MEMORYSTATUSEX memory_status;

            memory_status.dwLength = sizeof(MEMORYSTATUSEX);

            GetSystemInfo(&system_info);
            GlobalMemoryStatusEx(&memory_status);

            memory_info.page_size_ = system_info.dwPageSize;
            memory_info.allocation_granularity_ = system_info.dwAllocationGranularity;
            memory_info.lowest_memory_address_ = system_info.lpMinimumApplicationAddress;
            memory_info.highest_memory_address_ = system_info.lpMaximumApplicationAddress;

            memory_info.total_physical_memory_ = static_cast<uint64_t>(memory_status.ullTotalPhys);
            memory_info.total_virtual_memory_ = static_cast<uint64_t>(memory_status.ullTotalVirtual);
            memory_info.total_page_memory_ = static_cast<uint64_t>(memory_status.ullTotalPageFile);
            memory_info.available_physical_memory_ = static_cast<uint64_t>(memory_status.ullAvailPhys);
            memory_info.available_virtual_memory_ = static_cast<uint64_t>(memory_status.ullAvailVirtual);
            memory_info.available_page_memory_ = static_cast<uint64_t>(memory_status.ullAvailPageFile);

            return memory_info;
        }

        DisplayInfo WindowsSystem::GetDisplayInfo() const
        {
            DWORD display_index = 0;
            DISPLAY_DEVICE adapter_device;
            DISPLAY_DEVICE monitor_device;

            DEVMODE dev_mode;

            DisplayInfo display_info;
            MonitorInfo monitor_info;

            adapter_device.cb = sizeof(DISPLAY_DEVICE);
            monitor_device.cb = sizeof(DISPLAY_DEVICE);

            while (EnumDisplayDevices(nullptr, display_index, &adapter_device, EDD_GET_DEVICE_INTERFACE_NAME))
            {
                // Discard monitors that are not currently attached
                if ((adapter_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) != 0)
                {
                    monitor_info.adapter_name_ = syntropy::to_string(adapter_device.DeviceString);

                    if (EnumDisplaySettings(adapter_device.DeviceName, ENUM_CURRENT_SETTINGS, &dev_mode))
                    {
                        // Fill current monitor info
                        EnumDisplayDevices(adapter_device.DeviceName, 0, &monitor_device, 0);
                        
                        monitor_info.is_primary_ = (adapter_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0;
                        monitor_info.width_ = static_cast<uint32_t>(dev_mode.dmPelsWidth);
                        monitor_info.height_ = static_cast<uint32_t>(dev_mode.dmPelsHeight);
                        monitor_info.refresh_rate_ = static_cast<float>(dev_mode.dmDisplayFrequency);
                        monitor_info.monitor_name_ = syntropy::to_string(monitor_device.DeviceString);

                        display_info.monitors_.emplace_back(std::move(monitor_info));
                    }
                }

                ++display_index;
            }

            return display_info;
        }

        PlatformInfo WindowsSystem::GetPlatformInfo() const
        {
            PlatformInfo platform_info;

            platform_info.operating_system_ = OperatingSystem::kWindows;

            return platform_info;
        }

        //////////////// WINDOWS MEMORY ////////////////

        Memory& WindowsMemory::GetInstance()
        {
            static WindowsMemory instance;
            return instance;
        }

        WindowsMemory::WindowsMemory()
        {
            auto info = platform::GetSystem().GetMemoryInfo();

            page_size_ = info.page_size_;
            allocation_granularity_ = info.allocation_granularity_;
        }

        size_t WindowsMemory::GetPageSize() const
        {
            return page_size_;
        }

        size_t WindowsMemory::GetAllocationGranularity() const
        {
            return allocation_granularity_;
        }

        VirtualMemoryRange WindowsMemory::ReserveVirtualRange(size_t count)
        {
            // Round up to the nearest multiple of the allocation granularity
            count = ((count + allocation_granularity_ - 1) / allocation_granularity_) * allocation_granularity_;

            auto base_address = VirtualAlloc(0, count, MEM_RESERVE, PAGE_READWRITE);

            return VirtualMemoryRange(base_address, count);
        }

        bool WindowsMemory::ReleaseVirtualRange(const VirtualMemoryRange& range)
        {
            return VirtualFree(range.GetBaseAddress(), 0, MEM_RELEASE) != 0;
        }

        MemoryBlock WindowsMemory::AllocMemoryBlock(const VirtualMemoryRange& range, size_t offset, size_t size)
        {
            if (offset + size > range.GetCount())
            {
                return MemoryBlock(nullptr, 0);
            }

            // Round up to the nearest multiple of the page size
            size = ((size + page_size_ - 1) / page_size_) * page_size_;

            // Round down to the nearest multiple of the page size
            offset = (offset / page_size_) * page_size_;

            auto address = VirtualAlloc(range.GetAddress(offset), size, MEM_COMMIT, PAGE_READWRITE);

            return MemoryBlock(address, size);
        }

        MemoryBlock WindowsMemory::FreeMemoryBlock(const VirtualMemoryRange& range, size_t offset, size_t size)
        {
            if (offset + size > range.GetCount())
            {
                return MemoryBlock(nullptr, 0);
            }

            // Round down to the nearest multiple of the page size
            size = (size / page_size_) * page_size_;

            // Round up to the nearest multiple of the page size
            offset = ((offset + page_size_ - 1) / page_size_) * page_size_;

            if (VirtualFree(range.GetAddress(offset), size, MEM_DECOMMIT) != 0)
            {
                return MemoryBlock(range.GetAddress(offset), size);
            }
            else
            {
                return MemoryBlock(nullptr, 0);
            }
        }

        bool WindowsMemory::FreeMemoryBlock(const MemoryBlock& block)
        {
            // Assumes the memory block has not been tampered with
            return VirtualFree(block.GetBaseAddress(), block.GetCount(), MEM_DECOMMIT) != 0;
        }
    }
}

#endif
