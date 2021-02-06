
/// \file event.inl
///
/// \author Raffaele D. Facendola - 2017

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* LISTENER                                                             */
    /************************************************************************/

    inline Listener
    ::Listener(Movable<Details::EventChain> event_chain) noexcept
    {
        events_ += Move(event_chain);
    }

    inline Mutable<Listener> Listener
    ::operator+=(Movable<Listener> rhs) noexcept
    {
        events_ += Move(rhs.events_);

        return *this;
    }

    /************************************************************************/
    /* EVENT                                                                */
    /************************************************************************/

    template <typename... TArguments>
    inline void Event<TArguments...>
    ::Notify(Immutable<TArguments>... arguments) const noexcept
    {
        listeners_(arguments...);
    }

    template <typename... TArguments>
    template <typename TDelegate>
    [[nodiscard]] inline Listener Event<TArguments...>
    ::Subscribe(Forwarding<TDelegate> delegate) const noexcept
    {
        return listeners_.Emplace(Forward<TDelegate>(delegate));
    }

}

// ===========================================================================
