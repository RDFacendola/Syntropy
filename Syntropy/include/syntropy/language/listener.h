
/// \file listener.h
/// \brief This header is part of the Syntropy language module. It contains definitions for listener that can be subscribed to events.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/smart_pointers.h"

namespace Syntropy
{
    /************************************************************************/
    /* EVENT HANDLER                                                        */
    /************************************************************************/

    /// \brief Represents an event-listener relationship seen from a listener's point of view. When the handler is destroyed the the listener is automatically unsubscribed.
    /// Event handlers are double-linked to keep alive multiple event-listener relationships at once.
    /// \author Raffaele D. Facendola - May 2020.
    class EventHandler
    {
    public:

        /// \brief Default constructor.
        EventHandler() noexcept = default;

        /// \brief No copy constructor.
        EventHandler(const EventHandler&) = delete;

        /// \brief No assignment operator.
        EventHandler& operator=(const EventHandler&) = delete;

        /// \brief Virtual destructor.
        virtual ~EventHandler();

        /// \brief Link to other events.
        void LinkToEvents(UniquePtr<EventHandler> rhs);

        /// \brief Unlink from other events and link the remaining ones.
        /// \return Returns a unique pointer to this. The returned value can be ignored, causing this event handler to be destroyed.
        UniquePtr<EventHandler> UnlinkFromEvents();

        /// \brief Release the next event along with every other event that is reachable from this.
        UniquePtr<EventHandler> ReleaseNextEvents();

    private:

        /// \brief Pointer to the next event.
        UniquePtr<EventHandler> next_event_{ nullptr };

        /// \brief Pointer to the previous event.
        Pointer<EventHandler> previous_event_{ nullptr };

    };

    /************************************************************************/
    /* LISTENER                                                             */
    /************************************************************************/

    /// \brief Represents a listener bound to many events at once.
    /// Whenever the listener goes out of scope, each event gets unsubscribed automatically.
    /// \author Raffaele D. Facendola - May 2020.
    class Listener
    {
    public:

        /// \brief Create an empty listener.
        Listener() = default;

        /// \brief Copy constructor.
        /// Event handlers cannot be copied, therefore this method does nothing but making the owner copyable.
        Listener(const Listener& rhs);

        /// \brief Create a listener bound to an event.
        Listener(UniquePtr<EventHandler> event_handler);

        /// \brief Default destructor.
        ~Listener() = default;

        /// \brief No assignment operator.
        /// Event handlers cannot be assigned, only reset.
        /// \see ::Reset().
        Listener& operator=(const Listener& rhs) = delete;

        /// \brief Take ownership of all events bound to another listener.
        Listener& operator+=(Listener&& rhs);

        /// \brief Reset the listener, unsubscribing all listeners at once.
        void Reset();

    private:

        /// \brief Dispatcher used to keep alive the event chain.
        EventHandler dispatcher_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // EventHandler.
    // =============

    inline EventHandler::~EventHandler()
    {
        UnlinkFromEvents();
    }

    // Listener.
    // =========

    inline Listener::Listener(UniquePtr<EventHandler> event_handler)
    {
        dispatcher_.LinkToEvents(std::move(event_handler));
    }

    inline Listener::Listener(const Listener& rhs)
    {
        // Do nothing: handlers cannot be safely copied.
    }

    inline Listener& Listener::operator+=(Listener&& rhs)
    {
        auto rhs_events = rhs.dispatcher_.ReleaseNextEvents();

        dispatcher_.LinkToEvents(std::move(rhs_events));

        return *this;
    }

    inline void Listener::Reset()
    {
        dispatcher_.ReleaseNextEvents();
    }

}