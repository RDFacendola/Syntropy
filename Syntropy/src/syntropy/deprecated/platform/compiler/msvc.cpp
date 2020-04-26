#define SYNTROPY_COMPILER_INCLUDE_GUARD

#include "syntropy/platform/compiler/msvc.h"

#undef SYNTROPY_COMPILER_INCLUDE_GUARD

#ifdef _MSC_VER

#include <intrin.h>

namespace syntropy::platform
{
    /************************************************************************/
    /* PLATFORM BUILTIN                                                     */
    /************************************************************************/

    uint64_t PlatformBuiltIn::GetMostSignificantBit(uint64_t number)
    {
        unsigned long index;
        _BitScanReverse64(&index, number);
        return static_cast<uint64_t>(index);
    }

    uint64_t PlatformBuiltIn::GetLeastSignificantBit(uint64_t number)
    {
        unsigned long index;
        _BitScanForward64(&index, number);
        return static_cast<uint64_t>(index);
    }

    float PlatformBuiltIn::GetFastInvSqrt(float number)
    {
        __m128 mm_number = _mm_load_ss(&number);

        _mm_store_ss(&number, _mm_rsqrt_ss(mm_number));

        return number;
    }
}

#endif
