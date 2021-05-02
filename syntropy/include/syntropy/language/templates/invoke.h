
/// \file invoke.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for functions and functors invocation.
///
/// \author Raffaele D. Facendola - December 2020.

// ===========================================================================

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

    /// \brief Invoke a callable object with provided arguments.
    ///
    /// \usage Invoke(functor, arguments...); // Calls functor(arguments...)
    ///        Invoke(non-member-function, arguments...);
    ///        Invoke(member-function, instance, arguments...);
    template <typename TCallable, typename... TArguments>
    constexpr InvokeResultOf<TCallable, TArguments...>
    Invoke(Forwarding<TCallable> callable,
           Forwarding<TArguments>... arguments) noexcept;

    /// \brief Create a functor to forward an invocation on any of
    ///        the provided functions.
    ///
    /// This method selects the first, well-formed, callable object in the
    /// provided list. If none of the callable object result in a well-formed
    /// invocation, the program is ill-formed.
    ///
    /// \usage auto invoke_any = InvokeAny([](auto&& ){ ... },
    ///                                    [](auto&& ){ ... },
    ///                                    [](auto&& ){ ... });
    ///       invoke_any(arguments...);
    template <typename... TFunctions>
    [[nodiscard]] constexpr auto
    InvokeAny(TFunctions... function) noexcept;
}

// ===========================================================================

#include "details/invoke.inl"

// ===========================================================================
