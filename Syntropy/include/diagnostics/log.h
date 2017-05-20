/// \file log.h
/// \brief This header is part of the syntropy diagnostic system. It contains utility macros used to send log messages.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <sstream>
#include <vector>
#include <string>

#include "diagnostics/diagnostics.h"
#include "diagnostics/debug.h"

#include "macro.h"
#include "algorithm.h"

/// \brief Utility macro for sending a message to the log manager.
#define SYNTROPY_LOG_MESSAGE(stacktrace, contexts, severity, ...) \
    syntropy::diagnostics::SendLogMessage(stacktrace, { SYNTROPY_EXPAND contexts }, syntropy::diagnostics::Severity::severity, __VA_ARGS__)

/// \brief Log an informative message.
/// \usage SYNTROPY_LOG((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_LOG(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_HERE, contexts, kInformative, __VA_ARGS__); \
    }

/// \brief Log a warning message.
/// \usage SYNTROPY_WARNING((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_WARNING(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_HERE, contexts, kWarning, __VA_ARGS__); \
    }

/// \brief Log an error message with full stacktrace.
/// Causes the debugger to break
/// \usage SYNTROPY_ERROR((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_ERROR(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_STACKTRACE, contexts, kError, __VA_ARGS__); \
        SYNTROPY_BREAK; \
    }

/// \brief Log a critical error message with full stacktrace.
/// Causes the debugger to break and the application to crash.
/// \usage SYNTROPY_CRITICAL((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_CRITICAL(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_STACKTRACE, contexts, kCritical, __VA_ARGS__); \
        SYNTROPY_BREAK; \
    }

namespace syntropy
{
    namespace diagnostics
    {

        /// \brief Send a log message to the log manager.
        /// \param stacktrace Stacktrace that caused the log. Can be either a full stack trace or the last function.
        /// \param contexts Log contexts used to categorize the log message.
        /// \param severity Severity of the log message.
        /// \param message Actual log message.
        void SendLogMessage(StackTrace stacktrace, std::vector<Context> contexts, Severity severity, std::string message);

        /// \brief Utility method used to compose a log message.
        template <typename... TMessage>
        void SendLogMessage(StackTrace stacktrace, std::vector<Context> contexts, Severity severity, TMessage&&... message)
        {
            std::ostringstream builder;

            Insert(builder, std::forward<TMessage>(message)...);

            SendLogMessage(std::move(stacktrace), std::move(contexts), severity, builder.str());
        }

    }
}
