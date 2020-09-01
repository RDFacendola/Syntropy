
/// \file meta.h
/// \brief This header is part of Syntropy language module. It contains template metaprogramming-related type traits.
///
/// This header contains mostly aliases to the standard traits, eventually modified to suite Syntropy needs and to enforce code style consistency.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/type_traits/constants.h"

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

    /************************************************************************/
    /* LOGIC                                                                */
    /************************************************************************/

    /// \brief Boolean value equal to the conjunction (logical AND) on a sequence of traits.
    template <typename... TPredicates>
    inline constexpr Bool Conjunction = std::conjunction_v<TPredicates...>;

    /// \brief Boolean value equal to the disjunction (logical OR) on a sequence of traits.
    template <typename... TPredicates>
    inline constexpr Bool Disjunction = std::disjunction_v<TPredicates...>;

    /// \brief Boolean value equal to the negation (logical NOT) of a type traits.
    template <typename TPredicate>
    inline constexpr Bool Negation = std::negation_v<TPredicate>;

    /************************************************************************/
    /* IS VALID EXPRESSION                                                  */
    /************************************************************************/

    /// \brief If TExpression<TTypes...> is a valid expression, exposes a member value equal to true, otherwise exposes a member value equal to false.
    /// Default trait for invalid expressions.
    /// \see Based on std::experimental::is_detected.
    template <typename TVoid, template<typename...> typename TExpression, typename... TTypes>
    struct DetectValidExpression : False {};

    /// \brief Partial specialization for valid expressions.
    template <template<typename...> typename TExpression, typename... TTypes>
    struct DetectValidExpression<Void<TExpression<TTypes...>>, TExpression, TTypes...> : True {};

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a valid expression, false otherwise.
    ///
    /// \usage template<typename T> using HasFoo = decltype(&T::Foo());     // Expression type.
    ///        auto b = Traits::IsValidExpression<HasFoo, MyType>;                 // true if MyType::Foo is valid, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpression = DetectValidExpression<void, TExpression, TTypes...>::Value;

    /************************************************************************/
    /* ENABLE IF VALID EXPRESSION                                           */
    /************************************************************************/

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression = Traits::EnableIf<IsValidExpression<TExpression, TTypes...>>;

}