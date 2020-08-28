
/// \file type_traits.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <type_traits>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <cstdint>
#include <type_traits>
#include <tuple>

#include "syntropy/language/foundation.h"
#include "syntropy/serialization/serialization_types.h"

#include "syntropy/language/type_traits/constants.h"
#include "syntropy/language/type_traits/properties.h"
#include "syntropy/language/type_traits/manipulation.h"
#include "syntropy/language/type_traits/relationships.h"
#include "syntropy/language/type_traits/categories.h"
#include "syntropy/language/type_traits/operations.h"
#include "syntropy/language/type_traits/meta.h"
#include "syntropy/language/type_traits/capabilities.h"
#include "syntropy/language/type_traits/utility.h"

namespace Syntropy
{
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

}