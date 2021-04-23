
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

    /// \brief List of types needed to perform a callable object invocation.
    ///
    /// \remarks For member-functions, the first argument in the list is the
    ///          (possibly) qualified type of the class the function belongs
    ///          to. For functor objects the calling instance is implicit,
    ///          therefore its type is not present in the argument list.
    template <typename TCallable>
    using InvokeArgumentsOf
        = Details::InvokeArgumentsOf<TCallable>;

    /// \brief Provides indexed access to an invocation argument type.
    template <Int TIndex, typename TCallable >
    using InvokeArgumentOf
        = ElementTypeOf<TIndex, InvokeArgumentsOf<TCallable>>;

    /// \brief Result type of a callable object invocation.
    ///
    /// \remarks In case of member and non-member functions the argument list
    ///          is not necessary to determine the correct return type.
    ///          For functors, however, there might be several overloads,
    ///          therefore the argument types must be specified.
    template <typename TCallable, typename... TArguments>
    using InvokeResultOf
        = Details::InvokeResultOf<TCallable, TArguments...>;

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
