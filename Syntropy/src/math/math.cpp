#include "math/math.h"

namespace syntropy
{

    //////////////// MATH ////////////////

    size_t Math::NextMultipleOf(size_t number, size_t multiple)
    {
        return PreviousMultipleOf(number + multiple - 1, multiple);
    }

    size_t Math::PreviousMultipleOf(size_t number, size_t multiple)
    {
        return (number / multiple) * multiple;
    }

}