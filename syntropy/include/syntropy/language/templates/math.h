
/// \file math.h
/// \brief This header is part of Syntropy language module.
///        It contains math-related template metaprogramming definitions.
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/details/math_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* COMPILE-TIME MATH                                                    */
    /************************************************************************/

    /// \brief Get the greatest common divisor between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int GCD = Details::GCD<VLeft, VRight>;

    /// \brief Get the lowest common multiple between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int LCM = Details::LCM<VLeft, VRight>;
}

// ===========================================================================
