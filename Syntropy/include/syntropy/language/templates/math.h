
/// \file math.h
/// \brief This header is part of Syntropy language module. It contains math-related template metaprogramming definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/details/math.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* GCD                                                                  */
    /************************************************************************/

    /// \brief Get the greatest common divisor between kLeft and kRight.
    template <Int kLeft, Int kRight>
    using GCD = Details::GCD<kLeft, kRight>;

    /************************************************************************/
    /* LCM                                                                  */
    /************************************************************************/

    /// \brief Get the lowest common multiple between kLeft and kRight.
    template <Int kLeft, Int kRight>
    using LCM = Details::LCM<kLeft, kRight>;
}