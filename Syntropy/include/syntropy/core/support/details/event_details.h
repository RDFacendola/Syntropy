
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
    class EventHandler
    {
        friend class EventChain;

    public:

        /// \brief Default constructor.
        EventHandler() noexcept = default;

        /// \brief No copy-constructor.
        EventHandler(Immutable<EventHandler> rhs) noexcept = delete;
        
        /// \brief No move-constructor.
        EventHandler(Movable<EventHandler> rhs) noexcept = delete;

        /// \brief Virtual destructor.
        virtual ~EventHandler() noexcept;

        /// \brief No copy-assignment operator.
        Mutable<EventHandler> operator=(Immutable<EventHandler> rhs) noexcept = delete;
        
        /// \brief No move-assignment operator.
        Mutable<EventHandler> operator=(Movable<EventHandler> rhs) noexcept = delete;

        /// \brief Destroy this handler.
        void Destroy() noexcept;

    private:

        /// \brief Next event.
        Memory::UniquePtr<EventHandler> next_event_;

        /// \brief Previous event.
        RWPtr<EventHandler> previous_event_{ nullptr };

    };

    /************************************************************************/
    /* LISTENER CHAIN                                                       */
    /************************************************************************/

    /// \brief Represents the set of listeners subscribed to an event.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename... TArguments>
    class ListenerHandler : public EventHandler
    {
        template <typename... TArguments>
        friend class ListenerChain;

    public:

        /// \brief Default constructor.
        ListenerHandler() noexcept = default;

        /// \brief No copy-constructor.
        ListenerHandler(Immutable<ListenerHandler> rhs) noexcept = delete;

        /// \brief No move-constructor.
        ListenerHandler(Movable<ListenerHandler> rhs) noexcept = delete;

        /// \brief Virtual destructor.
        virtual ~ListenerHandler() noexcept;

        /// \brief No copy-assignment operator.
        Mutable<ListenerHandler> operator=(Immutable<ListenerHandler> rhs) noexcept = delete;

        /// \brief No move-assignment operator.
        Mutable<ListenerHandler> operator=(Movable<ListenerHandler> rhs) noexcept = delete;

    private:

        /// \brief Notify the delegate.
        virtual void Notify(Immutable<TArguments>... arguments) const noexcept;

        /// \brief Next listener.
        RWPtr<ListenerHandler> next_listener_{ nullptr };

        /// \brief Previous listener.
        RWPtr<ListenerHandler> previous_listener_{ nullptr };

    };

    /************************************************************************/
    /* LISTENER DELEGATE                                                    */
    /************************************************************************/

    /// \brief Represents a delegate called whenever an event is signaled to a listener.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TDelegate, typename... TArguments>
    class ListenerHandlerDelegate: public ListenerHandler<TArguments...>
    {
    public:

        /// \brief Create a new delegate bound to a functor.
        ListenerHandlerDelegate(Forwarding<TDelegate> delegate);

        /// \brief No copy-constructor.
        ListenerHandlerDelegate(Immutable<ListenerHandlerDelegate> rhs) noexcept = delete;

        /// \brief No move-constructor.
        ListenerHandlerDelegate(Movable<ListenerHandlerDelegate> rhs) noexcept = delete;

        /// \brief Default virtual destructor.
        virtual ~ListenerHandlerDelegate() noexcept = default;

        /// \brief No copy-assignment operator.
        Mutable<ListenerHandlerDelegate> operator=(Immutable<ListenerHandlerDelegate> rhs) noexcept = delete;

        /// \brief No move-assignment operator.
        Mutable<ListenerHandlerDelegate> operator=(Movable<ListenerHandlerDelegate> rhs) noexcept = delete;

    private:

        virtual void Notify(Immutable<TArguments>... arguments) const noexcept override;

        /// \brief Handler function.
        TDelegate delegate_;

    };

    /************************************************************************/
    /* EVENT CHAIN                                                          */
    /************************************************************************/

    /// \brief List of events a listener is subscribed to.
    /// \author Raffaele D. Facendola - January 2021.
    class EventChain
    {
    public:

        /// \brief Default constructor.
        EventChain() noexcept = default;

        /// \brief Create a new event chain with a single event.
        EventChain(Memory::UniquePtr<EventHandler> event) noexcept;

        /// \brief Copy-constructor.
        EventChain(Immutable<EventChain> rhs) noexcept;

        /// \brief Move-constructor.
        EventChain(Movable<EventChain> rhs) noexcept;

        /// \brief Destructor.
        ~EventChain() noexcept;

        /// \brief Copy-assignment operator.
        Mutable<EventChain> operator=(Immutable<EventChain> rhs) noexcept;

        /// \brief Move-assignment operator.
        Mutable<EventChain> operator=(Movable<EventChain> rhs) noexcept;

        /// \brief Append an event chain to this one's, transferring its ownership.
        Mutable<EventChain> operator+=(Movable<EventChain> rhs) noexcept;

    private:

        /// \brief Link two event chains together and return the event chain that was unlinked as a result.
        /// The behavior of this method is undefined if rhs is linked to an existing event chain.
        static Memory::UniquePtr<EventHandler> Link(Mutable<EventHandler> lhs, Memory::UniquePtr<EventHandler> rhs) noexcept;

        /// \brief Access the last event accessible from lhs.
        static Mutable<EventHandler> GetTail(Mutable<EventHandler> lhs) noexcept;

        /// \brief Dummy event linked to each event a listener is subscribed to.
        EventHandler events_;

    };

    /************************************************************************/
    /* LISTENER CHAIN                                                       */
    /************************************************************************/

    /// \brief List of listeners subscribed to an event.
    /// \author Raffaele D. Facendola - January 2021.
    template <typename... TArguments>
    class ListenerChain
    {
    public:

        /// \brief Default constructor.
        ListenerChain() noexcept = default;

        /// \brief Copy-constructor.
        ListenerChain(Immutable<ListenerChain> rhs) noexcept;

        /// \brief Move-constructor.
        ListenerChain(Movable<ListenerChain> rhs) noexcept;

        /// \brief Destructor.
        ~ListenerChain() noexcept;

        /// \brief Copy-assignment operator.
        Mutable<ListenerChain> operator=(Immutable<ListenerChain> rhs) noexcept;

        /// \brief Move-assignment operator.
        Mutable<ListenerChain> operator=(Movable<ListenerChain> rhs) noexcept;

        /// \brief Notify the listener chain starting from this element.
        void operator()(Immutable<TArguments>... arguments) const noexcept;

        /// \brief Emplace a new listener to the chain and return the event chain associated to it.
        template <typename TDelegate>
        EventChain Emplace(Forwarding<TDelegate> delegate) noexcept;

    private:

        /// \brief Link two listener chains together and return the listener chain that was unlinked as a result.
        /// The behavior of this method is undefined if rhs is linked to an existing listener chain.
        static RWPtr<ListenerHandler<TArguments...>> Link(Mutable<ListenerHandler<TArguments...>> lhs, RWPtr<ListenerHandler<TArguments...>> rhs) noexcept;

        /// \brief Access the last listener accessible from lhs.
        static Mutable<ListenerHandler<TArguments...>> GetTail(Mutable<ListenerHandler<TArguments...>> lhs) noexcept;

        /// \brief Destroy all listeners accessible from lhs.
        static void Destroy(RWPtr<ListenerHandler<TArguments...>> lhs) noexcept;

        /// \brief Dummy listener linked to each listener subscribed to an event.
        ListenerHandler<TArguments...> listeners_;

    };

}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // EventHandler.
    // =============

    inline EventHandler::~EventHandler() noexcept
    {
        // Fix the event chain up before destruction.
        
        if (next_event_)
        {
            next_event_->previous_event_ = previous_event_;
        }

        if (previous_event_)
        {
            previous_event_->next_event_ = Move(next_event_);
        }
    }

    inline void EventHandler::Destroy() noexcept
    {
        if (previous_event_)
        {
            auto unique_this = Move(previous_event_->next_event_);

            // previous_event_->next_event_ = nullptr;         // Destructor will make this pointing to the next event (if any).
        }
    }

    // ListenerHandler.
    // ==============

    template <typename... TArguments>
    inline ListenerHandler<TArguments...>::~ListenerHandler() noexcept
    {
        // Fix the listener chain up before destruction.

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
    void ListenerHandler<TArguments...>::Notify(Immutable<TArguments>... arguments) const noexcept
    {
        // Base implementation: do nothing.
    }

    // ListenerHandlerDelegate.
    // ========================

    template <typename TDelegate, typename... TArguments>
    inline ListenerHandlerDelegate<TDelegate, TArguments...>::ListenerHandlerDelegate(Forwarding<TDelegate> delegate)
        : delegate_(Forward<TDelegate>(delegate))
    {

    }

    template <typename TDelegate, typename... TArguments>
    inline void ListenerHandlerDelegate<TDelegate, TArguments...>::Notify(Immutable<TArguments>... arguments) const noexcept
    {
        delegate_(arguments...);
    }

    // EventChain.
    // ===========

    inline EventChain::EventChain(Memory::UniquePtr<EventHandler> event) noexcept
    {
        Link(events_, Move(event));
    }

    inline EventChain::EventChain(Immutable<EventChain> rhs) noexcept
    {
        // #TODO Duplicate rhs chain.
    }

    inline EventChain::EventChain(Movable<EventChain> rhs) noexcept
    {
        Link(events_, Link(rhs.events_, nullptr));
    }

    inline EventChain::~EventChain() noexcept
    {
        Link(events_, nullptr);
    }

    inline Mutable<EventChain> EventChain::operator=(Immutable<EventChain> rhs) noexcept
    {
        if (PtrOf(rhs) != this)
        {
            // #TODO Duplicate rhs chain and wipe lhs chain.
        }

        return *this;
    }

    inline Mutable<EventChain> EventChain::operator=(Movable<EventChain> rhs) noexcept
    {
        Link(events_, Link(rhs.events_, nullptr));

        return *this;
    }

    inline Mutable<EventChain> EventChain::operator+=(Movable<EventChain> rhs) noexcept
    {
        if (PtrOf(rhs) != this)
        {
            Link(GetTail(events_), Link(rhs.events_, nullptr));
        }

        return *this;
    }

    inline Memory::UniquePtr<EventHandler> EventChain::Link(Mutable<EventHandler> lhs, Memory::UniquePtr<EventHandler> rhs) noexcept
    {
        SYNTROPY_ASSERT((!rhs) || (!rhs->previous_event_));

        // Unlink events past lhs.

        auto unlinked = Move(lhs.next_event_);

        if (unlinked)
        {
            unlinked->previous_event_ = nullptr;
        }

        // Append rhs to lhs.

        if (rhs)
        {
            rhs->previous_event_ = PtrOf(lhs);
        }

        lhs.next_event_ = Move(rhs);

        // Yield unlinked events' ownership to the caller.

        return unlinked;
    }

    inline Mutable<EventHandler> EventChain::GetTail(Mutable<EventHandler> lhs) noexcept
    {
        auto tail = PtrOf(lhs);

        for (; tail->next_event_; tail = tail->next_event_.Get());

        return *tail;
    }

    // ListenerChain.
    // ==============

    template <typename... TArguments>
    inline ListenerChain<TArguments...>::ListenerChain(Immutable<ListenerChain> rhs) noexcept
    {
        // Do nothing: duplicating an event won't duplicate its listeners.
    }

    template <typename... TArguments>
    inline ListenerChain<TArguments...>::ListenerChain(Movable<ListenerChain> rhs) noexcept
    {
        Link(listeners_, Link(rhs.listeners_, nullptr));
    }

    template <typename... TArguments>
    inline ListenerChain<TArguments...>::~ListenerChain() noexcept
    {
        Destroy(Link(listeners_, nullptr));
    }

    template <typename... TArguments>
    inline Mutable<ListenerChain<TArguments...>> ListenerChain<TArguments...>::operator=(Immutable<ListenerChain> rhs) noexcept
    {
        if (PtrOf(rhs) != this)
        {
            // Duplicating an event won't duplicate its listeners: this method only destroys existing ones.

            Destroy(Link(listeners_, nullptr));
        }

        return *this;
    }

    template <typename... TArguments>
    inline Mutable<ListenerChain<TArguments...>> ListenerChain<TArguments...>::operator=(Movable<ListenerChain> rhs) noexcept
    {
        Destroy(Link(listeners_, Link(rhs.listeners_, nullptr)));

        return *this;
    }

    template <typename... TArguments>
    inline void ListenerChain<TArguments...>::operator()(Immutable<TArguments>... arguments) const noexcept
    {
        for (auto listener = listeners_.next_listener_; listener; listener = listener->next_listener_)
        {
            listener->Notify(arguments...);
        }
    }

    template <typename... TArguments>
    template <typename TDelegate>
    inline EventChain ListenerChain<TArguments...>::Emplace(Forwarding<TDelegate> delegate) noexcept
    {
        auto listener = Memory::MakeUnique<ListenerHandlerDelegate<TDelegate, TArguments...>>(Forward<TDelegate>(delegate));

        Link(GetTail(listeners_), listener.Get());

        return EventChain{ Move(listener) };
    }

    template <typename... TArguments>
    inline RWPtr<ListenerHandler<TArguments...>> ListenerChain<TArguments...>::Link(Mutable<ListenerHandler<TArguments...>> lhs, RWPtr<ListenerHandler<TArguments...>> rhs) noexcept
    {
        SYNTROPY_ASSERT((!rhs) || (!rhs->previous_listener_));

        // Unlink listeners past lhs.

        auto unlinked = Exchange(lhs.next_listener_, nullptr);

        if (unlinked)
        {
            unlinked->previous_listener_ = nullptr;
        }

        // Append rhs to lhs.

        if (rhs)
        {
            rhs->previous_listener_ = PtrOf(lhs);
        }

        lhs.next_listener_ = rhs;

        // Return unlinked listeners.

        return unlinked;
    }

    template <typename... TArguments>
    inline Mutable<ListenerHandler<TArguments...>> ListenerChain<TArguments...>::GetTail(Mutable<ListenerHandler<TArguments...>> lhs) noexcept
    {
        auto tail = PtrOf(lhs);

        for (; tail->next_listener_; tail = tail->next_listener_);

        return *tail;
    }

    template <typename... TArguments>
    inline void ListenerChain<TArguments...>::Destroy(RWPtr<ListenerHandler<TArguments...>> lhs) noexcept
    {
        for (auto listener = lhs; listener;)
        {
            auto next_listener = listener->next_listener_;

            listener->Destroy();

            listener = next_listener;
        }
    }
}
