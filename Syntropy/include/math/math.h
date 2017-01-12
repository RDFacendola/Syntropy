
/// \file math.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>


namespace syntropy
{

    class Math
    {
    public:

        // TODO: Generalize me
        static constexpr size_t NextMultipleOf(size_t number, size_t multiple);

        // TODO: Generalize me
        static constexpr size_t PreviousMultipleOf(size_t number, size_t multiple);

        // TODO: Ceil(log2(number)). If number is 0 result is 0.
        static size_t CeilLog2(size_t number);

    };

}

namespace syntropy
{
    // Implementation

    //////////////// MATH ////////////////

    inline constexpr size_t Math::NextMultipleOf(size_t number, size_t multiple)
    {
        return PreviousMultipleOf(number + multiple - 1, multiple);
    }

    inline constexpr size_t Math::PreviousMultipleOf(size_t number, size_t multiple)
    {
        return (number / multiple) * multiple;
    }

}