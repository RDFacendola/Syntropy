
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
    ::operator=(Movable<Listener> rhs) noexcept
    {
        events_ = Move(rhs.events_);
        rhs.events_ = {};

        return *this;
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
    inline void
    Event<TArguments...>
    ::Notify(Immutable<TArguments>... arguments) const noexcept
    {
        listeners_(arguments...);
    }

    template <typename... TArguments>
    inline Mutable<Event<TArguments...>>
    Event<TArguments...>
    ::operator=(Movable<Event> rhs) noexcept
    {
        listeners_ = Move(rhs.listeners_);

        return *this;
    }

    template <typename... TArguments>
    template <typename TDelegate>
    [[nodiscard]] inline Listener
    Event<TArguments...>
    ::Subscribe(Forwarding<TDelegate> delegate) const noexcept
    {
        return listeners_.Emplace(Forward<TDelegate>(delegate));
    }

}

// ===========================================================================
