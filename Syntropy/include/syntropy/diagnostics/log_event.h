
/// \file log_event.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for log events.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/strings/context.h"
#include "syntropy/core/strings/string.h"
#include "syntropy/serialization/string_stream.h"
#include "syntropy/core/architecture/scope_guard.h"

#include "syntropy/diagnostics/severity.h"
#include "syntropy/diagnostics/stack_trace.h"

#include "syntropy/time/date.h"
#include "syntropy/time/time_of_day.h"

#include <thread>
#include <chrono>
#include <iostream>

namespace Syntropy
{
    /************************************************************************/
    /* LOG CLOCK                                                            */
    /************************************************************************/

    /// \brief Type of a log clock.
    using TLogClock = std::chrono::system_clock;

    /************************************************************************/
    /* LOG TIME                                                             */
    /************************************************************************/

    /// \brief Type of a log timestamp.
    using TLogTimestamp = TLogClock::time_point;

    /************************************************************************/
    /* LOG EVENT                                                            */
    /************************************************************************/

    /// \brief Represents a loggable event.
    /// \author Raffaele D. Facendola - November 2016.
    class LogEvent
    {
    public:

        /// \brief Create a new log event.
        LogEvent(Severity severity, Context context, StackTrace stack_trace, String message);

        /// \brief Default copy ctor.
        LogEvent(const LogEvent&) = default;

        /// \brief Default move ctor.
        LogEvent(LogEvent&&) = default;

        /// \brief Default assignment operator.
        LogEvent& operator=(const LogEvent&) = default;

        /// \brief Default destructor.
        ~LogEvent() = default;

        /// \brief Get the point in time the event was created.
        TLogTimestamp GetTimestamp() const;

        /// \brief Get the thread the event was generated from.
        std::thread::id GetThread() const;

        /// \brief Get the event severity.
        Severity GetSeverity() const;

        /// \brief Get the event context.
        const Context& GetContext() const;

        /// \brief Get the event stack trace.
        const StackTrace& GetStackTrace() const;

        /// \brief Get the event message.
        const String& GetMessage() const;

    private:

        /// \brief Point in time when the event was created.
        TLogTimestamp timestamp_{ TLogClock::now() };

        /// \brief Id of the thread that generated the event.
        std::thread::id thread_{ std::this_thread::get_id() };

        /// \brief Event severity.
        Severity severity_{ Severity::kInformative };

        /// \brief Context used to categorize the event.
        Context context_;

        /// \brief Stack trace that generated the event.
        StackTrace stack_trace_;

        /// \brief Event message.
        String message_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a log event from a variadic number of arguments composing the message.
    template <typename... TMessage>
    LogEvent MakeLogEvent(Severity severity, Context context, StackTrace stack_trace, TMessage&&... message);

    /// \brief Stream insertion for LogEvents.
    std::ostream& operator<<(std::ostream& out, const LogEvent& log_event);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LogEvent.

    inline LogEvent::LogEvent(Severity severity, Context context, StackTrace stack_trace, String message)
        : severity_(severity)
        , context_(Move(context))
        , stack_trace_(Move(stack_trace))
        , message_(Move(message))
    {

    }

    inline TLogTimestamp LogEvent::GetTimestamp() const
    {
        return timestamp_;
    }

    inline std::thread::id LogEvent::GetThread() const
    {
        return thread_;
    }

    inline Severity LogEvent::GetSeverity() const
    {
        return severity_;
    }

    inline const Context& LogEvent::GetContext() const
    {
        return context_;
    }

    inline const String& LogEvent::GetMessage() const
    {
        return message_;
    }

    inline const StackTrace& LogEvent::GetStackTrace() const
    {
        return stack_trace_;
    }

    // Non-member functions.

    template <typename... TMessage>
    inline LogEvent MakeLogEvent(Severity severity, Context context, StackTrace stack_trace, TMessage&&... message)
    {
        static thread_local auto message_builder = OStringStream{};

        auto builder_cleanup = MakeScopeGuard([]() { message_builder.str(""); });

        (message_builder << ... << Forward<TMessage>(message));

        return LogEvent(severity, Move(context), Move(stack_trace), message_builder.str());
    }

    inline std::ostream& operator<<(std::ostream& out, const LogEvent& log_event)
    {
        auto date = Date{ log_event.GetTimestamp() };
        auto time_of_day = TimeOfDay{ log_event.GetTimestamp() };

        out << "[" << date << " " << time_of_day << " @ " << log_event.GetThread() << "]";
        out << "[" << std::uppercase << log_event.GetSeverity() << std::nouppercase << " " << log_event.GetContext() << "]";
        out << ": " << log_event.GetMessage() << "\n" << log_event.GetStackTrace();

        return out;
    }

}
