
/// \file sfinae.h
/// \brief This header is part of Syntropy language module. It contains implementation details of SFINAE-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/constants.h"

namespace Syntropy::Traits::Details
{
    /************************************************************************/
    /* IS VALID EXPRESSION                                                  */
    /************************************************************************/

    /// \brief If TExpression<TTypes...> is a valid expression, exposes a member value equal to true, otherwise exposes a member value equal to false.
    /// Default trait for invalid expressions.
    /// \see Based on std::experimental::is_detected.
    template <typename TVoid, template<typename...> typename TExpression, typename... TTypes>
    struct DetectValidExpression : Templates::False {};

    /// \brief Partial specialization for valid expressions.
    template <template<typename...> typename TExpression, typename... TTypes>
    struct DetectValidExpression<Templates::Void<TExpression<TTypes...>>, TExpression, TTypes...> : Templates::True {};

}