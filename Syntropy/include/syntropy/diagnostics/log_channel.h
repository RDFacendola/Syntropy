
/// \file log_channel.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for log channels.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <algorithm>

#include "syntropy/diagnostics/severity.h"
#include "syntropy/diagnostics/verbosity.h"
#include "syntropy/diagnostics/log_event.h"

#include "syntropy/containers/vector.h"

namespace syntropy
{
    /************************************************************************/
    /* LOG CHANNEL                                                          */
    /************************************************************************/

    /// \brief Base interface for log channels events can be sent to.
    /// \author Raffaele D. Facendola - November 2016.
    class LogChannel
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~LogChannel() = default;

        /// \brief Send a log event to the channel.
        virtual void Send(const LogEvent& log_event) = 0;

        /// \brief Send a log event to the channel.
        virtual void Send(LogEvent&& log_event) = 0;

        /// \brief Flush cached states.
        virtual void Flush() = 0;

    };

    /************************************************************************/
    /* LOG CHANNEL T <TLOG CHANNEL>                                         */
    /************************************************************************/

    /// \brief Wrapper used to type-erase log channels.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TLogChannel>
    class LogChannelT : public LogChannel
    {
    public:

        /// \brief Create a new log channel.
        template <typename... TArguments>
        LogChannelT(Verbosity verbosity, Vector<Context> contexts, TArguments&&... arguments);

        /// \brief Create a new log channel.
        template <typename... TArguments>
        LogChannelT(Verbosity verbosity, const Context& context, TArguments&&... arguments);

        virtual void Send(const LogEvent& log_event) override;

        virtual void Send(LogEvent&& log_event) override;

        virtual void Flush() override;

    private:

        /// \brief Check whether an event can be sent to the channel.
        bool CanSend(const LogEvent& log_event);

        /// \brief Channel verbosity.
        Verbosity verbosity_{ Verbosity::kAll };

        /// \brief Contexts the channel reacts to.
        Vector<Context> contexts_{ Context{} };

        /// \brief Concrete log channel.
        TLogChannel log_channel_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LogChannelT<TLogChannel>.

    template <typename TLogChannel>
    template <typename... TArguments>
    inline LogChannelT<TLogChannel>::LogChannelT(Verbosity verbosity, Vector<Context> contexts, TArguments&&... arguments)
        : verbosity_(verbosity)
        , contexts_(std::move(contexts))
        , log_channel_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TLogChannel>
    template <typename... TArguments>
    LogChannelT<TLogChannel>::LogChannelT(Verbosity verbosity, const Context& context, TArguments&&... arguments)
        : LogChannelT(verbosity, Vector<Context>{ context }, std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TLogChannel>
    inline void LogChannelT<TLogChannel>::Send(const LogEvent& log_event)
    {
        if (CanSend(log_event))
        {
            log_channel_.Send(log_event);
        }
    }

    template <typename TLogChannel>
    inline void LogChannelT<TLogChannel>::Send(LogEvent&& log_event)
    {
        if (CanSend(log_event))
        {
            log_channel_.Send(std::move(log_event));
        }
    }

    template <typename TLogChannel>
    inline void LogChannelT<TLogChannel>::Flush()
    {
        log_channel_.Flush();
    }

    template <typename TLogChannel>
    inline bool LogChannelT<TLogChannel>::CanSend(const LogEvent& log_event)
    {
        auto filter_by_context = [this, &log_event](const auto& context)
        {
            return context.Contains(log_event.GetContext());
        };

        return (log_event.GetSeverity() >= verbosity_) && std::any_of(contexts_.cbegin(), contexts_.cend(), filter_by_context);
    }
}
