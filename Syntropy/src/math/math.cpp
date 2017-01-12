#include "math/math.h"

#include "platform/builtin.h"

namespace syntropy
{

    //////////////// MATH ////////////////

    size_t Math::CeilLog2(size_t number)
    {
        if (number > 0)
        {
            auto msb = platform::GetBuiltIn().GetMostSignificantBit(number);
            return msb + ((number & (number - 1)) >> msb);        // Ceiling required for non-power of 2.
        }

        return 0;
    }

}