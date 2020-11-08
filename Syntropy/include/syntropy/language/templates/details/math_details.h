
/// \file math_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of math-related template metaprogramming definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* GREATEST COMMON DIVISOR                                              */
    /************************************************************************/

    /// \brief Euler's algorithm for greatest common divisor.
    template <Int VLeft, Int VRight>
    struct GCDHelper : GCDHelper<VRight, VLeft % VRight>
    {
    
    };

    /// \brief Specialization for terminal case.
    template <Int VLeft>
    struct GCDHelper<VLeft, 0>
    {
        static constexpr Int kValue = VLeft;
    };

    /// \brief Get the greatest common divisor between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int GCD = GCDHelper<VLeft, VRight>::kValue;

    /************************************************************************/
    /* LOWEST COMMON MULTIPLE                                               */
    /************************************************************************/

    /// \brief Euler's algorithm for lowest common multiple.
    template <Int VLeft, Int VRight>
    struct LCMHelper
    {
        static constexpr Int kValue = (VLeft * VRight) / GCDHelper<VLeft, VRight>::kValue;
    };

    /// \brief Special case where both numbers are 0.
    template <>
    struct LCMHelper<0, 0>
    {
        static constexpr Int kValue = 0;
    };

    /// \brief Get the lowest common multiple between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int LCM = LCMHelper<VLeft, VRight>::kValue;
}

// ===========================================================================
