#define SYNTROPY_OS_INCLUDE_GUARD

#include "platform/os/windows_os.h"

#undef SYNTROPY_OS_INCLUDE_GUARD

#ifdef _WIN64

#pragma comment(lib, "DbgHelp.lib")

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#undef max

#pragma warning(pop)

#include <string>
#include <mutex>
#include <thread>
#include <algorithm>
#include <memory>
#include <unordered_map>

#include "memory/bytes.h"
#include "diagnostics/assert.h"

#include "syntropy.h"

namespace syntropy::platform
{

    /************************************************************************/
    /* WINDOWS DEBUGGER                                                     */
    /************************************************************************/

    /// \brief Stateful details for the Windows debugger.
    class WindowsDebugger
    {
    public:

        /// \brief Get the singleton instance.
        static WindowsDebugger& GetInstance()
        {
            static WindowsDebugger instance;
            return instance;
        }

        /// \brief Get the current stacktrace.
        /// \param caller Code that issued the call to this method.
        diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller) const
        {
            std::unique_lock<std::mutex> lock(mutex_);                  // Stackwalking is not thread-safe

            CONTEXT context;
            STACKFRAME64 stackframe;

            diagnostics::StackTrace stacktrace;

            RtlCaptureContext(&context);

            stacktrace.elements_.reserve(64);                           // An educated guess of the stack trace depth

            GetStackFrame(context, stackframe);

            // Stack walking
            bool keep_walking;
            int frames_to_discard = 3;                                  // Discards this call, PlatformDebugger::GetStackTrace and diagnostics::Debugger::GetStackTrace: we don't really want those frames to show up in the callstack.

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

            // Preserve the caller symbols in case PDB were not available.
            stacktrace.elements_.front() = std::move(caller);

            return stacktrace;
        }

    private:

        /// \brief Maximum symbol length.
        static const size_t kMaxSymbolLength = 1024;

        /// \brief Contains a properly-sized SYMBOL_INFO structure that accounts for the actual maximum symbol length.
        union SymbolInfo
        {
            char buffer[sizeof(SYMBOL_INFO) + kMaxSymbolLength * sizeof(TCHAR)];        ///< \brief Raw buffer.

            SYMBOL_INFO symbol;                                                         ///< \brief Actual symbol info.
        };

        /// \brief Default constructor.
        WindowsDebugger()
            : process_(GetCurrentProcess())
        {
            has_symbols_ = (SymInitialize(process_, nullptr, true) != 0);

            if (has_symbols_)
            {
                SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
            }
        }

        /// \brief Default destructor.
        ~WindowsDebugger()
        {
            if (has_symbols_)
            {
                SymCleanup(process_);
            }
        }

        /// \brief Get the stackframe from context.
        /// \param context Contains the current context.
        /// \param stackframe Contains the current stackframe.
        void GetStackFrame(const CONTEXT& context, STACKFRAME64& stackframe) const
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

        /// \brief Get a StackTraceElement from a stack frame.
        /// \param stackframe Stackframe to convert.
        /// \return Returns the StackTraceElement associated to the provided stackframe.
        diagnostics::StackTraceElement GetStackTraceElement(const STACKFRAME64& stackframe) const
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
            
        mutable std::mutex mutex_;              ///< \brief Used for synchronization. Symbol loading and stack walking are not thread safe!

        HANDLE process_;                        ///< \brief Current process handle.

