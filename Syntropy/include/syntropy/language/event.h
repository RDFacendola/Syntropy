
/// \file event.h
/// \brief This header is part of the Syntropy language module. It contains definitions for events that can be subscribed to.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/core/smart_pointers.h"

#include "syntropy/language/listener.h"

namespace syntropy
{
    /************************************************************************/
    /* LISTENER HANDLER <TARGUMENTS>                                        */
    /************************************************************************/

    /// \brief Represents an event-listener relationship seen from an event's point of view. When the handler is destroyed the the listener is automatically unsubscribed.
    /// Event handlers are double-linked to keep alive multiple event-listener relationships at once.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename... TArguments>
    class ListenerHandler : public EventHandler
    {
    public:

        /// \brief Default constructor.
        ListenerHandler() = default;

        /// \brief No copy constructor.
        ListenerHandler(const ListenerHandler&) = delete;

        /// \brief No assignment operator.
        ListenerHandler& operator=(const ListenerHandler&) = delete;

        /// \brief Virtual destructor.
        virtual ~ListenerHandler();

        /// \brief Notify the observer and propagate to other listeners.
        void Notify(const TArguments&... arguments) const;

        /// \brief Link to other listeners.
        void LinkToListeners(ListenerHandler& rhs);

        /// \brief Unlink from other listeners.
        /// \brief Returns a pointer to this.
        ObserverPtr<ListenerHandler> UnlinkFromListeners();

        /// \brief Release the next listener along with every other listener that is reachable from this.
        ObserverPtr<ListenerHandler> ReleaseNextListeners();

    private:

        /// \brief Notify the handler.
        virtual void NotifyHandler(const TArguments&... arguments) const;

        /// \brief Pointer to the next listener.
        ObserverPtr<ListenerHandler> next_listener_{ nullptr };

        /// \brief Pointer to the previous listener.
        ObserverPtr<ListenerHandler> previous_listener_{ nullptr };

    };

    /************************************************************************/
    /* LISTENER DELEGATE <TDELEGATE, TARGUMENTS...>                         */
    /************************************************************************/

    /// \brief Represents a delegate called whenever an event is signaled to a listener.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TDelegate, typename... TArguments>
    class ListenerDelegate : public ListenerHandler<TArguments...>
    {
    public:

        /// \brief Create a new delegate bound to a callable object.
        ListenerDelegate(TDelegate&& delegate);

    private:

        /// \brief Notify the underlying delegate.
        virtual void NotifyHandler(const TArguments&... arguments) const override;

        /// \brief Underlying delegate to call whenever the listener is notified.
        TDelegate delegate_;

    };

    /************************************************************************/
    /* EVENT <TARGUMENTS...>                                                */
    /************************************************************************/

    /// \brief Represents a event bound to many listeners at once.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename... TArguments>
    class Event
    {
    public:

        /// \brief Default constructor.
        Event() = default;

        /// \brief Copy constructor.
        /// Listener handlers cannot be copied, therefore this method does nothing but making the owner copyable.
        Event(const Event& rhs);

        /// \brief Destructor, unsubscribe each listener bound to the event.
        ~Event();

        /// brief No assignment operator.
        Event& operator=(const Event&) = delete;

        /// \brief Subscribe to the event.
        /// \return Return a listener object used to keep the relationship alive.
        template <typename TDelegate>
        [[nodiscard]] Listener Subscribe(TDelegate&& delegate) const;

        /// \brief Notify subscribed listeners.
        void Notify(const TArguments&... arguments) const;

    private:

        /// \brief Dispatcher used to keep listener chain alive.
        /// \remarks Events have no visible state and can be subscribed to and notify events from constant contexts.
        mutable ListenerHandler<TArguments...> dispatcher_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename... TArguments, typename TDelegate>
    UniquePtr<ListenerDelegate<TDelegate, TArguments...>> MakeListenerDelegate(TDelegate&& delegate);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ListenerHandler<TArguments...>.

