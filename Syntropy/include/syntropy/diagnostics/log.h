/// \file log.h
/// \brief This header is part of the syntropy diagnostic system. It contains logging classes and functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <sstream>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>
#include <memory>

#include "syntropy/diagnostics/diagnostics.h"

#include "syntropy/containers/context.h"

#include "syntropy/patterns/algorithm.h"

#include "syntropy/macro.h"

/// \brief Utility macro for sending a message to the log manager.
#define SYNTROPY_LOG_MESSAGE(stacktrace, contexts, severity, ...) \
    syntropy::diagnostics::GetLogManager().Send \
    ( \
        syntropy::diagnostics::LogMessage \
        ( \
            std::move(stacktrace) \
            , { SYNTROPY_EXPAND contexts } \
            , syntropy::diagnostics::Severity::severity \
            , __VA_ARGS__ \
        ) \
    )

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
        SYNTROPY_TRAP; \
    }

namespace syntropy
{
    namespace diagnostics
    {

        /// \brief Represents a single log message.
        /// \author Raffaele D. Facendola - November 2016
        struct LogMessage
        {
            /// \brief Create a new log message.
            /// \param stacktrace Callstack that caused the log. Can be either a full stack trace or the last function.
            /// \param contexts Log contexts used to categorize the log message.
            /// \param severity Severity of the log message.
            /// \param message Actual log message parts.
            template <typename... TMessage>
            LogMessage(StackTrace stacktrace, std::vector<Context> contexts, Severity severity, TMessage&&... message);

            std::chrono::system_clock::time_point time_;                    ///< \brief Time point associated to the message creation.
            Severity severity_;                                             ///< \brief Severity of the message.
            std::thread::id thread_id_;                                     ///< \brief Id of the thread that issued the message.
            std::vector<Context> contexts_;                                 ///< \brief Contexts used to categorize the message.
            StackTrace stacktrace_;                                         ///< \brief Stack trace.
            std::string message_;                                           ///< \brief Log message.

        };

        /// \brief Channel used to collect incoming log messages.
        /// \author Raffaele D. Facendola - November 2016
        class LogChannel
        {

        public:

            /// \brief Create a new channel.
            /// \param context Contexts bound to this channel. Used to filter log messages by context.
            /// \param verbosity Minimum required severity for which a message is processed.
            LogChannel(std::vector<Context> contexts, Severity verbosity = Severity::kInformative);

            /// \brief Virtual destructor.
            virtual ~LogChannel() = default;

            /// \brief Send a message to the channel.
            /// If the message context or verbosity do not match any of the ones specified by this channel, the message is ignored.
            /// \param log Message to send.
            LogChannel& operator<<(const LogMessage& log);

            /// \brief Get the verbosity level of the channel.
            /// This represents the minimum required severity for which a message is processed.
            /// \return Returns the verbosity level of the channel.
            Severity GetVerbosity() const;

            /// \brief Get the contexts this channel reacts to.
            /// Log messages must specify at least one of these contexts in order to be processed. Note that contexts are hierarchical.
            /// \return Returns the list of contexts this channel reacts to.
            const std::vector<Context>& GetContexts() const;

            /// \brief Flush any cached state.
            virtual void Flush() = 0;

        protected:

            /// \brief Handle a message.
            /// \param log Message to handle. The message is guaranteed to have severity equal or higher to the verbosity level and have at least one context matching at least one among the contexts bound to the stream. 
            /// \param contexts List of contexts specified by the log message that match any of the channel contexts.
            virtual void OnSendMessage(const LogMessage& log, const std::vector<Context>& contexts) = 0;

        private:

            std::vector<Context> contexts_;                 ///< \brief Contexts this channel reacts to.

            Severity verbosity_;                            ///< \brief Minimum severity needed to process a message.
        };

        /// \brief Singleton used to issue log messages and events.
        /// \author Raffaele D. Facendola - November 2016
        class LogManager
        {

        public:

            /// \brief Get the log manager instance.
            static LogManager& GetInstance();

            /// \brief Create new log channel.
            /// \tparam TLogChannel Type of channel to create. Must derive from LogChannel.
            /// \param args Arguments passed to the channel's constructor.
            /// \return Returns a reference to the new channel.
            template <typename TLogChannel, typename... TArgs>
            const TLogChannel& CreateChannel(TArgs&&... args);

            /// \brief Add a log channel to the manager.
            /// The manager takes ownership of the channel.
            /// \tparam TLogChannel Type of channel to create. Must derive from LogChannel.
            /// \param channel Channel to add to the manager.
            /// \return Returns a reference to the added channel.
            template <typename TLogChannel = LogChannel>
            const TLogChannel& AcquireChannel(std::unique_ptr<TLogChannel> channel);

            /// \brief Send a log message.
            /// \param log_message Log message to send.
            void Send(const LogMessage& log_message);

        private:

            /// \brief Flush any cached state.
            void Flush();

            /// \brief Prevents direct instantiation.
            LogManager() = default;

            std::recursive_mutex mutex_;                                    ///< \brief Used to synchronize various logging threads. Recursive because channel creation may cause logs.

            std::vector<std::unique_ptr<LogChannel>> channels_;             /// \brief List of log channels.
        };

        /// \brief Get a reference to the LogManager singleton.
        LogManager& GetLogManager();

        /// \brief Import a log manager configuration from a JSON file.
        /// Existing log channels are preserved: avoid importing the same file more than once.
        /// \param Path of the file containing the configuration.
        /// \return Returns true if the configuration could be imported successfully, returns false otherwise.
        bool ImportLogConfigurationFromJSON(const std::string& path);

    }
}

namespace syntropy
{
    namespace diagnostics
    {

        /************************************************************************/
        /* LOG MESSAGE                                                          */
        /************************************************************************/

        template <typename... TMessage>
        LogMessage::LogMessage(StackTrace stacktrace, std::vector<Context> contexts, Severity severity, TMessage&&... message)
            : time_(std::chrono::system_clock::now())
            , severity_(severity)
            , thread_id_(std::this_thread::get_id())
            , contexts_(std::move(contexts))
            , stacktrace_(std::move(stacktrace))
        {
            std::ostringstream builder;

            (builder << ... << message);

            message_ = builder.str();
        }

        /************************************************************************/
        /* LOG MANAGER                                                          */
        /************************************************************************/

        template <typename TLogChannel, typename... TArgs>
        const TLogChannel& LogManager::CreateChannel(TArgs&&... args)
        {
            static_assert(std::is_base_of_v<LogChannel, TLogChannel>, "TLogChannel must derive from LogChannel");

            std::unique_lock<std::recursive_mutex> lock(mutex_);

            channels_.emplace_back(std::make_unique<TLogChannel>(std::forward<TArgs>(args)...));

            return *static_cast<TLogChannel*>(channels_.back().get());
        }

        template <typename TLogChannel>
        const TLogChannel& LogManager::AcquireChannel(std::unique_ptr<TLogChannel> channel)
        {
            static_assert(std::is_base_of_v<LogChannel, TLogChannel>, "TLogChannel must derive from LogChannel");

            std::unique_lock<std::recursive_mutex> lock(mutex_);

            channels_.emplace_back(std::move(channel));

            return *static_cast<TLogChannel*>(channels_.back().get());
        }

    }
}