        bool has_symbols_;                      ///< \brief Whether the symbols for this process were loaded correctly.
    };

    /************************************************************************/
    /* WINDOWS MEMORY                                                       */
    /************************************************************************/

    /// \brief Stateful details for the Windows memory.
    class WindowsMemory
    {
    public:

        static WindowsMemory& GetInstance()
        {
            static WindowsMemory instance;
            return instance;
        }

        /// \brief Get the allocation granularity.
        /// This value determines the granularity at which virtual memory can be reserved and released.
        Bytes GetAllocationGranularity() const
        {
            return allocation_granularity_;
        }

        /// \brief Get the size of each virtual memory page.
        /// This value determines the granularity at which virtual memory can be committed and decommitted.
        Bytes GetPageSize() const
        {
            return page_size_;
        }

        /// \brief Get the alignment of each virtual memory page.
        Alignment GetPageAlignment() const
        {
            return page_alignment_;
        }

    private:

        WindowsMemory()
        {
            SYSTEM_INFO system_info;
            GetSystemInfo(&system_info);

            allocation_granularity_ = Bytes(system_info.dwAllocationGranularity);
            page_size_ = Bytes(system_info.dwPageSize);
            page_alignment_ = Alignment(page_size_);                        // Since each page can be committed at page size boundaries, the page size is also the alignment.
        }

        Bytes allocation_granularity_;      ///< \brief Memory allocation granularity, in bytes.

        Bytes page_size_;                   ///< \brief Memory page size, in bytes.

        Alignment page_alignment_;          ///< \brief Memory page alignment, in bytes.
    };

    /************************************************************************/
    /* PLATFORM DEBUGGER                                                    */
    /************************************************************************/
        
    bool PlatformDebugger::IsDebuggerAttached()
    {
        return IsDebuggerPresent() != 0;
    }

    diagnostics::StackTrace PlatformDebugger::GetStackTrace(diagnostics::StackTraceElement caller)
    {
        return WindowsDebugger::GetInstance().GetStackTrace(std::move(caller));
    }

    /************************************************************************/
    /* PLATFORM SYSTEM                                                      */
    /************************************************************************/

    CPUInfo PlatformSystem::GetCPUInfo()
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

    StorageInfo PlatformSystem::GetStorageInfo()
    {
        StorageInfo storage_info;

        auto drive_mask = GetLogicalDrives();
        char unit_path[] = "A:\\";

        ULARGE_INTEGER size, available_space;
        DriveInfo drive_info;

        while (drive_mask != 0) 
        {

            if ((drive_mask & 1) && GetDriveTypeA(unit_path) == DRIVE_FIXED)
            {
                GetDiskFreeSpaceExA(unit_path, NULL, &size, &available_space);

                // Fill current drive info
                drive_info.label_ = unit_path;
                drive_info.total_space_ = static_cast<uint64_t>(size.QuadPart);
                drive_info.available_space_ = static_cast<uint64_t>(available_space.QuadPart);

                storage_info.drives_.emplace_back(std::move(drive_info));

            }

            drive_mask >>= 1;
            ++unit_path[0];
        }

        return storage_info;
    }

    MemoryInfo PlatformSystem::GetMemoryInfo()
    {
        MemoryInfo memory_info;

        MEMORYSTATUSEX memory_status;

        memory_status.dwLength = sizeof(MEMORYSTATUSEX);

        GlobalMemoryStatusEx(&memory_status);

        memory_info.total_physical_memory_ = static_cast<uint64_t>(memory_status.ullTotalPhys);
        memory_info.total_virtual_memory_ = static_cast<uint64_t>(memory_status.ullTotalVirtual);
        memory_info.total_page_memory_ = static_cast<uint64_t>(memory_status.ullTotalPageFile);
        memory_info.available_physical_memory_ = static_cast<uint64_t>(memory_status.ullAvailPhys);
        memory_info.available_virtual_memory_ = static_cast<uint64_t>(memory_status.ullAvailVirtual);
        memory_info.available_page_memory_ = static_cast<uint64_t>(memory_status.ullAvailPageFile);

        return memory_info;
    }

    DisplayInfo PlatformSystem::GetDisplayInfo()
    {
        DWORD display_index = 0;
        DISPLAY_DEVICEA adapter_device;
        DISPLAY_DEVICEA monitor_device;

        DEVMODEA dev_mode;

        DisplayInfo display_info;
        MonitorInfo monitor_info;

        adapter_device.cb = sizeof(adapter_device);
        monitor_device.cb = sizeof(monitor_device);

        while (EnumDisplayDevicesA(nullptr, display_index, &adapter_device, EDD_GET_DEVICE_INTERFACE_NAME))
        {
            // Discard monitors that are not currently attached
            if ((adapter_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) != 0)
            {
                monitor_info.adapter_name_ = adapter_device.DeviceString;

                if (EnumDisplaySettingsA(adapter_device.DeviceName, ENUM_CURRENT_SETTINGS, &dev_mode))
                {
                    // Fill current monitor info
                    EnumDisplayDevicesA(adapter_device.DeviceName, 0, &monitor_device, 0);
                        
                    monitor_info.is_primary_ = (adapter_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0;
                    monitor_info.width_ = static_cast<uint32_t>(dev_mode.dmPelsWidth);
                    monitor_info.height_ = static_cast<uint32_t>(dev_mode.dmPelsHeight);
                    monitor_info.refresh_rate_ = static_cast<float>(dev_mode.dmDisplayFrequency);
                    monitor_info.monitor_name_ = monitor_device.DeviceString;

                    display_info.monitors_.emplace_back(std::move(monitor_info));
                }
            }

            ++display_index;
        }

        return display_info;
    }

    PlatformInfo PlatformSystem::GetPlatformInfo()
    {
        PlatformInfo platform_info;

        platform_info.operating_system_ = OperatingSystem::kWindows;

        return platform_info;
    }

    /************************************************************************/
    /* PLATFORM THREADING                                                   */
    /************************************************************************/

    size_t PlatformThreading::GetCPUIndex()
    {
        // Group size based on: https://msdn.microsoft.com/en-us/library/windows/desktop/dd405503(v=vs.85).aspx
        static constexpr size_t kGroupSize = 64;

        PROCESSOR_NUMBER processor_number;

        GetCurrentProcessorNumberEx(&processor_number);

        return processor_number.Number + (processor_number.Group * kGroupSize);
    }

    AffinityMask PlatformThreading::GetSystemAffinity()
    {
        size_t process_affinity;
        size_t system_affinity;

        auto result = ::GetProcessAffinityMask(GetCurrentProcess(), &process_affinity, &system_affinity) != 0;

        return result ? system_affinity : size_t(0);
    }

    bool PlatformThreading::SetProcessAffinity(const AffinityMask& affinity_mask)
    {
        HANDLE process_handle = GetCurrentProcess();

        return SetProcessAffinityMask(process_handle, affinity_mask.to_ullong()) != 0;
    }

    AffinityMask PlatformThreading::GetProcessAffinity()
    {
        size_t process_affinity;
        size_t system_affinity;

        auto result = GetProcessAffinityMask(GetCurrentProcess(), &process_affinity, &system_affinity) != 0;

        return result ? process_affinity : size_t(0);
    }

    bool PlatformThreading::SetThreadAffinity(const AffinityMask& affinity_mask, std::thread* thread)
    {
        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        return SetThreadAffinityMask(thread_handle, affinity_mask.to_ullong()) != 0;
    }

    AffinityMask PlatformThreading::GetThreadAffinity(std::thread* thread)
    {
        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        auto process_affinity = GetProcessAffinity();

        auto affinity_mask = SetThreadAffinityMask(thread_handle, process_affinity.to_ullong());        // Returns the old affinity mask.

        SetThreadAffinityMask(thread_handle, affinity_mask);                                            // Restore the original affinity mask. That API, though.

        return affinity_mask;
    }

    bool PlatformThreading::SetThreadPriority(ThreadPriority priority, std::thread* thread)
    {
        static std::unordered_map<ThreadPriority, int> priority_table = 
        {
            { ThreadPriority::kLowest, THREAD_PRIORITY_IDLE },
            { ThreadPriority::kLower, THREAD_PRIORITY_LOWEST },
            { ThreadPriority::kLow, THREAD_PRIORITY_BELOW_NORMAL },
            { ThreadPriority::kNormal, THREAD_PRIORITY_NORMAL },
            { ThreadPriority::kHigh, THREAD_PRIORITY_ABOVE_NORMAL },
            { ThreadPriority::kHigher, THREAD_PRIORITY_HIGHEST },
            { ThreadPriority::kHighest, THREAD_PRIORITY_TIME_CRITICAL }
        };

        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        return ::SetThreadPriority(thread_handle, priority_table[priority]) != 0;
    }

    ThreadPriority PlatformThreading::GetThreadPriority(std::thread* thread)
    {
        static std::unordered_map<int, ThreadPriority> priority_table =
        {
            { THREAD_PRIORITY_IDLE, ThreadPriority::kLowest },
            { THREAD_PRIORITY_LOWEST, ThreadPriority::kLower },
            { THREAD_PRIORITY_BELOW_NORMAL, ThreadPriority::kLow },
            { THREAD_PRIORITY_NORMAL, ThreadPriority::kNormal },
            { THREAD_PRIORITY_ABOVE_NORMAL, ThreadPriority::kHigh },
            { THREAD_PRIORITY_HIGHEST, ThreadPriority::kHigher },
            { THREAD_PRIORITY_TIME_CRITICAL, ThreadPriority::kHighest }
        };

        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        auto priority = ::GetThreadPriority(thread_handle);

        return priority_table[priority];
    }

    /************************************************************************/
    /* PLATFORM MEMORY                                                      */
    /************************************************************************/

    Bytes PlatformMemory::GetPageSize()
    {
        return WindowsMemory::GetInstance().GetPageSize();
    }

    Alignment PlatformMemory::GetPageAlignment()
    {
        return WindowsMemory::GetInstance().GetPageAlignment();
    }

    MemoryRange PlatformMemory::Allocate(Bytes size)
    {
        return MemoryRange(VirtualAlloc(0, std::size_t(size), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE), size);
    }

    MemoryRange PlatformMemory::Reserve(Bytes size)
    {
        return MemoryRange(VirtualAlloc(0, std::size_t(size), MEM_RESERVE, PAGE_READWRITE), size);
    }

    bool PlatformMemory::Release(const MemoryRange& memory_range)
    {
        return VirtualFree(memory_range.GetBase(), 0, MEM_RELEASE) != 0;
    }

    bool PlatformMemory::Commit(const MemoryRange& memory_range)
    {
        return VirtualAlloc(memory_range.GetBase(), std::size_t(memory_range.GetSize()), MEM_COMMIT, PAGE_READWRITE) != nullptr;
    }

    bool PlatformMemory::Decommit(const MemoryRange& memory_range)
    {
        return VirtualFree(memory_range.GetBase(), std::size_t(memory_range.GetSize()), MEM_DECOMMIT) != 0;
    }

}

#endif
