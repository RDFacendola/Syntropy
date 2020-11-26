#include "syntropy/platform/intrinsics.h"

#include "syntropy/hal/hal_intrinsics.h"

namespace Syntropy
{
    /************************************************************************/
    /* INTRINSICS                                                           */
    /************************************************************************/

    Int Intrinsics::GetMostSignificantBit(Int rhs)
    {
        return HALIntrinsics::GetMostSignificantBit(rhs);
    }

    Int Intrinsics::GetLeastSignificantBit(Int rhs)
    {
        return HALIntrinsics::GetLeastSignificantBit(rhs);
    }

    Float Intrinsics::GetFastInvSqrt(Float rhs)
    {
        return HALIntrinsics::GetFastInvSqrt(rhs);
    }

}