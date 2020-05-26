
/// \file type_traits.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <type_traits>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/syntropy.h"

#include <cstdint>
#include <type_traits>
#include <tuple>

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

    //----------------------------------------------------------------------//

    /// \brief Evaluates to false.
    /// This value can be used to trigger static_assert that evaluates to false.
    template <typename...>
    inline constexpr bool AlwaysFalseV = false;

    //----------------------------------------------------------------------//

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

    //----------------------------------------------------------------------//

    /// \brief Trait used to determine the type of the arguments of a callable object.
    template <typename TCallable>
    struct FunctionArguments : FunctionArguments<decltype(&std::decay_t<TCallable>::operator())> {};

    /// \brief Specialization for member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct FunctionArguments<TReturn(TCallable::*)(TArguments...)>
    {
        using Type = std::tuple<TArguments...>;
    };

    /// \brief Specialization for const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct FunctionArguments<TReturn(TCallable::*)(TArguments...) const>
    {
        using Type = std::tuple<TArguments...>;
    };

    /// \brief Helper type for FunctionArguments<TFunction>.
    template <typename TFunction>
    using FunctionArgumentsT = typename FunctionArguments<TFunction>::Type;

    /// \brief Trait used to determine the type of the kArgumentIndex-th argument of a callable object.
    template <std::int64_t kArgumentIndex, typename TCallable>
    struct FunctionArgument
    {
        using Type = std::tuple_element_t<kArgumentIndex, FunctionArgumentsT<TCallable>>;
    };

    /// \brief Helper type for FunctionArgument<kArgumentIndex, TFunction>.
    template <std::int64_t kArgumentIndex, typename TFunction>
    using FunctionArgumentT = typename FunctionArgument<kArgumentIndex, TFunction>::Type;

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

    /************************************************************************/
    /* STANDARD ALIASES                                                     */
    /************************************************************************/

    /// \brief Wraps the standard type trait std::remove_reference_t<T>
    template <typename TType>
    using RemoveReferenceT = std::remove_reference_t<TType>;

    /// \brief Wraps the standard type trait std::is_polymorphic_v.
    template <typename TType>
    inline constexpr bool IsPolymorphicV = std::is_polymorphic_v<TType>;

    /// \brief Wraps the standard type trait std::is_final_v.
    template <typename TType>
    inline constexpr bool IsFinalV = std::is_final_v<TType>;
}