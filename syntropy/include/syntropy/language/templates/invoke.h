
/// \file invoke.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for functions and functors invocation.
///
/// \author Raffaele D. Facendola - Dec 2020.

#pragma once

#include "syntropy/language/foundation/definitions.h"

#include "syntropy/language/templates/details/invoke_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* INVOKE                                                               */
    /************************************************************************/

    /// \brief Type alias equal to the argument types a callable object
    ///        can be called with.
    /// If no matching element could be found, the program is ill-formed.
    template <typename TCallable>
    using FunctionArguments = Details::FunctionArguments<TCallable>;

    /// \brief Provides indexed access to function arguments' types.
    template <Int VIndex, typename TCallable >
    using FunctionArgumentsElement
        = TypeListElement<VIndex, FunctionArguments<TCallable>>;

    /// \brief Type alias for the return type of a callable object invocation
    ///        with provided arguments.
    template <typename TCallable, typename... TArguments>
    using InvokeResult = Details::InvokeResult<TCallable, TArguments...>;

    /// \brief Invoke a callable object with provided arguments.
    template <typename TCallable, typename... TArguments>
    constexpr InvokeResult<TCallable, TArguments...>
    Invoke(Forwarding<TCallable> callable,
           Forwarding<TArguments>... arguments) noexcept;
}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Invoke.
    // =======

    template <typename TCallable, typename... TArguments>
    constexpr InvokeResult<TCallable, TArguments...>
    Invoke(Forwarding<TCallable> callable,
           Forwarding<TArguments>... arguments) noexcept
    {
        return Details::Invoke(Forward<TCallable>(callable),
                               Forward<TArguments>(arguments)...);
    }
}

// ===========================================================================
