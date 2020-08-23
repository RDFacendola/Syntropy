
/// \file type_traits.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <type_traits>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <cstdint>
#include <type_traits>
#include <tuple>

#include "syntropy/language/language.h"
#include "syntropy/serialization/serialization_types.h"

#include "syntropy/language/type_traits/properties.h"
#include "syntropy/language/type_traits/manipulation.h"
#include "syntropy/language/type_traits/relationships.h"

namespace Syntropy
{
    /************************************************************************/
    /* META                                                                 */
    /************************************************************************/

    /// \brief Type equal to TTrue if condition is true, equal to TFalse otherwise.
    template <Bool Condition, typename TTrue, typename TFalse>
    using ConditionalT = std::conditional_t<Condition, TTrue, TFalse>;

    /************************************************************************/
    /* TYPE QUERY                                                           */
    /************************************************************************/



    /// \brief Constant equal to true if TType is copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyConstructibleV = std::is_copy_constructible_v<TType>;

    /// \brief Constant equal to true if TType is move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveConstructibleV = std::is_move_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDefaultConstructibleV = std::is_trivially_default_constructible_v<TType>;



    /************************************************************************/
    /* TYPE CATEGORIES                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is boolean, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsBooleanV = IsSameV<TType, Bool>;

    /// \brief Constant equal to true if TType is integral, equal to false otherwise.
    /// Differently from standard's std::is_integral, booleans and chars are not considered integral types.
    template <typename TType>
    inline constexpr Bool IsIntegralV = IsSameV<TType, Int> || IsSameV<TType, Fix8> || IsSameV<TType, Fix16> || IsSameV<TType, Fix32> || IsSameV<TType, Fix64>;

    /// \brief Constant equal to true if TType is a floating point, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsFloatingPointV = IsSameV<TType, Float>;

    /************************************************************************/
    /* TYPE TRANSFORMATIONS                                                 */
    /************************************************************************/

    /// \brief Determine the common type among all types TTypes, that is the type to which all TTypes... can be converted to.
    template <typename... TTypes>
    using CommonTypeT = std::common_type_t<TTypes...>;

    /************************************************************************/
    /* STRIP                                                                */
    /************************************************************************/

    /// \brief Type equal to TType after being stripped of either all references or extents, an indirection level or all qualifiers (in this order).
    template <typename TType>
    struct Strip : std::conditional<std::is_array<TType>::value || std::is_reference<TType>::value,
        std::remove_all_extents_t<std::remove_reference_t<TType>>,              // Remove references and extents from the outermost level (mutually exclusive)
        std::conditional_t<std::is_pointer<TType>::value,
        std::remove_pointer_t<TType>,                                           // Remove a pointer level (removes qualifiers as well)
        std::remove_cv_t<TType>>> {};                                           // Remove const and volatile qualifiers from the innermost level

    /// \brief Helper type Strip<TType>.
    template <typename TType>
    using StripT = typename Strip<TType>::type;

    /************************************************************************/
    /* CLASS NAMES                                                          */
    /************************************************************************/

    /// \brief Provides a member typedef which is the same as TType except that any pointer, qualifiers, references and extents are removed recursively.
    /// For example: ClassName<int ** const *[1][3]> has a member Type 'int'.
    template <typename TType, typename = void>
    struct ClassName
    {
        using Type = TType;
    };

    /// \brief Recursive specialization which strips an indirection level \ qualifiers until the class name is reached.
    template <typename TType>
    struct ClassName<TType, std::enable_if_t<!std::is_same_v<StripT<TType>, TType>>> : ClassName<StripT<TType>> {};

    /// \brief Helper type for ClassName<TType>.
    template <typename TType>
    using ClassNameT = typename ClassName<TType>::Type;

    /// \brief Constant equal to true if TType is a class name without reference, qualifiers, extents and indirection levels, equal to false otherwise.
    template <typename TType>
    constexpr Bool IsClassNameV = std::is_same<ClassNameT<TType>, TType>::value;

    /************************************************************************/
    /* IS SPECIALIZATION                                                    */
    /************************************************************************/

    /// \brief If TType is a specialization of TTemplate IsSpecializationV is true, false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsSpecializationV = false;

