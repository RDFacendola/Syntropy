#ifdef _M_X64

#include "syntropy/hal/hal_intrinsics.h"

/************************************************************************/
/* HEADERS & LIBRARIES                                                  */
/************************************************************************/

#include <xmmintrin.h>

namespace syntropy
{
    /************************************************************************/
    /* HAL INTRINSICS                                                       */
    /************************************************************************/

    float HALIntrinsics::GetFastInvSqrt(float rhs)
    {
        auto mm_number = _mm_load_ss(&rhs);

        _mm_store_ss(&rhs, _mm_rsqrt_ss(mm_number));

        return rhs;
    }

}

#endif