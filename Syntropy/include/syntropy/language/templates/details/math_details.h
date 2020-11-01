
/// \file math_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of math-related template metaprogramming definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/templates_details.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* GCD                                                                  */
    /************************************************************************/

    /// \brief Euler's algorithm for greatest common divisor.
    template <Int VLeft, Int VRight>
    struct GCD : GCD<VRight, VLeft % VRight> {};

    /// \brief Specialization for terminal case.
    template <Int VLeft>
    struct GCD<VLeft, 0> : Constant<Int, VLeft> {};

    /************************************************************************/
    /* LCM                                                                  */
    /************************************************************************/

    /// \brief Euler's algorithm for lowest common multiple.
    template <Int VLeft, Int VRight>
    struct LCM : Constant<Int, (VLeft * VRight) / GCD<VLeft, VRight>::kValue> {};

    /// \brief Special case where both numbers are 0.
    template <>
    struct LCM<0, 0> : Constant<Int, 0> {};
}

// ===========================================================================
