
/// \file observable.h
/// \brief Contains definition for classes used to implement observable objects and event-related functionalities.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace syntropy
{
    /// \brief Base class for listeners.
    /// \author Raffaele D. Facendola - November 2017
    class Listener
    {
    public:

        /// \brief Default constructor.
        Listener() = default;

        /// \brief No copy constructor.
        Listener(const Listener&) = delete;

        /// \brief No move constructor.
        Listener(Listener&&) = delete;

        /// \brief No assignment operator.
        Listener& operator=(const Listener&) = delete;

        /// \brief Default virtual destructor.
        virtual ~Listener() = default;
    };

    /// \brief Represents a concrete listener subscribed to an observable object.
    /// \author Raffaele D. Facendola - June 2017
    template <typename... TArguments>
    class ListenerT : public Listener
    {
    public:

        /// \brief Create a new listener object.
        /// \param handler Handler routine for the event being notified.
        template <typename THandler>
        ListenerT(THandler handler)
            : handler_(std::move(handler))
        {

        }

        /// \brief Default destructor.
        /// Unsubscribes the listener from the current observable object (if any).
        virtual ~ListenerT() = default;

        /// \brief Notify the listener.
        /// \param arguments Argument being passed to the handler routine.
        template <typename... TEventArguments>
        void operator()(TEventArguments&&... arguments) const
        {
            handler_(std::forward<TEventArguments>(arguments)...);
        }

    private:

        std::function<void(TArguments...)> handler_;        ///< \brief Handler routine for the event.
    };

    /// \brief Interface for observable objects that can be subscribed to.
    /// An observable object can be subscribed by any number of listener.
    /// Whenever the object is destroyed, its listeners are unsubscribed automatically.
    /// \author Raffaele D. Facendola - June 2017
    template <typename... TArguments>
    class Observable
    {
    public:

        /// \brief Create a new observable object.
        Observable() = default;

        /// \brief No copy constructor.
        Observable(const Observable&) = delete;

        /// \brief No move constructor.
        Observable(Observable&&) = delete;

        /// \brief No assignment operator.
        Observable& operator=(const Observable&) = delete;

        /// \brief Virtual default destructor.
        virtual ~Observable() = default;

        /// \brief Subscribe a new listener to this observable object.
        /// \param handler Handler called whenever the observable object notifies.
        /// \return Returns a listener handle. The listener is subscribed as long as this handle is alive.
        template <typename THandler>
        std::shared_ptr<Listener> Subscribe(THandler handler)
        {
            auto listener = std::make_shared<ListenerT<TArguments...>>(std::move(handler));      // #TODO Use proper allocator.

            listeners_.emplace_back(listener);

            return listener;
        }

    protected:

        std::vector<std::weak_ptr<ListenerT<TArguments...>>> listeners_;         ///< \brief Listeners subscribed to this object.

    };

    /// \brief Observable event with notification capabilities.
    /// \author Raffaele D. Facendola - June 2017
    template <typename... TArguments>
    class Event : public Observable<TArguments...>
    {
    public:

        /// \brief Virtual default constructor.
        virtual ~Event() = default;

        /// \brief Trigger the event, notifying registered listeners.
        /// \param arguments Arguments passed to the listeners.
        template <typename... TEventArguments>
        void Notify(TEventArguments&&... arguments)
        {
            // Reverse iteration since the collection may change during the loop: new listeners can be added, invalid listeners are removed.

            auto& listeners = this->listeners_;

            for (auto index = listeners.size(); index > 0;)
            {
                --index;

                if (auto listener = listeners[index].lock())
                {
                    (*listener)(arguments...);
                }
                else
                {
                    std::swap(listeners.back(), listeners[index]);
                    listeners.pop_back();
                }
            }
        }
    };

}