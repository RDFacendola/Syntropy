#include "math/math.h"

namespace syntropy
{

    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    float Math::FastInvSqrt(float number)
    {
        return platform::BuiltIn::GetFastInvSqrt(number);
    }

    float Math::FastSqrt(float number)
    {
        return 1.0f / FastInvSqrt(number);
    }

}