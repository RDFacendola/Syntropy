#include "syntropy/platform/system.h"

#include "syntropy/hal/hal_system.h"

namespace syntropy
{
    /************************************************************************/
    /* SYSTEM                                                               */
    /************************************************************************/

    CPUInfo System::GetCPUInfo()
    {
        return HALSystem::GetCPUInfo();
    }

    StorageInfo System::GetStorageInfo()
    {
        return HALSystem::GetStorageInfo();
    }

    MemoryInfo System::GetMemoryInfo()
    {
        return HALSystem::GetMemoryInfo();
    }

    DisplayInfo System::GetDisplayInfo()
    {
        return HALSystem::GetDisplayInfo();
    }

    PlatformInfo System::GetPlatformInfo()
    {
        return HALSystem::GetPlatformInfo();
    }

}