    template <typename... TArguments>
    inline ListenerHandler<TArguments...>::~ListenerHandler()
    {
        UnlinkFromListeners();
    }

    template <typename... TArguments>
    void ListenerHandler<TArguments...>::Notify(const TArguments&... arguments) const
    {
        NotifyHandler(arguments...);

        if (next_listener_)
        {
            next_listener_->Notify(arguments...);
        }
    }

    template <typename... TArguments>
    inline void ListenerHandler<TArguments...>::LinkToListeners(ListenerHandler& rhs)
    {
        SYNTROPY_ASSERT(!rhs.next_listener_ && !rhs.previous_listener_);

        rhs.next_listener_ = next_listener_;
        rhs.previous_listener_ = this;

        if (next_listener_)
        {
            next_listener_->previous_listener_ = &rhs;
        }

        next_listener_ = &rhs;
    }

    template <typename... TArguments>
    inline ObserverPtr<ListenerHandler<TArguments...>> ListenerHandler<TArguments...>::UnlinkFromListeners()
    {
        if (next_listener_)
        {
            next_listener_->previous_listener_ = previous_listener_;
        }

        if (previous_listener_)
        {
            previous_listener_->next_listener_ = next_listener_;
        }

        return this;
    }

    template <typename... TArguments>
    inline ObserverPtr<ListenerHandler<TArguments...>> ListenerHandler<TArguments...>::ReleaseNextListeners()
    {
        if (auto next_listener = next_listener_)
        {
            SYNTROPY_ASSERT(this == next_listener_->previous_listener_);

            next_listener_->previous_listener_ = nullptr;
            next_listener_ = nullptr;

            return next_listener;
        }

        return nullptr;
    }

    template <typename... TArguments>
    inline void ListenerHandler<TArguments...>::NotifyHandler(const TArguments&... arguments) const
    {
        // Do nothing.
    }

    // ListenerDelegate<TDelegate, TArguments...>.

    template <typename TDelegate, typename... TArguments>
    inline ListenerDelegate<TDelegate, TArguments...>::ListenerDelegate(TDelegate&& delegate)
        : delegate_(std::move(delegate))
    {

    }

    template <typename TDelegate, typename... TArguments>
    inline void ListenerDelegate<TDelegate, TArguments...>::NotifyHandler(const TArguments&... arguments) const
    {
        delegate_(arguments...);
    }

    // Event<TArguments...>.

    template <typename... TArguments>
    inline Event<TArguments...>::Event(const Event& rhs)
    {
        // Do nothing.
    }

    template <typename... TArguments>
    inline Event<TArguments...>::~Event()
    {
        for (auto listener = dispatcher_.ReleaseNextListeners(); listener; )
        {
            auto next_listener = listener->ReleaseNextListeners();

            listener->UnlinkFromEvents();       // Don't take ownership, causing the listener to be destroyed.

            listener = next_listener;
        }
    }

    template <typename... TArguments>
    template <typename TDelegate>
    inline Listener Event<TArguments...>::Subscribe(TDelegate&& delegate) const
    {
        auto listener_handler_delegate = MakeListenerDelegate<TArguments...>(std::move(delegate));

        dispatcher_.LinkToListeners(*listener_handler_delegate);

        return UniquePtr<EventHandler>(std::move(listener_handler_delegate));
    }

    template <typename... TArguments>
    inline void Event<TArguments...>::Notify(const TArguments&... arguments) const
    {
        dispatcher_.Notify(arguments...);
    }

    // Non-member functions.

    template <typename... TArguments, typename TDelegate>
    inline UniquePtr<ListenerDelegate<TDelegate, TArguments...>> MakeListenerDelegate(TDelegate&& delegate)
    {
        return MakeUnique<ListenerDelegate<TDelegate, TArguments...>>(std::move(delegate));
    }

}