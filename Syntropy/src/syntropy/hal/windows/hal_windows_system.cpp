#ifdef _WIN64

#include "syntropy/hal/hal_system.h"

/************************************************************************/
/* HEADERS & LIBRARIES                                                  */
/************************************************************************/

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#undef max

#pragma warning(pop)

namespace syntropy
{
    /************************************************************************/
    /* HAL SYSTEM                                                           */
    /************************************************************************/

    CPUInfo HALSystem::GetCPUInfo()
    {
        auto cpu_info = CPUInfo{};
        auto frequency = LARGE_INTEGER{};
        auto system_info = SYSTEM_INFO{};

        GetSystemInfo(&system_info);
        auto b  = QueryPerformanceFrequency(&frequency);

        cpu_info.cores_ = static_cast<Int>(system_info.dwNumberOfProcessors);      // Accounts for logical processors.
        cpu_info.frequency_ = static_cast<Int>(frequency.QuadPart) * 1000ll;       // MHz -> Hz.

        if (system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        {
            cpu_info.architecture_ = CPUArchitecture::kx64;
        }
        else if (system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
        {
            cpu_info.architecture_ = CPUArchitecture::kx86;
        }
        else if (system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM)
        {
            cpu_info.architecture_ = CPUArchitecture::kARM;
        }
        else
        {
            cpu_info.architecture_ = CPUArchitecture::kUnknown;
        }

        return cpu_info;
    }

    StorageInfo HALSystem::GetStorageInfo()
    {
        auto storage_info = StorageInfo{};
        
        char unit_path[] = "A:\\";
        auto size = ULARGE_INTEGER{};
        auto available_space = ULARGE_INTEGER{};
        auto drive_info = DriveInfo{};

        for (auto drive_mask = GetLogicalDrives(); drive_mask != 0; drive_mask >>= 1)
        {
            auto drive_type = GetDriveTypeA(unit_path);

            if ((drive_mask & 1) &&  ((drive_type == DRIVE_FIXED) || (drive_type == DRIVE_REMOVABLE)))
            {
                GetDiskFreeSpaceExA(unit_path, NULL, &size, &available_space);

                drive_info.label_ = unit_path;
                drive_info.total_space_ = ToBytes(size.QuadPart);
                drive_info.available_space_ = ToBytes(available_space.QuadPart);

                storage_info.drives_.emplace_back(std::move(drive_info));
            }

            ++unit_path[0];
        }

        return storage_info;
    }

    MemoryInfo HALSystem::GetMemoryInfo()
    {
        auto memory_info = MemoryInfo{};
        auto memory_status = MEMORYSTATUSEX{};

        memory_status.dwLength = sizeof(MEMORYSTATUSEX);

        GlobalMemoryStatusEx(&memory_status);

        memory_info.total_physical_memory_ = ToBytes(memory_status.ullTotalPhys);
        memory_info.total_virtual_memory_ = ToBytes(memory_status.ullTotalVirtual);
        memory_info.total_page_memory_ = ToBytes(memory_status.ullTotalPageFile);
        memory_info.available_physical_memory_ = ToBytes(memory_status.ullAvailPhys);
        memory_info.available_virtual_memory_ = ToBytes(memory_status.ullAvailVirtual);
        memory_info.available_page_memory_ = ToBytes(memory_status.ullAvailPageFile);

        return memory_info;
    }

    DisplayInfo HALSystem::GetDisplayInfo()
    {
        auto display_index = DWORD{ 0 };
        auto adapter_device = DISPLAY_DEVICEA{};
        auto monitor_device = DISPLAY_DEVICEA{};
        auto dev_mode = DEVMODEA{};

        auto display_info = DisplayInfo{};
        auto monitor_info = MonitorInfo{};

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
                    monitor_info.width_ = static_cast<Int>(dev_mode.dmPelsWidth);
                    monitor_info.height_ = static_cast<Int>(dev_mode.dmPelsHeight);
                    monitor_info.refresh_rate_ = static_cast<Float>(dev_mode.dmDisplayFrequency);
                    monitor_info.device_name_ = monitor_device.DeviceString;

                    display_info.monitors_.emplace_back(std::move(monitor_info));
                }
            }

            ++display_index;
        }

        return display_info;
    }

    PlatformInfo HALSystem::GetPlatformInfo()
    {
        auto platform_info = PlatformInfo{};

        platform_info.operating_system_ = OperatingSystem::kWindows;

        return platform_info;
    }

}

#endif