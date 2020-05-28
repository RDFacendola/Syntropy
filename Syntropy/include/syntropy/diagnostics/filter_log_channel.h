
/// \file filter_log_channel.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for log channels used to filter events based on their severity and context.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/diagnostics/log_event.h"
#include "syntropy/diagnostics/verbosity.h"
#include "syntropy/core/context.h"
#include "syntropy/containers/vector.h"

namespace syntropy
{
    /************************************************************************/
    /* FILE LOG CHANNEL                                                     */
    /************************************************************************/

    /// \brief A meta log channel that filters log events before sending them to an underlying log channel
    /// Event filtering is based on severity and contexts inclusion.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TLogChannel>
    class FilterLogChannel
    {
    public:

        /// \brief Create a new file log channel.
        template <typename... TArguments>
        FilterLogChannel(Verbosity verbosity, Vector<Context> contexts, TArguments&&... arguments);

        /// \brief Send a log event to the channel.
        void Send(const LogEvent& log_event);

        /// \brief Flush cached states.
        void Flush();

    private:

        /// \brief Test whether a log event meets the verbosity and context requirements of the channel.
        /// \return Returns true if the event meets the log criteria, returns false otherwise.
        bool Filter(const LogEvent& log_event);

        /// \brief Channel verbosity.
        Verbosity verbosity_{ Verbosity::kAll };

        /// \brief Contexts the channel reacts to.
        Vector<Context> contexts_{ Context{} };

        /// \brief Underlying log channel.
        TLogChannel log_channel_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // FilterLogChannel<TLogChannel>.

    template <typename TLogChannel>
    template <typename... TArguments>
    inline FilterLogChannel<TLogChannel>::FilterLogChannel(Verbosity verbosity, Vector<Context> contexts, TArguments&&... arguments)
        : verbosity_(verbosity)
        , contexts_(std::move(contexts))
        , log_channel_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TLogChannel>
    inline void FilterLogChannel<TLogChannel>::Send(const LogEvent& log_event)
    {
        if (Filter(log_event))
        {
            log_channel_.Send(log_event);
        }
    }

    template <typename TLogChannel>
    inline bool FilterLogChannel<TLogChannel>::Filter(const LogEvent& log_event)
    {
        auto filter_by_context = [this, &log_event](const auto& context)
        {
            return context.Contains(log_event.GetContext());
        };

        return (log_event.GetSeverity() >= verbosity_) && std::any_of(contexts_.cbegin(), contexts_.cend(), filter_by_context);
    }

    template <typename TLogChannel>
    inline void FilterLogChannel<TLogChannel>::Flush()
    {
        log_channel_.Flush();
    }

}
