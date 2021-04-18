
/// \file invoke.inl
///
/// \author Raffaele D. Facendola - December 2020.

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* INVOKE                                                               */
    /************************************************************************/

    template <typename TCallable, typename... TArguments>
    constexpr InvokeResultOf<TCallable, TArguments...>
    Invoke(Forwarding<TCallable> callable,
           Forwarding<TArguments>... arguments) noexcept
    {
        return Details::Invoke(Forward<TCallable>(callable),
                               Forward<TArguments>(arguments)...);
    }
}

// ===========================================================================
