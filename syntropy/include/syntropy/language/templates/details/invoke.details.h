
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
