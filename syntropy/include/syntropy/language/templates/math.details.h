
/// \file math_details.h
///
/// \author Raffaele D. Facendola - September 2020.

#pragma once

#include <numeric>

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* COMPILE-TIME MATH                                                    */
    /************************************************************************/

    /// \brief Get the greatest common divisor between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int GCD = std::gcd(VLeft, VRight);

    /// \brief Get the lowest common multiple between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int LCM = std::lcm(VLeft, VRight);
}

// ===========================================================================