    /// \brief If TType is a specialization of TTemplate IsSpecializationV is true, false otherwise.
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr Bool IsSpecializationV<TTemplate<TTypes...>, TTemplate> = true;

    /************************************************************************/
    /* ALWAYS FALSE                                                         */
    /************************************************************************/

    /// \brief Evaluates to false.
    /// This value can be used to trigger static_assert that evaluates to false.
    template <typename...>
    inline constexpr Bool AlwaysFalseV = false;

    /************************************************************************/
    /* IS CONTIGUOUS SEQUENCE                                               */
    /************************************************************************/

    /// \brief If the sequence {Ints} is contiguous provides a member constant value equal to true, otherwise value is false.
    template <Int... Ints>
    struct IsContiguousSequence : std::true_type {};

    /// \brief Specialization for two-element sequences or more.
    template <Int Int1, Int Int2, Int... Ints>
    struct IsContiguousSequence<Int1, Int2, Ints...>
    {
        static constexpr Bool Value = (Int1 + 1 == Int2) && IsContiguousSequence<Int2, Ints...>::Value;
    };

    /// \brief Helper value for IsContiguousSequence<T, Ints...>.
    template <Int... Ints>
    constexpr Bool IsContiguousSequenceV = IsContiguousSequence<Ints...>::Value;

    /************************************************************************/
    /* FUNCTION ARGUMENTS                                                   */
    /************************************************************************/

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
    template <Int kArgumentIndex, typename TCallable>
    struct FunctionArgument
    {
        using Type = std::tuple_element_t<kArgumentIndex, FunctionArgumentsT<TCallable>>;
    };

    /// \brief Helper type for FunctionArgument<kArgumentIndex, TFunction>.
    template <Int kArgumentIndex, typename TFunction>
    using FunctionArgumentT = typename FunctionArgument<kArgumentIndex, TFunction>::Type;

    /************************************************************************/
    /* TUPLE ELEMENT INDEX                                                  */
    /************************************************************************/

    /// \brief Provides a member constant value equal to the index of the first tuple element whose type is equal to TType.
    template <typename TType, typename TTuple>
    struct TupleElementIndex;

    /// \brief Partial template specialization when the first element of the tuple is equal to TType.
    template <typename TType, typename... TTypes>
    struct TupleElementIndex<TType, std::tuple<TType, TTypes...>>
    {
        static constexpr Int Value = 0;
    };

    /// \brief Partial template specialization when the first element in the tuple is not equal to TType. Discard the element and increase the value by one.
    template <typename TType, typename TDiscard, typename... TTypes>
    struct TupleElementIndex<TType, std::tuple<TDiscard, TTypes...>>
    {
        static constexpr Int Value = 1 + TupleElementIndex<TType, std::tuple<TTypes...>>::Value;
    };

    /// \brief Helper value for tuple_element_index<TType, TTuple>.
    template <typename TType, typename TTuple>
    constexpr Int TupleElementIndexV = TupleElementIndex<TType, TTuple>::Value;

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
    /// \usage template<typename T> using HasFoo = decltype(&T::Foo());     // Expression type.
    ///        auto b = IsValidExpressionV<HasFoo, MyType>;                 // true if MyType::Foo is valid, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpressionV = IsValidExpression<void, TExpression, TTypes...>::value;

    /************************************************************************/
    /* ENABLE IF                                                            */
    /************************************************************************/

    /// \brief If kEnable is true exposes a member typedef equal to TType, otherwise there's no member typedef.
    template <Bool kEnable, typename TType = void>
    using EnableIfT = std::enable_if_t<kEnable, TType>;

    /************************************************************************/
    /* ENABLE IF VALID EXPRESSION                                           */
    /************************************************************************/

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no member typedef.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpressionT = EnableIfT<IsValidExpressionV<TExpression, TTypes...>>;

    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

    /// \brief Check if T can be compared equal to U.
    template <typename T, typename U>
    using HasEqualityComparison = decltype(std::declval<T>() == std::declval<U>());

    /// \brief Check if T can be compared inequal to U.
    template <typename T, typename U>
    using HasInequalityComparison = decltype(std::declval<T>() != std::declval<U>());

}