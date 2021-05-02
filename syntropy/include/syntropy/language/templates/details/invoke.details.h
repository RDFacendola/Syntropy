
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

    // InvocationPriority.
    // ===================

    /// \brief A compile-time tag-type used to sort an overload function
    ///        set according to an explicit priority value.
    template <Int TPriority>
    struct InvocationPriority
        : InvocationPriority<TPriority - 1>{};

    /// \brief Lowest priority tag type.
    template <>
    struct InvocationPriority<0> {};

    // InvocationSequence.
    // ===================

    /// \brief An ordered sequence of functions.
    template <typename...>
    struct InvocationSequence
    {
        /// \brief Lowest priority.
        using PriorityType = InvocationPriority<0>;
    };

    /// \brief Partial specialization for non-empty sets.
    template <typename TFunction, typename... TFunctions>
    struct InvocationSequence<TFunction, TFunctions...>
        : TFunction
        , InvocationSequence<TFunctions...>
    {
        /// \brief Priority of the current function.
        using PriorityType = InvocationPriority<sizeof...(TFunctions) + 1>;

        /// \brief Create an invocation set.
        InvocationSequence(TFunction function, TFunctions... functions)
        noexcept;

        /// \brief Forwards the call to the first function in the sequence.
        ///
        /// \remarks This overload is always preferred to the other, due to its
        ///          higher priority.
        template <typename... TArguments, typename UFunction = TFunction>
        auto
        operator()(PriorityType, Forwarding<TArguments>... arguments)
        const noexcept -> decltype(UFunction::operator()(
            Forward<TArguments>(arguments)...));

        /// \brief Forwards the call to the next function in the sequence.
        ///
        /// \remarks This overload is selected if the current function, called
        ///          with provided arguments, results in an ill-formed program.
        template <typename... TArguments>
        decltype(auto)
        operator()(typename InvocationSequence<TFunctions...>::PriorityType,
                   Forwarding<TArguments>... arguments)
        const noexcept;
    };

    // InvocationSet.
    // ==============

    /// \brief Functor used to forward a function call to an ordered sequence
    ///        of functions.
    ///
    /// \see This class automates what's described in this amazing post:
    ///      https://wandbox.org/permlink/AB9uQxO2MymNDDtt
    template <typename... TFunctions>
    struct InvocationSet : InvocationSequence<TFunctions...>
    {
        /// \brief Create an invocation set.
        InvocationSet(TFunctions... functions) noexcept;

        /// \brief Forward the arguments to the first function in the set that
        ///        would result in a well-formed program.
        template <typename... TArguments>
        decltype(auto)
        operator()(Forwarding<TArguments>... arguments)
        const noexcept;
    };

}

// ===========================================================================

#include "invoke.details.inl"

// ===========================================================================
