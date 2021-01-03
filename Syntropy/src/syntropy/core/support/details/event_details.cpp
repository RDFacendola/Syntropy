#include "syntropy/core/support/details/event_details.h"

#include "syntropy/language/support/swap.h"

#include "syntropy/memory/unique_ptr.h"

#include "syntropy/diagnostics/assert.h"

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // EventChain.
    // ===========

    EventChain::EventChain(Immutable<EventChain> rhs) noexcept
    {
        SYNTROPY_ASSERT(false);
    }

    EventChain::EventChain(Movable<EventChain> rhs) noexcept
    {
        Swap(next_event_, rhs.next_event_);
        Swap(previous_event_, rhs.previous_event_);

        if (next_event_)
        {
            next_event_->previous_event_ = this;        // Fix-up the head to point correctly to this instance and not the one being moved.
        }
    }

    EventChain::~EventChain() noexcept
    {
        // Only this instance is destroyed, the rest is fixed-up.

        EventUnlink();
    }

    Mutable<EventChain> EventChain::operator=(Immutable<EventChain> rhs) noexcept
    {
        SYNTROPY_ASSERT(false);

        return *this;
    }
    
    Mutable<EventChain> EventChain::operator=(Movable<EventChain> rhs) noexcept
    {
        Swap(next_event_, rhs.next_event_);
        Swap(previous_event_, rhs.previous_event_);

        if (next_event_)
        {
            next_event_->previous_event_ = this;        // Fix-up the head to point correctly to this instance and not the one being moved.
        }

        return *this;
    }

    void EventChain::EventLink(Memory::UniquePtr<EventChain> head) noexcept
    {
        SYNTROPY_ASSERT(head);
        SYNTROPY_ASSERT(!head->previous_event_);

        auto tail = head.Get();

        for (; tail->next_event_; tail = tail->next_event_.Get());

        // Tail.

        if (next_event_)
        {
            next_event_->previous_event_ = tail;
        }

        tail->next_event_ = Move(next_event_);

        // Head.

        head->previous_event_ = this;
        next_event_ = Move(head);
    }

    Memory::UniquePtr<EventChain> EventChain::EventUnlink()
    {
        if (next_event_)
        {
            next_event_->previous_event_ = previous_event_;
        }

        if (previous_event_)
        {
            auto unique_this = Move(previous_event_->next_event_);

            previous_event_->next_event_ = Move(next_event_);

            return unique_this;
        }

        return {};
    }

    Memory::UniquePtr<EventChain> EventChain::EventRelease() noexcept
    {
        if (next_event_)
        {
            next_event_->previous_event_ = nullptr;
            
            return Exchange(next_event_, Memory::UniquePtr<EventChain>{});
        }

        return {};
    }

}

// ===========================================================================