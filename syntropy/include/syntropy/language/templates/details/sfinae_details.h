
/// \file sfinae_details.h
///
/// \brief This header is part of Syntropy language module.
///        It contains implementation details of sfinae.h.
///
/// \author Raffaele D. Facendola - February 2021.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* SFINAE                                                               */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename... TTypes>
    using Void = void;

    /// \brief Type equal to TType if TEnable is true, otherwise there's
    ///        no such type.
    template <Bool TEnable>
    using EnableIf
        = std::enable_if_t<TEnable>;

    /// \brief If TExpression<TTypes...> is a valid expression, exposes a
    ///        member value equal to true, otherwise exposes a member value
    ///        equal to false.
    /// Default trait for invalid expressions.
    /// \see Based on std::experimental::is_detected.
    template <typename TVoid,
              template<typename...> typename TExpression,
              typename... TTypes>
    struct IsValidExpressionHelper
    {
        static constexpr Bool kValue = false;
    };

    /// \brief Partial specialization for valid expressions.
    template <template<typename...> typename TExpression,
              typename... TTypes>
    struct IsValidExpressionHelper<Void<TExpression<TTypes...>>,
                                   TExpression,
                                   TTypes...>
    {
        static constexpr Bool kValue = true;
    };

    /// \brief Boolean value equal to true if TExpression<TTypes...>
    ///        is a valid expression, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpression
        = IsValidExpressionHelper<void, TExpression, TTypes...>::kValue;

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member
    ///        typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression
        = EnableIf<IsValidExpression<TExpression, TTypes...>>;
}

// ===========================================================================
