#include "platform/builtin.h"

#include "platform/compiler/compiler.h"

namespace syntropy::platform
{
    /************************************************************************/
    /* BUILTIN                                                              */
    /************************************************************************/

    uint64_t BuiltIn::GetMostSignificantBit(uint64_t number)
    {
        return platform::PlatformBuiltIn::GetMostSignificantBit(number);
    }

    uint64_t BuiltIn::GetLeastSignificantBit(uint64_t number)
    {
        return platform::PlatformBuiltIn::GetLeastSignificantBit(number);
    }

    float BuiltIn::GetFastInvSqrt(float number)
    {
        return platform::PlatformBuiltIn::GetFastInvSqrt(number);
    }
}
