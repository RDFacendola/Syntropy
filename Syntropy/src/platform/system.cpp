#include "platform/system.h"

#include "platform/os/os.h"

namespace syntropy
{
    namespace platform
    {

        /************************************************************************/
        /* SYSTEM                                                               */
        /************************************************************************/

        CPUInfo System::GetCPUInfo()
        {
            return PlatformSystem::GetCPUInfo();
        }

        StorageInfo System::GetStorageInfo()
        {
            return PlatformSystem::GetStorageInfo();
        }

        MemoryInfo System::GetMemoryInfo()
        {
            return PlatformSystem::GetMemoryInfo();
        }

        DisplayInfo System::GetDisplayInfo()
        {
            return PlatformSystem::GetDisplayInfo();
        }

        PlatformInfo System::GetPlatformInfo()
        {
            return PlatformSystem::GetPlatformInfo();
        }

    }
}