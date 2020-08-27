#include "syntropy/language/listener.h"

#include "syntropy/diagnostics/assert.h"

namespace Syntropy
{
    /************************************************************************/
    /* EVENT HANDLER                                                        */
    /************************************************************************/

    void EventHandler::LinkToEvents(UniquePtr<EventHandler> rhs)
    {
        if (rhs)
        {
            SYNTROPY_ASSERT(!rhs->previous_event_ && !rhs->next_event_);

            auto tail = this;

            for (; tail->next_event_; tail = tail->next_event_.get());

            rhs->previous_event_ = tail;

            tail->next_event_ = Move(rhs);
        }
    }

    UniquePtr<EventHandler> EventHandler::UnlinkFromEvents()
    {
        if (next_event_)
        {
            next_event_->previous_event_ = previous_event_;
        }

        if (previous_event_)
        {
            auto unique_this = Move(previous_event_->next_event_);     // When the pointer goes out of scope "this" gets destroyed.

            previous_event_->next_event_ = Move(next_event_);

            return unique_this;                                             // Return the ownership of this to the owner.
        }

        return {};                                                          // Will cause recursive destruction of successors.
    }

    UniquePtr<EventHandler> EventHandler::ReleaseNextEvents()
    {
        if (auto next_event = Move(next_event_))
        {
            SYNTROPY_ASSERT(this == next_event->previous_event_);

            next_event->previous_event_ = nullptr;
            next_event_ = nullptr;

            return next_event;
        }

        return nullptr;
    }
}