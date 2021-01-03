
/// \file event_details.h
/// \brief This header is part of the Syntropy core module. It contains implementation details of events and listeners.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/swap.h"

#include "syntropy/memory/unique_ptr.h"

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* EVENT CHAIN                                                          */
    /************************************************************************/

    /// \brief Represents the set of events a listener refers to.
    /// \author Raffaele D. Facendola - January 2021.
    class EventChain
    {
    public:

        /// \brief Default constructor.
        EventChain() noexcept = default;

        /// \brief Copy-constructor.
        EventChain(Immutable<EventChain> rhs) noexcept;
        
        /// \brief Default move-constructor.
        EventChain(Movable<EventChain> rhs) noexcept;

        /// \brief Virtual destructor.
        virtual ~EventChain() noexcept;

        /// \brief Copy-assignment operator.
        Mutable<EventChain> operator=(Immutable<EventChain> rhs) noexcept;
        
        /// \brief Default move-assignment operator.
        Mutable<EventChain> operator=(Movable<EventChain> rhs) noexcept;

        /// \brief Append an event chain to this.
        void EventLink(Memory::UniquePtr<EventChain> head) noexcept;
        
        /// \brief Unlink this event from the event chain and yield the ownership to the caller.
        Memory::UniquePtr<EventChain> EventUnlink();

        /// \brief Yield the ownership of the next events to the caller.
        Memory::UniquePtr<EventChain> EventRelease() noexcept;

    private:

        /// \brief Next event.
        Memory::UniquePtr<EventChain> next_event_;

        /// \brief Previous event.
        RWPtr<EventChain> previous_event_{ nullptr };

    };

    /************************************************************************/
    /* LISTENER CHAIN                                                       */
    /************************************************************************/

    /// \brief Represents the set of listeners subscribed to an event.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename... TArguments>
    class ListenerChain : public EventChain
    {
    public:

        /// \brief Default constructor.
        ListenerChain() noexcept = default;

        /// \brief Copy-constructor.
        ListenerChain(Immutable<ListenerChain> rhs) noexcept;

        /// \brief Default move-constructor.
        ListenerChain(Movable<ListenerChain> rhs) noexcept;

        /// \brief Virtual destructor.
        virtual ~ListenerChain() noexcept;

        /// \brief Copy-assignment operator.
        Mutable<ListenerChain> operator=(Immutable<ListenerChain> rhs) noexcept;

        /// \brief Default move-assignment operator.
        Mutable<ListenerChain> operator=(Movable<ListenerChain> rhs) noexcept;

        /// \brief Notify the listener chain starting from this element.
        void Notify(Immutable<TArguments>... arguments) const noexcept;

        /// \brief Append an listener chain to this.
        void ListenerLink(Mutable<ListenerChain> head) noexcept;

        /// \brief Reset the listener chain, causing all listeners in an event to be unsubscribed.
        void ListenerReset() noexcept;

    private:

        /// \brief Invoke the event-listener delegate.
        virtual void Invoke(Immutable<TArguments>... arguments) const noexcept;

        /// \brief Next listener.
        RWPtr<ListenerChain> next_listener_{ nullptr };

        /// \brief Previous listener.
        RWPtr<ListenerChain> previous_listener_{ nullptr };

    };

    /************************************************************************/
    /* LISTENER DELEGATE                                                    */
    /************************************************************************/

    /// \brief Represents a delegate called whenever an event is signaled to a listener.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TDelegate, typename... TArguments>
    class ListenerDelegate: public ListenerChain<TArguments...>
    {
    public:

        /// \brief Create a new delegate bound to a functor.
        ListenerDelegate(Forwarding<TDelegate> delegate);

        /// \brief Default virtual destructor.
        virtual ~ListenerDelegate() noexcept = default;

    private:

        virtual void Invoke(Immutable<TArguments>... arguments) const noexcept override;

        /// \brief Handler function.
        TDelegate delegate_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new event handler delegate.
    template <typename... TArguments, typename TDelegate>
    Memory::UniquePtr<ListenerDelegate<TDelegate, TArguments...>> MakeListenerDelegate(Forwarding<TDelegate> delegate);

}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ListenerChain.
    // ==============

    template <typename... TArguments>
    inline ListenerChain<TArguments...>::ListenerChain(Immutable<ListenerChain> rhs) noexcept
        : EventChain(rhs)
    {
        SYNTROPY_ASSERT(false);
    }

    template <typename... TArguments>
    inline ListenerChain<TArguments...>::ListenerChain(Movable<ListenerChain> rhs) noexcept
        : EventChain(rhs)
    {
        Swap(next_listener_, rhs.next_listener_);
        Swap(previous_listener_, rhs.previous_listener_);

        if (next_listener_)
        {
            next_listener_->previous_listener_ = this;          // Fix-up the head to point correctly to this instance and not the one being moved.
        }
    }

    template <typename... TArguments>
    inline ListenerChain<TArguments...>::~ListenerChain() noexcept
    {
        // Only this instance is destroyed, the rest is fixed-up.

        if (next_listener_)
        {
            next_listener_->previous_listener_ = previous_listener_;
        }

        if (previous_listener_)
        {
            previous_listener_->next_listener_ = next_listener_;
        }
    }

    template <typename... TArguments>
    inline Mutable<ListenerChain<TArguments...>> ListenerChain<TArguments...>::operator=(Immutable<ListenerChain> rhs) noexcept
    {
        EventChain::operator=(Move(rhs));

        SYNTROPY_ASSERT(false);

        return *this;
    }   
    
    template <typename... TArguments>
    inline Mutable<ListenerChain<TArguments...>> ListenerChain<TArguments...>::operator=(Movable<ListenerChain> rhs) noexcept
    {
        EventChain::operator=(Move(rhs));

        Swap(next_listener_, rhs.next_listener_);
        Swap(previous_listener_, rhs.previous_listener_);

        if (next_listener_)
        {
            next_listener_->previous_listener_ = this;      // Fix-up the head to point correctly to this instance and not the one being moved.
        }

        return *this;
    }

    template <typename... TArguments>
    inline void ListenerChain<TArguments...>::Notify(Immutable<TArguments>... arguments) const noexcept
    {
        for (auto listener = this; listener; listener = listener->next_listener_)
        {
            listener->Invoke(arguments...);
        }
    }

    template <typename... TArguments>
    inline void ListenerChain<TArguments...>::ListenerLink(Mutable<ListenerChain> head) noexcept
    {
        SYNTROPY_ASSERT(!head.previous_listener_);

        auto tail = PtrOf(head);

        for (; tail->next_listener_; tail = tail->next_listener_);

        // Tail.

        if (next_listener_)
        {
            next_listener_->previous_listener_ = tail;
        }

        tail->next_listener_ = next_listener_;

        // Head.

        head.previous_listener_ = this;
        next_listener_ = PtrOf(head);
    }

    template <typename... TArguments>
    void ListenerChain<TArguments...>::ListenerReset() noexcept
    {
        SYNTROPY_ASSERT(!previous_listener_);

        for (; next_listener_;)
        {
            next_listener_->EventUnlink();          // dtor will fix up chain linking, moving the next listener backwards.
        }

        next_listener_ = nullptr;
    }

    template <typename... TArguments>
    void ListenerChain<TArguments...>::Invoke(Immutable<TArguments>... arguments) const noexcept
    {
        // Do nothing.
    }

    // ListenerDelegate.
    // ======================

    template <typename TDelegate, typename... TArguments>
    inline ListenerDelegate<TDelegate, TArguments...>::ListenerDelegate(Forwarding<TDelegate> delegate)
        : delegate_(Forward<TDelegate>(delegate))
    {

    }

    template <typename TDelegate, typename... TArguments>
    inline void ListenerDelegate<TDelegate, TArguments...>::Invoke(Immutable<TArguments>... arguments) const noexcept
    {
        delegate_(arguments...);
    }

    // Non-member functions.
    // =====================

    template <typename... TArguments, typename TDelegate>
    Memory::UniquePtr<ListenerDelegate<TDelegate, TArguments...>> MakeListenerDelegate(Forwarding<TDelegate> delegate)
    {
        return Memory::MakeUnique<ListenerDelegate<TDelegate, TArguments...>>(Forward<TDelegate>(delegate));
    }

}
