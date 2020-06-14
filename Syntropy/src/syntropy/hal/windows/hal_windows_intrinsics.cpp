#ifdef _WIN64

#include "syntropy/hal/hal_intrinsics.h"

/************************************************************************/
/* HEADERS & LIBRARIES                                                  */
/************************************************************************/

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>
#include <intrin.h>
#undef max

#pragma warning(pop)

namespace syntropy
{
    /************************************************************************/
    /* HAL INTRINSICS                                                       */
    /************************************************************************/

    Int HALIntrinsics::GetMostSignificantBit(Int rhs)
    {
        auto index = DWORD{};

        _BitScanReverse64(&index, rhs);

        return ToInt(index);
    }

    Int HALIntrinsics::GetLeastSignificantBit(Int rhs)
    {
        auto index = DWORD{};

        _BitScanForward64(&index, rhs);

        return ToInt(index);
    }
}

#endif