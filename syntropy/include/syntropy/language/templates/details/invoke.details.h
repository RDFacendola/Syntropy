
/// \file invoke.details.h
///
/// \author Raffaele D. Facendola - December 2020.

// ===========================================================================

#pragma once

#include <type_traits>
#include <functional>

#include "syntropy/language/foundation/definitions.h"

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* INVOKE                                                               */
    /************************************************************************/

    // InvokeArgumentsOf.
    // ==================

    /// \brief Type of arguments needed to perform a functor invocation.
    template <typename TFunctor>
    struct InvokeArgumentsOfFunctorHelper
    {

    };

    /// \brief Specialization for non-const functor objects.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct InvokeArgumentsOfFunctorHelper<
        TReturn(TCallable::*)(TArguments...)>
    {
        using Type = TypeList<TArguments...>;
    };

    /// \brief Specialization for const functor objects.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct InvokeArgumentsOfFunctorHelper<
        TReturn(TCallable::*)(TArguments...) const>
    {
        using Type = TypeList<TArguments...>;
    };

    /// \brief Type of arguments needed to perform an invocation.
    template <typename TCallable>
    struct InvokeArgumentsOfHelper
        : InvokeArgumentsOfFunctorHelper<decltype(&TCallable::operator())>
    {

    };

    /// \brief Specialization for non-member functions.
    template <typename TReturn, typename... TArguments>
    struct InvokeArgumentsOfHelper<TReturn(*)(TArguments...)>
    {
        using Type = TypeList<TArguments...>;
    };

    /// \brief Specialization for member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct InvokeArgumentsOfHelper<
        TReturn(TCallable::*)(TArguments...)>
    {
        using Type = TypeList<TCallable, TArguments...>;
    };

    /// \brief Specialization for ref-qualified (mutable) member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct InvokeArgumentsOfHelper<
        TReturn(TCallable::*)(TArguments...) &>
    {
        using Type = TypeList<Mutable<TCallable>, TArguments...>;
    };

    /// \brief Specialization for const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct InvokeArgumentsOfHelper<
        TReturn(TCallable::*)(TArguments...) const>
    {
        using Type = TypeList<Immutable<TCallable>, TArguments...>;
    };

    /// \brief Specialization for ref-qualified (movable) member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct InvokeArgumentsOfHelper<
        TReturn(TCallable::*)(TArguments...) &&>
    {
        using Type = TypeList<Movable<TCallable>, TArguments...>;
    };

    /// \brief Specialization for const ref-qualified (immovable) member
    ///        functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct InvokeArgumentsOfHelper<
        TReturn(TCallable::*)(TArguments...) const &&>
    {
        using Type = TypeList<Immovable<TCallable>, TArguments...>;
    };

    /// \brief List of types needed to perform a callable object invocation.
    template <typename TCallable>
    using InvokeArgumentsOf
        = typename InvokeArgumentsOfHelper<TCallable>::Type;

    // InvokeResultOfHelper.
    // =====================

    /// \brief Result type of a callable object invocation.
    template <typename TCallable, typename... TArguments>
    struct InvokeResultOfHelper
        : Alias<std::invoke_result_t<TCallable, TArguments...>>
    {

    };

    /// \brief Specialization for non-member functions.
    template <typename TReturn,
              typename... TArguments,
              typename... UArguments>
    struct InvokeResultOfHelper<TReturn(*)(TArguments...), UArguments...>
        : Alias<TReturn>
    {

    };

    /// \brief Specialization for member functions.
    template <typename TCallable,
              typename TReturn,
              typename... TArguments,
              typename... UArguments>
    struct InvokeResultOfHelper<TReturn(TCallable::*)(TArguments...),
                                UArguments...>
        : Alias<TReturn>
    {

    };

    /// \brief Specialization for ref-qualified (mutable) member functions.
    template <typename TCallable,
              typename TReturn,
              typename... TArguments,
              typename... UArguments>
    struct InvokeResultOfHelper<TReturn(TCallable::*)(TArguments...) &,
                                UArguments...>
        : Alias<TReturn>
    {

    };

    /// \brief Specialization for const member functions.
    template <typename TCallable,
              typename TReturn,
              typename... TArguments,
              typename... UArguments>
    struct InvokeResultOfHelper<TReturn(TCallable::*)(TArguments...) const,
                                UArguments...>
        : Alias<TReturn>
    {

    };

    /// \brief Specialization for ref-qualified (movable) member functions.
    template <typename TCallable,
              typename TReturn,
              typename... TArguments,
              typename... UArguments>
    struct InvokeResultOfHelper<TReturn(TCallable::*)(TArguments...) &&,
                                UArguments...>
        : Alias<TReturn>
    {

    };

    /// \brief Specialization for ref-qualified (immovable) member functions.
    template <typename TCallable,
              typename TReturn,
              typename... TArguments,
              typename... UArguments>
    struct InvokeResultOfHelper<TReturn(TCallable::*)(TArguments...) const &&,
                                UArguments...>
        : Alias<TReturn>
    {

    };

    /// \brief Result type of a callable object invocation.
    template <typename TCallable, typename... TArguments>
    using InvokeResultOf
        = typename InvokeResultOfHelper<TCallable, TArguments...>::Type;

}

// ===========================================================================
