
/// \file log_event_filter.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for functor used to filter log events.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/diagnostics/log_event.h"
#include "syntropy/diagnostics/verbosity.h"
#include "syntropy/core/context.h"
#include "syntropy/core/vector.h"

namespace Syntropy
{
    /************************************************************************/
    /* LOG EVENT FILTER                                                     */
    /************************************************************************/

    /// \brief Object used to filter log events by severity and context inclusion.
    /// \author Raffaele D. Facendola - May 2020.
    class LogEventFilter
    {
    public:

        /// \brief Default constructor. Accept events of any severity and in any context.
        LogEventFilter() = default;

        /// \brief Create a new log event filter.
        LogEventFilter(Verbosity verbosity, Array<Context> contexts);

        /// \brief Create a new log event filter.
        LogEventFilter(Verbosity verbosity);

        /// \brief Create a new log event filter.
        LogEventFilter(Array<Context> contexts);

        /// \brief Default copy constructor.
        LogEventFilter(const LogEventFilter&) = default;

        /// \brief Default move constructor.
        LogEventFilter(LogEventFilter&&) = default;

        /// \brief Default copy-assignment operator.
        LogEventFilter& operator=(const LogEventFilter&) = default;

        /// \brief Default move-assignment operator.
        LogEventFilter& operator=(LogEventFilter&&) = default;

        /// \brief Default destructor.
        ~LogEventFilter() = default;

        /// \brief Test a log event against this filter.
        /// \return Returns true if the log event meets the filter requirements, returns false otherwise.
        Bool operator()(const LogEvent& log_event) const;

    private:

        /// \brief Filter verbosity.
        Verbosity verbosity_{ Verbosity::kAll };

        /// \brief Filter contexts.
        Array<Context> contexts_{ Context{} };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LogEventFilter.

    inline LogEventFilter::LogEventFilter(Verbosity verbosity, Array<Context> contexts)
        : verbosity_(verbosity)
        , contexts_(Move(contexts))
    {

    }

    inline LogEventFilter::LogEventFilter(Verbosity verbosity)
        : verbosity_(verbosity)
    {

    }

    inline LogEventFilter::LogEventFilter(Array<Context> contexts)
        : contexts_(Move(contexts))
    {

    }

    inline Bool LogEventFilter::operator()(const LogEvent& log_event) const
    {
        auto filter_by_context = [this, &log_event](const auto& context)
        {
            return context.Contains(log_event.GetContext());
        };

        return (log_event.GetSeverity() >= verbosity_) && std::any_of(contexts_.cbegin(), contexts_.cend(), filter_by_context);
    }

}
