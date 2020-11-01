
/// \file tuple_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details for functional-style templates.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/type_list.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <Int VIndex, typename TFunction, typename... TTuples>
    constexpr decltype(auto) ApplyAt(TFunction&& function, TTuples&&... tuples) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* FUNCTION ARGUMENTS                                                   */
    /************************************************************************/

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for lambdas and callable objects.
    template <typename TCallable>
    struct FunctionArguments : FunctionArguments<decltype(&Decay<TCallable>::operator())> {};

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for non-const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct FunctionArguments<TReturn(TCallable::*)(TArguments...)> : Templates::Alias<TypeList<TArguments...>> {};

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct FunctionArguments<TReturn(TCallable::*)(TArguments...) const> : Templates::Alias<TypeList<TArguments...>> {};

    /************************************************************************/
    /* LOCKSTEP                                                             */
    /************************************************************************/

    // ApplyLockstep.
    // ==============

    template <typename TFunction, typename... TTuples, Int... VIndexes>
    constexpr void ApplyLockstep(IntegerSequence<VIndexes...>, TFunction&& function, TTuples&&... tuples) noexcept
    {
        (ApplyAt<VIndexes>(Forward<TFunction>(function), Forward<TTuples>(tuples)...), ...);
    }
}

// ===========================================================================