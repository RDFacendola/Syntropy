#include "platform/builtin.h"

#include "platform/compiler.h"

namespace syntropy
{
    namespace platform
    {
        //////////////// BUILTIN ////////////////

        BuiltIn& BuiltIn::GetInstance()
        {
            return platform::PlatformBuiltIn::GetInstance();
        }

        BuiltIn& GetBuiltIn()
        {
            return BuiltIn::GetInstance();
        }
    }
}
