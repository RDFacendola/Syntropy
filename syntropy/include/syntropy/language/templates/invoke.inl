
/// \file invoke.inl
///
/// \author Raffaele D. Facendola - Dec 2020.

#pragma once

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* INVOKE                                                               */
    /************************************************************************/

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
