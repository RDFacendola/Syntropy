#include "syntropy/math/math.h"

namespace syntropy
{
    float FastTanh(float rhs)
    {
        // Pade approximation.

        if (rhs < -3.0f)
        {
            return -1.0f;
        }

        if (rhs > 3.0f)
        {
            return +1.0f;
        }

        float rhs2 = rhs * rhs;

        return rhs * (27.0f + rhs2) / (27.0f + 9.0f * rhs2);
    }

}