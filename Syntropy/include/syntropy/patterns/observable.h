
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
    /************************************************************************/
    /* LISTENER                                                             */
    /************************************************************************/

    /// \brief Represents zero or more listeners subscribed to observable objects.
    /// \author Raffaele D. Facendola - November 2017
    class Listener
    {
    public:

        /// \brief Create an empty listener.
        Listener() = default;

        /// \brief Create a non-empty listener.
        Listener(const std::shared_ptr<void>& listener);

        /// \brief Default copy constructor.
        Listener(const Listener&) = default;

        /// \brief Default move constructor.
        Listener(Listener&&) = default;

        /// \brief No assignment operator.
        Listener& operator=(const Listener&) = delete;

        /// \brief Default destructor.
        ~Listener() = default;

        /// \brief Chain this listener with another one.
        Listener& operator+=(Listener&& rhs);

    private:

        /// \brief Opaque strong references to listeners.
        std::vector<std::shared_ptr<void>> listeners_;

    };

    /************************************************************************/
    /* OBSERVABLE                                                           */
    /************************************************************************/

    /// \brief Interface for observable events.
    /// \author Raffaele D. Facendola - June 2017.
    template <typename... TArguments>
    class Observable
    {
    public:

        /// \brief Virtual default destructor.
        virtual ~Observable() = default;

        /// \brief Subscribe a new listener.
        /// \return Returns a listener handle. The listener is subscribed as long as this handle is alive.
        template <typename TListener>
        Listener Subscribe(TListener&& listener) const;

    protected:

        /// \brief Add a new listener to the event.
        virtual Listener AddListener(std::function<void(TArguments...)> listener) const = 0;

    };

    /************************************************************************/
    /* EVENT                                                                */
    /************************************************************************/

    /// \brief Observable object that can be used to notify subscribed listeners.
    /// \author Raffaele D. Facendola - June 2017
    template <typename... TArguments>
    class Event : public Observable<TArguments...>
    {
    public:

        /// \brief Default constructor.
        Event() = default;

        /// \brief Empty copy constructor.
        Event(const Event&);

        /// \brief Move constructor.
        /// Moving an event moves existing listeners.
        Event(Event&&) = default;

        /// \brief No assignment operator.
        Event& operator=(const Event&) = delete;

        /// \brief Virtual default destructor.
        virtual ~Event() = default;

        /// \brief Notify all subscribed listeners.
        /// \param arguments Arguments passed to the listeners.
        template <typename... UArguments>
        void Notify(UArguments&&... arguments) const;

    protected:

        /// \brief Type of a listener function.
        using TListener = std::function<void(TArguments...)>;

        virtual Listener AddListener(std::function<void(TArguments...)> listener) const override;

        mutable std::vector<std::weak_ptr<TListener>> listeners_;           ///< \brief Listeners subscribed to this object. Mutable needed to subscribe to const events.
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Listener.

    inline Listener::Listener(const std::shared_ptr<void>& listener)
    {
        listeners_.emplace_back(listener);
    }

    inline Listener& Listener::operator+=(Listener&& rhs)
    {
        listeners_.reserve(listeners_.size() + rhs.listeners_.size());

        for (auto&& listener : rhs.listeners_)
        {
            listeners_.emplace_back(std::move(listener));
        }

        return *this;
    }

    // Observable<TArguments...>.

    template <typename... TArguments>
    template <typename TListener>
    inline Listener Observable<TArguments...>::Subscribe(TListener&& listener) const
    {
        return AddListener(std::forward<TListener>(listener));
    }

    // Event<TArguments...>.

    template <typename... TArguments>
    inline Event<TArguments...>::Event(const Event&)
    {
        // Copying an event won't preserve existing listeners.
    }

    template <typename... TArguments>
    template <typename... UArguments>
    void Event<TArguments...>::Notify(UArguments&&... arguments) const
    {
        // Reverse iteration since the collection may change during the loop: new listeners can be added, invalid listeners are removed.

        for (auto index = static_cast<std::int64_t>(listeners_.size()) - 1; index >= 0; --index)
        {
            if (auto listener = listeners_[index].lock())
            {
                (*listener)(arguments...);
            }
            else
            {
                std::swap(listeners_.back(), listeners_[index]);
                listeners_.pop_back();
            }
        }
    }

    template <typename... TArguments>
    Listener Event<TArguments...>::AddListener(std::function<void(TArguments...)> listener) const
    {
        auto shared_listener = std::make_shared<TListener>(std::move(listener));

        listeners_.emplace_back(shared_listener);       // Weak reference.

        return std::shared_ptr<void>(shared_listener);
    }

}
