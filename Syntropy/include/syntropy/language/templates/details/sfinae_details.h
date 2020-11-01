
/// \file sfinae_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of SFINAE-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/templates/details/templates_details.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* ENABLE IF                                                            */
    /************************************************************************/

    /// \brief Type equal to TType if VEnable is true, otherwise there's no such type.
    template <Bool VEnable>
    using EnableIf = std::enable_if_t<VEnable>;

    /************************************************************************/
    /* IS VALID EXPRESSION                                                  */
    /************************************************************************/

    /// \brief If TExpression<TTypes...> is a valid expression, exposes a member value equal to true, otherwise exposes a member value equal to false.
    /// Default trait for invalid expressions.
    /// \see Based on std::experimental::is_detected.
    template <typename TVoid, template<typename...> typename TExpression, typename... TTypes>
    struct IsValidExpressionHelper : False {};

    /// \brief Partial specialization for valid expressions.
    template <template<typename...> typename TExpression, typename... TTypes>
    struct IsValidExpressionHelper<Void<TExpression<TTypes...>>, TExpression, TTypes...> : True {};

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a valid expression, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpression = IsValidExpressionHelper<void, TExpression, TTypes...>::kValue;

    /************************************************************************/
    /* ENABLE IF VALID EXPRESSION                                           */
    /************************************************************************/

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression = EnableIf<IsValidExpression<TExpression, TTypes...>>;
}

// ===========================================================================