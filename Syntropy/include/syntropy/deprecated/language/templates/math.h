
/// \file math.h
/// \brief This header is part of Syntropy language module. It contains math-related template metaprogramming definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once


#include "syntropy/language/foundation/types.h"

#include "syntropy/math/math.h"

#include "syntropy/language/templates/details/math_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* COMPILE-TIME MATH                                                    */
    /************************************************************************/

    /// \brief Get the absolute value of VValue.
    template <Int VValue>
    inline constexpr Int Abs = Math::Abs(VValue);

    /// \brief Get the greatest common divisor between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int GCD = Details::GCD<VLeft, VRight>;

    /// \brief Get the lowest common multiple between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int LCM = Details::LCM<VLeft, VRight>;
}

// ===========================================================================