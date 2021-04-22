
/// \file invoke.details.inl
///
/// \author Raffaele D. Facendola - April 2021.


// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* INVOKE                                                               */
    /************************************************************************/

    // InvocationSequence.
    // ===================

    template <typename TFunction, typename... TFunctions>
    InvocationSequence<TFunction, TFunctions...>
    ::InvocationSequence(TFunction function, TFunctions... functions) noexcept
        : TFunction{ Move(function) }
        , InvocationSequence<TFunctions...>{ Move(functions)... }
    {

    }

    template <typename TFunction, typename... TFunctions>
    template <typename... TArguments, typename UFunction>
    auto
    InvocationSequence<TFunction, TFunctions...>
    ::operator()(PriorityType, Forwarding<TArguments>... arguments)
    const noexcept -> decltype(UFunction::operator()(
        Forward<TArguments>(arguments)...))
    {
        return UFunction::operator()(Forward<TArguments>(arguments)...);
    }

    template <typename TFunction, typename... TFunctions>
    template <typename... TArguments>
    decltype(auto)
    InvocationSequence<TFunction, TFunctions...>
    ::operator()(typename InvocationSequence<TFunctions...>::PriorityType,
               Forwarding<TArguments>... arguments)
    const noexcept
    {
        return InvocationSequence<TFunctions...>::operator()(
            typename InvocationSequence<TFunctions...>::PriorityType{},
            Forward<TArguments>(arguments)...);
    }

    // InvocationSet.
    // ==============

    template <typename... TFunctions>
    InvocationSet<TFunctions...>
    ::InvocationSet(TFunctions... functions) noexcept
        : InvocationSequence<TFunctions...>{ Move(functions)... }
    {

    }

    template <typename... TFunctions>
    template <typename... TArguments>
    decltype(auto)
    InvocationSet<TFunctions...>
    ::operator()(Forwarding<TArguments>... arguments)
    const noexcept
    {
        return InvocationSequence<TFunctions...>::operator()(
            typename InvocationSequence<TFunctions...>::PriorityType{},
            Forward<TArguments>(arguments)...);
    }

}

// ===========================================================================
