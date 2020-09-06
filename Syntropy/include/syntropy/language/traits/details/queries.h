
/// \file queries.h
/// \brief This header is part of Syntropy language module. It contains implementation details of queries-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/support/type_list.h"
#include "syntropy/language/templates/constants.h"

namespace Syntropy::Traits::Details
{
    /************************************************************************/
    /* ELEMENT TYPE                                                         */
    /************************************************************************/

    /// \brief Provides a type alias Type which is equal to the VIndex-th type in a type list.
    /// If the provided index exceeds the type-list range, the program is ill-formed.
    template <Int VIndex, typename TType, typename... TTypes>
    struct ElementType : ElementType<VIndex - 1, TTypes...>
    {
        static_assert(VIndex >= 0, "Invalid index");
    };

    /// \brief Provides a type alias Type which is equal to the VIndex-th type in a type list.
    /// Partial specialization which returns the first type in a type list.
    template <typename TType, typename... TTypes>
    struct ElementType<0, TType, TTypes...> : Templates::Alias<TType>
    {

    };

    /// \brief Provides a type alias Type which is equal to the VIndex-th type in a type list.
    /// Partial specialization for type lists.
    template <Int VIndex, typename... TTypes>
    struct ElementType<VIndex, TypeList<TTypes...>> : ElementType<VIndex, TTypes...>
    {

    };

    /************************************************************************/
    /* ELEMENT INDEX                                                        */
    /************************************************************************/

    /// \brief Provide a member constant value_ which is equal to the index of the element with type TMatch among a list of types [TType, TTypes...].
    template <typename TMatch, typename TType, typename... TTypes>
    struct ElementIndex : Templates::Constant<Int, 1 + ElementIndex<TMatch, TTypes...>::value_>
    {

    };

    /// \brief Provide a member constant value_ which is equal to the index of the element with type TMatch among a list of types [TMatch, TTypes...].
    /// Partial specialization for a list of types starting with TMatch as type.
    template <typename TMatch, typename... TTypes>
    struct ElementIndex<TMatch, TMatch, TTypes...> : Templates::Constant<Int, 0>
    {

    };

    /// \brief Provide a member constant value_ which is equal to the index of the element with type TMatch among a list of types [TTypes...].
    /// Partial specialization for type lists.
    template <typename TMatch, typename... TTypes>
    struct ElementIndex<TMatch, TypeList<TTypes...>> : ElementIndex<TMatch, TTypes...>
    {

    };

    /************************************************************************/
    /* FUNCTION ARGUMENTS                                                   */
    /************************************************************************/

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for lambdas and callable objects.
    template <typename TCallable>
    struct FunctionArguments : FunctionArguments<decltype(&Decay<TCallable>::operator())>
    {
        
    };

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for non-const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct FunctionArguments<TReturn(TCallable::*)(TArguments...)> : Templates::Alias<TypeList<TArguments...>>
    {

    };

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct FunctionArguments<TReturn(TCallable::*)(TArguments...) const> : Templates::Alias<TypeList<TArguments...>>
    {

    };

}