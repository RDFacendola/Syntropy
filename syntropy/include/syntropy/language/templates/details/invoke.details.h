
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

    /// \brief Provides the a type alias Type, which is the argument types a
    ///        callable object can be called with.
    /// Partial specialization for lambdas and callable objects.
    template <typename TCallable>
    struct ArgumentsOfHelper
        : ArgumentsOfHelper<decltype(PtrOf(TCallable::operator()))>
    {

    };

    /// \brief Provides the a type alias Type, which is the argument types a
    ///        callable object can be called with.
    /// Partial specialization for non-const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct ArgumentsOfHelper<TReturn(TCallable::*)(TArguments...)>
    {
        using Type = TypeList<TArguments...>;
    };

    /// \brief Provides the a type alias Type, which is the argument types a
    ///        callable object can be called with.
    ///
    /// Partial specialization for const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct ArgumentsOfHelper<TReturn(TCallable::*)(TArguments...) const>
    {
        using Type = TypeList<TArguments...>;
    };

    /// \brief Type list equal to the argument types a callable object
    ///        can be called with.
    ///
    /// If no matching element could be found, the program is ill-formed.
    template <typename TCallable>
    using ArgumentsOf
        = typename ArgumentsOfHelper<TCallable>::Type;

    /// \brief Type alias for the return type of a callable object invocation.
    template <typename TCallable, typename... TArguments>
    using InvokeResultOf
        = std::invoke_result_t<TCallable, TArguments...>;

    template <typename TCallable, typename... TArguments>
    constexpr InvokeResultOf<TCallable, TArguments...>
    Invoke(Forwarding<TCallable> callable,
           Forwarding<TArguments>... arguments) noexcept
    {
        return std::invoke(Forward<TCallable>(callable),
                           Forward<TArguments>(arguments)...);
    }
}

// ===========================================================================
