
/// \file observable.h
/// \brief Contains definition for classes used to implement observable objects and event-related functionalities.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

namespace syntropy
{

    template <typename... TArguments>
    class Observable;
    
    template <typename... TArguments>
    class Listener;

    template <typename... TArguments>
    class Event;

    /// \brief Interface for observable objects that can be subscribed to.
    /// An observable object can be subscribed by any number of listener.
    /// Whenever the object is destroyed, its listeners are unsubscribed automatically.
    /// \author Raffaele D. Facendola - June 2017
    template <typename... TArguments>
    class Observable
    {
        template <typename... TArguments>
        friend class Listener;

    public:

        /// \brief Create a new observable object.
        Observable() = default;

        /// \brief No copy constructor.
        Observable(const Observable&) = delete;

        /// \brief No move constructor.
        Observable(Observable&&) = delete;

        /// \brief No assignment operator.
        Observable& operator=(const Observable&) = delete;

        /// \brief Virtual destructor.
        /// Unsubscribes current listeners.
        virtual ~Observable();

        /// \brief Subscribe a new listener.
        /// \param handler Handler called whenever the observable object notifies.
        /// \return Returns a pointer to a listener.
        template <typename THandler>
        std::unique_ptr<Listener<TArguments...>> Subscribe(THandler handler);

    protected:

        std::vector<Listener<TArguments...>*> listeners_;               ///< \brief Listeners subscribed to this object. Observer pointers.

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
        void Notify(TEventArguments&&... arguments);
    };

    /// \brief Represents a handler routine subscribed to an observable object.
    /// If the listener is destroyed, it is automatically unsubscribed from the observable object.
    /// \author Raffaele D. Facendola - June 2017
    template <typename... TArguments>
    class Listener
    {
        template <typename... TArguments>
        friend class Observable;

        template <typename... TArguments>
        friend class Event;

    public:

        /// \brief No copy constructor.
        Listener(const Listener&) = delete;

        /// \brief No move constructor.
        Listener(Listener&&) = delete;

        /// \brief No assignment operator.
        Listener& operator=(const Listener&) = delete;

        /// \brief Default destructor.
        /// Unsubscribes the listener from the current observable object (if any).
        ~Listener();

        /// \brief Unsubscribe the listener from current observable object.
        /// This method does nothing if the listener is already unsubscribed.
        void Unsubscribe();

        /// \brief Check whether this listener is subscribed to an observable object.
        /// \return Returns true if this listener is subscribed to an observable object, returns false otherwise.
        bool IsSubscribed() const;

    private:

        /// \brief Create a new listener subscribed to an observable object.
        /// \param observable The object being observed.
        /// \param handler Handler routine for the event being notified.
        Listener(Observable<TArguments...>& observable, std::function<void(TArguments...)> handler);

        Observable<TArguments...>* observable_;             ///< \brief Observable this listener refers to. nullptr If the listener was unsubscribed.

        std::function<void(TArguments...)> handler_;        ///< \brief Handler routine for the event.
    };

}

namespace syntropy
{
    /************************************************************************/
    /* OBSERVABLE                                                           */
    /************************************************************************/
    
    template <typename... TArguments>
    Observable<TArguments...>::~Observable()
    {
        // Unsubscribe all the listeners registered so far.

        while (!listeners_.empty())
        {
            listeners_.front()->Unsubscribe();
        }
    }

    template <typename... TArguments>
    template <typename THandler>
    std::unique_ptr<Listener<TArguments...>> Observable<TArguments...>::Subscribe(THandler handler)
    {
        // Create a new listener and store it inside the listeners list.

        auto listener = new Listener<TArguments...>(*this, std::move(handler));

        listeners_.emplace_back(listener);

        return std::unique_ptr<Listener<TArguments...>>(listener);
    }

    /************************************************************************/
    /* EVENT                                                                */
    /************************************************************************/

    template <typename... TArguments>
    template <typename... TEventArguments>
    void Event<TArguments...>::Notify(TEventArguments&&... arguments)
    {
        // Notify the listeners registered so far.

        for (auto&& listener : this->listeners_)
        {
            listener->handler_(arguments...);
        }
    }

    /************************************************************************/
    /* LISTENER                                                             */
    /************************************************************************/

    template <typename... TArguments>
    Listener<TArguments...>::Listener(Observable<TArguments...>& observable, std::function<void(TArguments...)> handler)
        : observable_(std::addressof(observable))
        , handler_(std::move(handler))
    {

    }

    template <typename... TArguments>
    Listener<TArguments...>::~Listener()
    {
        Unsubscribe();
    }

    template <typename... TArguments>
    void Listener<TArguments...>::Unsubscribe()
    {
        // Remove this listener from the observable object (if any).

        if (IsSubscribed())
        {
            observable_->listeners_.erase
            (
                std::remove
                (
                    std::begin(observable_->listeners_),
                    std::end(observable_->listeners_),
                    this
                ),
                std::end(observable_->listeners_)
            );

            observable_ = nullptr;
        }
    }

    template <typename... TArguments>
    bool Listener<TArguments...>::IsSubscribed() const
    {
        return observable_ != nullptr;
    }

}