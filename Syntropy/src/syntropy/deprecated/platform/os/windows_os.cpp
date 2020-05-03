#define SYNTROPY_OS_INCLUDE_GUARD

#include "syntropy/platform/os/windows_os.h"

#undef SYNTROPY_OS_INCLUDE_GUARD

#ifdef _WIN64

#pragma comment(lib, "DbgHelp.lib")

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#undef max

#pragma warning(pop)

#include "syntropy/types/string.h"
#include <mutex>
#include <thread>
#include <algorithm>
#include <memory>
#include "syntropy/containers/map.h"

#include "syntropy/math/math.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/virtual_memory_page.h"
#include "syntropy/diagnostics/assert.h"

#include "syntropy/syntropy.h"

namespace syntropy::platform
{
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
        static Map<ThreadPriority, int> priority_table = 
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
        static Map<int, ThreadPriority> priority_table =
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

}

#endif
