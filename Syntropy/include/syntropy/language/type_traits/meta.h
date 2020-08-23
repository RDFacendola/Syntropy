
/// \file meta.h
/// \brief This header is part of Syntropy language module. It contains template metaprogramming-related type traits.
///
/// This header contains mostly aliases to the standard traits, eventually modified to suite Syntropy needs and to enforce code style consistency.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/fundamentals.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* CONDITIONAL                                                          */
    /************************************************************************/

    /// \brief Type equal to TTrue if kCondition is true, equal to TFalse otherwise.
    template <Bool kCondition, typename TTrue, typename TFalse>
    using Conditional = std::conditional_t<kCondition, TTrue, TFalse>;

    /************************************************************************/
    /* ENABLE IF                                                            */
    /************************************************************************/

    /// \brief Type equal to TType if kEnable is true, otherwise there's no such type.
    template <Bool kEnable, typename TType = void>
    using EnableIf = std::enable_if_t<kEnable, TType>;

}