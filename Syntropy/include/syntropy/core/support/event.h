
/// \file event.h
/// \brief This header is part of the Syntropy core module. It contains definitions for events that can be subscribed to.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/memory/unique_ptr.h"

#include "syntropy/core/support/details/event_details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* LISTENER                                                             */
    /************************************************************************/

    /// \brief Represents a listener bound to any number of events.
    /// Event-listener relationships are automatically destroyed when either the event or the listener go out of scope.
    /// \author Raffaele D. Facendola - May 2020.
    class Listener
    {
        template <typename... UArguments>
        friend class Event;

    public:

        /// \brief Create an empty listener.
        Listener() noexcept = default;

        /// \brief Default copy-constructor.
        Listener(Immutable<Listener> rhs) noexcept = default;

        /// \brief Default move-constructor.
        Listener(Movable<Listener> rhs) noexcept = default;

        /// \brief Default destructor.
        ~Listener() noexcept = default;

        /// \brief Default copy-assignment operator.
        Mutable<Listener> operator=(Immutable<Listener> rhs) noexcept = default;

        /// \brief Default move-assignment operator.
        Mutable<Listener> operator=(Movable<Listener> rhs) noexcept = default;

        /// \brief Take ownership of all events bound to another listener.
        Mutable<Listener> operator+=(Movable<Listener> rhs) noexcept;

        /// \brief Unsubscribe from all subscribed events.
        void Reset() noexcept;

    private:

        /// \brief Create a new bound listener.
        Listener(Memory::UniquePtr<Details::EventChain> event) noexcept;

        /// \brief Event chain.
        Details::EventChain events_;

    };

    /************************************************************************/
    /* EVENT                                                                */
    /************************************************************************/

    /// \brief Represents an event that can be notified to many listeners at once.
    /// Listeners bound to an event are never propagated during copy but can be moved to and from.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename... TArguments>
    class Event
    {
    public:

        /// \brief Default constructor.
        Event() noexcept = default;

        /// \brief Copy constructor. Listeners are not propagated.
        Event(Immutable<Event> rhs) noexcept;

        /// \brief Default-move constructor.
        Event(Movable<Event> rhs) noexcept = default;

        /// \brief Destroy the event and unsubscribe all listeners.
        ~Event() noexcept = default;

        /// \brief Copy-assignment operator. Existing listeners are preserved, whereas new listeners are ignored.
        Mutable<Event> operator=(Immutable<Event> rhs) noexcept;

        /// \brief Default move-assignment operator.
        Mutable<Event> operator=(Movable<Event> rhs) noexcept = default;

        /// \brief Notify subscribed listeners.
        void Notify(Immutable<TArguments>... arguments) const noexcept;

        /// \brief Unsubscribe all listeners from this event.
        void Reset() noexcept;

        /// \brief Subscribe to the event and return a listener object used to keep the relationship alive.
        template <typename TDelegate>
        [[nodiscard]] Listener Subscribe(Forwarding<TDelegate> delegate) const noexcept;

    private:

        /// \brief First listener in the listener chain.
        mutable Details::ListenerChain<TArguments...> listeners_;

    };
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Listener.
    // =========

    inline Listener::Listener(Memory::UniquePtr<Details::EventChain> event) noexcept
    {
        events_.EventLink(Move(event));
    }

    inline Mutable<Listener> Listener::operator+=(Movable<Listener> rhs) noexcept
    {
        events_.EventLink(rhs.events_.EventRelease());

        return *this;
    }

    inline void Listener::Reset() noexcept
    {
        events_ = {};
    }

    // Event.
    // ======

    template <typename... TArguments>
    inline Event<TArguments...>::Event(Immutable<Event> rhs) noexcept
    {
        // Listeners are not propagated on copy.
    }

    template <typename... TArguments>
    inline Mutable<Event<TArguments...>> Event<TArguments...>::operator=(Immutable<Event> rhs) noexcept
    {
        // Listeners are not propagated on copy.

        return *this;
    }

    template <typename... TArguments>
    inline void Event<TArguments...>::Notify(Immutable<TArguments>... arguments) const noexcept
    {
        listeners_.Notify(arguments...);
    }

    template <typename... TArguments>
    inline void Event<TArguments...>::Reset() noexcept
    {
        listeners_ = {};
    }

    template <typename... TArguments>
    template <typename TDelegate>
    [[nodiscard]] inline Listener Event<TArguments...>::Subscribe(Forwarding<TDelegate> delegate) const noexcept
    {
        auto listener_delegate = Details::MakeListenerDelegate<TArguments...>(Forward<TDelegate>(delegate));

        listeners_.ListenerLink(*listener_delegate);

        return Memory::UniquePtr<Details::EventChain>(Move(listener_delegate));
    }


}

// ===========================================================================
