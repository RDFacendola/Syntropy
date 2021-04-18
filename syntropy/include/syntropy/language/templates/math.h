
/// \file math.h
///
/// \brief This header is part of Syntropy language module.
///        It contains math-related template metaprogramming definitions.
///
/// \author Raffaele D. Facendola - September 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

#include "details/math.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* COMPILE-TIME MATH                                                    */
    /************************************************************************/

    /// \brief Get the greatest common divisor between TLeft and TRight.
    template <Int TLeft, Int TRight>
    inline constexpr Int GCD
        = Details::GCD<TLeft, TRight>;

    /// \brief Get the lowest common multiple between TLeft and TRight.
    template <Int TLeft, Int TRight>
    inline constexpr Int LCM
        = Details::LCM<TLeft, TRight>;
}

// ===========================================================================
