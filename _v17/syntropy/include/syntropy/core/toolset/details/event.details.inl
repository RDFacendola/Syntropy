
/// \file event.details.inl
///
/// \author Raffaele D. Facendola - 2017.

// ===========================================================================

#include "syntropy/language/support/swap.h"

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* EVENT HANDLER                                                        */
    /************************************************************************/

    inline EventHandler
    ::~EventHandler() noexcept
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

    inline void EventHandler
    ::Destroy() noexcept
    {
        if (previous_event_)
        {
            // Acquire ownership of self from the previous event and
            // self-destroy when leaving this scope.

            auto unique_this = Move(previous_event_->next_event_);
        }
    }

    inline RWUniquePtr<EventHandler> EventHandler
    ::CloneListener() noexcept
    {
        return {};
    }

    /************************************************************************/
    /* LISTENER HANDLER                                                     */
    /************************************************************************/

    template <typename... TArguments>
    inline ListenerHandler<TArguments...>
    ::~ListenerHandler() noexcept
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
    [[nodiscard]] RWUniquePtr<EventHandler> ListenerHandler<TArguments...>
    ::CloneListener() noexcept
    {
        auto clone = Clone();

        // Duplicate the listener inside the event.

        clone->previous_listener_ = this;
        clone->next_listener_ = Move(next_listener_);

        next_listener_ = clone.Get();

        return clone;
    }

    template <typename... TArguments>
    [[nodiscard]] inline RWUniquePtr<ListenerHandler<TArguments...>>
    ListenerHandler<TArguments...>
    ::Clone() const noexcept
    {
        return {};
    }

    template <typename... TArguments>
    inline void ListenerHandler<TArguments...>
    ::Notify(Immutable<TArguments>... arguments) const noexcept
    {
        // Base implementation: do nothing.
    }

    /************************************************************************/
    /* LISTENER HANDLER DELEGATE                                            */
    /************************************************************************/

    template <typename TDelegate, typename... TArguments>
    template <typename UDelegate>
    inline ListenerHandlerDelegate<TDelegate, TArguments...>
    ::ListenerHandlerDelegate(Forwarding<UDelegate> delegate)
        : delegate_(Forward<UDelegate>(delegate))
    {

    }

    template <typename TDelegate, typename... TArguments>
    [[nodiscard]] inline RWUniquePtr<ListenerHandler<TArguments...>>
    ListenerHandlerDelegate<TDelegate, TArguments...>
    ::Clone() const noexcept
    {
        return MakeRWUnique<ListenerHandlerDelegate<TDelegate,
                                                    TArguments...>>(delegate_);
    }

    template <typename TDelegate, typename... TArguments>
    inline void ListenerHandlerDelegate<TDelegate, TArguments...>
    ::Notify(Immutable<TArguments>... arguments) const noexcept
    {
        delegate_(arguments...);
    }

    /************************************************************************/
    /* EVENT CHAIN                                                          */
    /************************************************************************/

    inline EventChain
    ::EventChain(RWUniquePtr<EventHandler> event) noexcept
    {
        Link(events_, Move(event));
    }

    inline EventChain
    ::EventChain(Immutable<EventChain> rhs) noexcept
    {
        auto previous_event = &events_;

        auto first_event = rhs.events_.next_event_.Get();

        for (auto event = first_event; event; event = event->next_event_.Get())
        {
            previous_event->next_event_ = event->CloneListener();
            previous_event = previous_event->next_event_.Get();
        }
    }

    inline EventChain
    ::EventChain(Movable<EventChain> rhs) noexcept
    {
        Link(events_, Link(rhs.events_, nullptr));
    }

    inline EventChain
    ::~EventChain() noexcept
    {
        Link(events_, nullptr);
    }

    inline Mutable<EventChain> EventChain
    ::operator=(Immutable<EventChain> rhs) noexcept
    {
        if (PtrOf(rhs) != this)
        {
            auto clone = ToImmutable(rhs);

            Link(events_, Link(clone.events_, nullptr));
        }

        return *this;
    }

    inline Mutable<EventChain> EventChain
    ::operator=(Movable<EventChain> rhs) noexcept
    {
        Link(events_, Link(rhs.events_, nullptr));

        return *this;
    }

    inline Mutable<EventChain> EventChain
    ::operator+=(Movable<EventChain> rhs) noexcept
    {
        if (PtrOf(rhs) != this)
        {
            Link(GetTail(events_), Link(rhs.events_, nullptr));
        }

        return *this;
    }

    inline RWUniquePtr<EventHandler> EventChain
    ::Link(Mutable<EventHandler> lhs, RWUniquePtr<EventHandler> rhs) noexcept
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

    [[nodiscard]] inline Mutable<EventHandler> EventChain
    ::GetTail(Mutable<EventHandler> lhs) noexcept
    {
        auto tail = PtrOf(lhs);

        for (; tail->next_event_; tail = tail->next_event_.Get());

        return *tail;
    }

    /************************************************************************/
    /* LISTENER CHAIN                                                       */
    /************************************************************************/

    template <typename... TArguments>
    inline ListenerChain<TArguments...>
    ::ListenerChain(Immutable<ListenerChain> rhs) noexcept
    {
        // Do nothing: duplicating an event won't duplicate its listeners.
    }

    template <typename... TArguments>
    inline ListenerChain<TArguments...>
    ::ListenerChain(Movable<ListenerChain> rhs) noexcept
    {
        Link(listeners_, Link(rhs.listeners_, nullptr));
    }

    template <typename... TArguments>
    inline ListenerChain<TArguments...>
    ::~ListenerChain() noexcept
    {
        Destroy(Link(listeners_, nullptr));
    }

    template <typename... TArguments>
    inline Mutable<ListenerChain<TArguments...>> ListenerChain<TArguments...>
    ::operator=(Immutable<ListenerChain> rhs) noexcept
    {
        if (PtrOf(rhs) != this)
        {
            // Duplicating an event won't duplicate its listeners:
            // this method only destroys existing ones.

            Destroy(Link(listeners_, nullptr));
        }

        return *this;
    }

    template <typename... TArguments>
    inline Mutable<ListenerChain<TArguments...>> ListenerChain<TArguments...>
    ::operator=(Movable<ListenerChain> rhs) noexcept
    {
        Destroy(Link(listeners_, Link(rhs.listeners_, nullptr)));

        return *this;
    }

    template <typename... TArguments>
    inline void ListenerChain<TArguments...>
    ::operator()(Immutable<TArguments>... arguments) const noexcept
    {
        for (auto listener = listeners_.next_listener_;
             listener;
             listener = listener->next_listener_)
        {
            listener->Notify(arguments...);
        }
    }

    template <typename... TArguments>
    template <typename TDelegate>
    [[nodiscard]] inline EventChain ListenerChain<TArguments...>
    ::Emplace(Forwarding<TDelegate> delegate) noexcept
    {
        using UDelegate = ListenerHandlerDelegate<TDelegate, TArguments...>;

        auto listener = MakeRWUnique<UDelegate>(Forward<TDelegate>(delegate));

        Link(GetTail(listeners_), listener.Get());

        return EventChain{ Move(listener) };
    }

    template <typename... TArguments>
    inline RWPtr<ListenerHandler<TArguments...>> ListenerChain<TArguments...>
    ::Link(Mutable<ListenerHandler<TArguments...>> lhs,
           RWPtr<ListenerHandler<TArguments...>> rhs) noexcept
    {
        SYNTROPY_ASSERT((!rhs) || (!rhs->previous_listener_));

        // Unlink listeners past lhs.

        auto unlinked = Support::Exchange(lhs.next_listener_, nullptr);

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
    [[nodiscard]] inline Mutable<ListenerHandler<TArguments...>>
    ListenerChain<TArguments...>
    ::GetTail(Mutable<ListenerHandler<TArguments...>> lhs) noexcept
    {
        auto tail = PtrOf(lhs);

        for (; tail->next_listener_; tail = tail->next_listener_);

        return *tail;
    }

    template <typename... TArguments>
    inline void ListenerChain<TArguments...>
    ::Destroy(RWPtr<ListenerHandler<TArguments...>> lhs) noexcept
    {
        for (auto listener = lhs; listener;)
        {
            auto next_listener = listener->next_listener_;

            listener->Destroy();

            listener = next_listener;
        }
    }
}

// ===========================================================================
