#include "platform/system.h"

#include "platform/os.h"

namespace syntropy
{
    namespace platform
    {

        //////////////// SYSTEM ////////////////

        System& System::GetInstance()
        {
            return platform::PlatformSystem::GetInstance();
        }

        System& GetSystem()
        {
            return System::GetInstance();
        }

    }
}