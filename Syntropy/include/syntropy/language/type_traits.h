
/// \file type_traits.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <type_traits>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/syntropy.h"

#include <type_traits>

namespace syntropy
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief If TType is a specialization of TTemplate IsSpecializationV is true, false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr bool IsSpecializationV = false;

    /// \brief If TType is a specialization of TTemplate IsSpecializationV is true, false otherwise.
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr bool IsSpecializationV<TTemplate<TTypes...>, TTemplate> = true;

    /// \brief Evaluates to false.
    /// This value can be used to trigger static_assert that evaluates to false.
    template <typename...>
    inline constexpr bool AlwaysFalseV = false;

    /// \brief If the sequence {Ints} is contiguous provides a member constant value equal to true, otherwise value is false.
    template <std::int64_t... Ints>
    struct IsContiguousSequence : std::true_type {};

    /// \brief Specialization for two-element sequences or more.
    template <std::int64_t Int1, std::int64_t Int2, std::int64_t... Ints>
    struct IsContiguousSequence<Int1, Int2, Ints...>
    {
        static constexpr bool Value = (Int1 + 1 == Int2) && IsContiguousSequence<Int2, Ints...>::Value;
    };

    /// \brief Helper value for IsContiguousSequence<T, Ints...>.
    template <std::int64_t... Ints>
    constexpr bool IsContiguousSequenceV = IsContiguousSequence<Ints...>::Value;

    /************************************************************************/
    /* IS VALID EXPRESSION                                                  */
    /************************************************************************/

    /// \brief If TExpression<TTypes...> is a valid expression, exposes a member value equal to true, otherwise exposes a member value equal to false.
    /// Default trait for invalid expressions.
    /// \see Based on std::experimental::is_detected.
    template <typename TVoid, template<typename...> typename TExpression, typename... TTypes>
    struct IsValidExpression : std::false_type {};

    /// \brief Partial specialization for valid expressions.
    template <template<typename...> typename TExpression, typename... TTypes>
    struct IsValidExpression<std::void_t<TExpression<TTypes...>>, TExpression, TTypes...> : std::true_type {};

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a valid expression, false otherwise.
    ///
    /// \usage template<typename T> using HasFoo = decltype(&T::Foo);       // Expression type.
    ///        auto b = IsValidExpressionV<HasFoo, MyType>;                 // true if MyType::Foo is valid, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr bool IsValidExpressionV = IsValidExpression<void, TExpression, TTypes...>::value;

}