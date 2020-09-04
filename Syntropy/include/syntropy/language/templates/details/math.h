
/// \file math.h
/// \brief This header is part of Syntropy language module. It contains implementation details of math-related template metaprogramming definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/constants.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* GCD                                                                  */
    /************************************************************************/

    /// \brief Euler's algorithm for greatest common divisor.
    template <Int kLeft, Int kRight>
    struct GCD : GCD<kRight, kLeft % kRight>
    {

    };

    /// \brief Specialization for terminal case.
    template <Int kLeft>
    struct GCD<kLeft, 0> : Constant<Int, kLeft>
    {

    };

    /************************************************************************/
    /* LCM                                                                  */
    /************************************************************************/

    /// \brief Euler's algorithm for lowest common multiple.
    template <Int kLeft, Int kRight>
    struct LCM : Constant<Int, (kLeft * kRight) / GCD<kLeft, kRight>::value_>
    {

    };

    /// \brief Special case where both numbers are 0.
    template <>
    struct LCM<0, 0> : Constant<Int, 0>
    {

    };
}