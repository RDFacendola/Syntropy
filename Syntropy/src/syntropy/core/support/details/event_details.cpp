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

    }

    Mutable<EventChain> EventChain::operator=(Immutable<EventChain> rhs) noexcept
    {

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