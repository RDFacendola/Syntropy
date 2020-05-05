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

    std::int64_t HALIntrinsics::GetMostSignificantBit(std::int64_t rhs)
    {
        auto index = DWORD{};

        _BitScanReverse64(&index, rhs);

        return static_cast<int64_t>(index);
    }

    std::int64_t HALIntrinsics::GetLeastSignificantBit(std::int64_t rhs)
    {
        auto index = DWORD{};

        _BitScanForward64(&index, rhs);

        return static_cast<int64_t>(index);
    }
}

#endif