#include "syntropy/platform/intrinsics.h"

#include "syntropy/hal/hal_intrinsics.h"

namespace syntropy
{
    /************************************************************************/
    /* INTRINSICS                                                           */
    /************************************************************************/

    std::int64_t Intrinsics::GetMostSignificantBit(std::int64_t rhs)
    {
        return HALIntrinsics::GetMostSignificantBit(rhs);
    }

    std::int64_t Intrinsics::GetLeastSignificantBit(std::int64_t rhs)
    {
        return HALIntrinsics::GetLeastSignificantBit(rhs);
    }

    float Intrinsics::GetFastInvSqrt(float rhs)
    {
        return HALIntrinsics::GetFastInvSqrt(rhs);
    }

}