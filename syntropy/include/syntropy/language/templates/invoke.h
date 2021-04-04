
/// \file invoke.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for functions and functors invocation.
///
/// \author Raffaele D. Facendola - Dec 2020.

#pragma once

#include "syntropy/language/foundation/definitions.h"

// ===========================================================================

#include "details/invoke.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* INVOKE                                                               */
    /************************************************************************/

    /// \brief Type list equal to the argument types a callable object
    ///        can be called with.
    ///
    /// If no matching element could be found, the program is ill-formed.
    template <typename TCallable>
    using ArgumentsOf = Details::ArgumentsOf<TCallable>;

    /// \brief Provides indexed access to function arguments' types.
    template <Int TIndex, typename TCallable >
    using ArgumentOf = ElementTypeOf<TIndex, ArgumentsOf<TCallable>>;

    /// \brief Type alias for the return type of a callable object invocation
    ///        with provided arguments.
    template <typename TCallable, typename... TArguments>
    using InvokeResultOf = Details::InvokeResultOf<TCallable, TArguments...>;

    /// \brief Invoke a callable object with provided arguments.
    template <typename TCallable, typename... TArguments>
    constexpr InvokeResultOf<TCallable, TArguments...>
    Invoke(Forwarding<TCallable> callable,
           Forwarding<TArguments>... arguments) noexcept;
}

// ===========================================================================

#include "details/invoke.inl"

// ===========================================================================
