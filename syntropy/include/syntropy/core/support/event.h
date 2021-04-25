
/// \file event.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for events that can be subscribed to.
///
/// \author Raffaele D. Facendola - 2017.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

#include "details/event.details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* LISTENER                                                             */
    /************************************************************************/

    /// \brief Represents a listener bound to any number of events.
    ///
    /// Event-listener relationships are automatically destroyed when either
    /// the event or the listener go out of scope.
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
        Mutable<Listener>
        operator=(Immutable<Listener> rhs) noexcept = default;

        /// \brief Default move-assignment operator.
        Mutable<Listener>
        operator=(Movable<Listener> rhs) noexcept /*= default*/;

        /// \brief Take ownership of all events bound to another listener.
        Mutable<Listener>
        operator+=(Movable<Listener> rhs) noexcept;

    private:

        /// \brief Create a new listener bound to an event chain.
        Listener(Movable<Details::EventChain> event_chain) noexcept;

        /// \brief Event chain.
        Details::EventChain events_;

    };

    /************************************************************************/
    /* EVENT                                                                */
    /************************************************************************/

    /// \brief Represents an event that can be notified to many listeners
    ///        at once.
    ///
    /// Listeners bound to an event are never propagated during copy but can
    //  be moved to and from.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename... TArguments>
    class Event
    {
    public:

        /// \brief Default constructor.
        Event() noexcept = default;

        /// \brief Default copy-constructor.
        Event(Immutable<Event> rhs) noexcept = default;

        /// \brief Default-move constructor.
        Event(Movable<Event> rhs) noexcept = default;

        /// \brief Destroy the event and unsubscribe all listeners.
        ~Event() noexcept = default;

        /// \brief Default copy-assignment operator.
        Mutable<Event>
        operator=(Immutable<Event> rhs) noexcept = default;

        /// \brief Default move-assignment operator.
        Mutable<Event>
        operator=(Movable<Event> rhs) noexcept /*= default*/;

        /// \brief Notify subscribed listeners.
        void
        Notify(Immutable<TArguments>... arguments) const noexcept;

        /// \brief Subscribe to the event and return a listener object used
        ///        to keep the relationship alive.
        template <typename TDelegate>
        [[nodiscard]] Listener
        Subscribe(Forwarding<TDelegate> delegate) const noexcept;

    private:

        /// \brief First listener in the listener chain.
        mutable Details::ListenerChain<TArguments...> listeners_;

    };
}

// ===========================================================================

#include "details/event.inl"

// ===========================================================================
