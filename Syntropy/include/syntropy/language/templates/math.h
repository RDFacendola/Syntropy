
/// \file math.h
/// \brief This header is part of Syntropy language module. It contains math-related template metaprogramming definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/details/math.h"
#include "syntropy/math/math.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* ABS                                                                  */
    /************************************************************************/

    /// \brief Get the absolute value of VValue.
    template <Int VValue>
    inline constexpr Int Abs = Math::Abs(VValue);

    /************************************************************************/
    /* GCD                                                                  */
    /************************************************************************/

    /// \brief Get the greatest common divisor between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int GCD = Details::GCD<VLeft, VRight>::kValue;

    /************************************************************************/
    /* LCM                                                                  */
    /************************************************************************/

    /// \brief Get the lowest common multiple between VLeft and VRight.
    template <Int VLeft, Int VRight>
    inline constexpr Int LCM = Details::LCM<VLeft, VRight>::kValue;
}