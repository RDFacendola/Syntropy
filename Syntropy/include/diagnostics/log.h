/// \file log.h
/// \brief This header is part of the syntropy diagnostic system. It contains classes used to handle logs used for diagnostic purposes.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <chrono>
#include <mutex>
#include <sstream>
#include <algorithm>
#include <vector>
#include <thread>

#include "macro.h"
#include "diagnostics.h"
#include "debug.h"
#include "algorithm.h"

/// \brief Utility macro for sending a message to the log manager.
#define SYNTROPY_LOG_MESSAGE(trace, severity, contexts, ...) \
    syntropy::diagnostics::LogManager::GetInstance().SendMessage<syntropy::diagnostics::Severity::severity>(trace, { SYNTROPY_EXPAND contexts }, __VA_ARGS__);

/// \brief Log an informative message.
/// \usage SYNTROPY_LOG((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_LOG(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_HERE, kInformative, contexts, __VA_ARGS__); \
    }

/// \brief Log a warning message.
/// \usage SYNTROPY_WARNING((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_WARNING(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_HERE, kWarning, contexts, __VA_ARGS__); \
    }

/// \brief Log an error message with full stacktrace.
/// Causes the debugger to break
/// \usage SYNTROPY_ERROR((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_ERROR(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_STACKTRACE, kError, contexts, __VA_ARGS__); \
        SYNTROPY_BREAK; \
    }

/// \brief Log a critical error message with full stacktrace.
/// Causes the debugger to break and the application to crash.
/// \usage SYNTROPY_CRITICAL((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_CRITICAL(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_STACKTRACE, kCritical, contexts, __VA_ARGS__); \
        SYNTROPY_BREAK; \
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
            LogMessage();

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

            /// \brief Send a message to the channel.
            /// If the message context or verbosity do not match any of the ones specified by this channel, the message is ignored.
            /// \param log Message to send.
            LogChannel& operator<<(const LogMessage& log);

            /// \brief Get the verbosity level of the channel.
            /// This represents the minimum required severity for which a message is processed.
            /// \return Returns the verbosity level of the channel.
            Severity GetVerbosity() const;

            /// \brief Get the contexts this channel reacts to.
            /// Log messages must specifiy at least one of these contexts in order to be processed. Note that contexts are hierarchical.
            /// \return Returns the list of contexts this channel reacts to.
            const std::vector<Context>& GetContexts() const;

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

            /// \brief Send a log message.
            /// \tparam kSeverity Severity of the message.
            /// \param stacktrace Stacktrace that caused the log.
            /// \param context Log contexts used to categorize the log message.
            template <Severity kSeverity, typename... TMessage>
            void SendMessage(const StackTrace& stacktrace, std::vector<Context> contexts, TMessage&&... message);

        private:

            /// \brief Prevents direct instantiation.
            LogManager() = default;

            std::recursive_mutex mutex_;                                    ///< \brief Used to synchronize various logging threads.

            std::ostringstream message_builder_;                            ///< \brief Stream used to build log messages.

            std::vector<std::unique_ptr<LogChannel>> channels_;             /// \brief List of log channels.
        };

        /// \brief Channel used to redirect formatted log messages to an output stream.
        /// \author Raffaele D. Facendola - December 2016
        class StreamLogChannel : public LogChannel
        {

        public:

            // Tokens for the format string

            static const std::string kTimeToken;                ///< \brief {time} Output the time of the day of the log message in the format hh:mm:ss.ms
            static const std::string kDateToken;                ///< \brief {date} Output the date of the log message in the format YYYY-MM-DD
            static const std::string kSeverityToken;            ///< \brief {severity} Output the severity of the log message.
            static const std::string kThreadToken;              ///< \brief {thread} Output the thread that caused the log message.
            static const std::string kContextsToken;            ///< \brief {context} Output the list of contexts the channel reacted to in the format Context1, ..., ContextN
            static const std::string kStackTraceToken;          ///< \brief {trace} Output the stack trace that caused the log message if available, output the last function name otherwise.
            static const std::string kFunctionToken;            ///< \brief {function} Output the function that caused the log message.
            static const std::string kMessageToken;             ///< \brief {message} Output the actual log message text.

            static const char kTokenStart;                      ///< \brief Character delimiting the begin of a token. '{'.
            static const char kTokenEnd;                        ///< \brief Character delimiting the end of a token. '}'.

            /// \brief Configuration for a StreamLogChannel.
            struct Configuration
            {
                std::ostream& stream_;              ///< \brief Stream where the messages will be redirected to.
                std::string format_;                ///< \brief Format of the messages. Example of a valid format string: "{date} {time} [{context}]: {message}". Unrecognized tokens are considered plain strings.
                std::vector<Context> contexts_;     ///< \brief Contexts the channel should react to.
                Severity verbosity_;                ///< \brief Minimum required severity for which a message is processed.
                Severity flush_severity_;           ///< \brief Minimum severity required for a stream flush. Used to prevent log message loss after an application crash.
            };

            /// \brief Create a new stream log channel.
            /// \param configuration Configuration for this stream.
            StreamLogChannel(const Configuration& configuration);

        private:

            virtual void OnSendMessage(const LogMessage& log, const std::vector<Context>& contexts) override;

            /// \brief Arguments passed to the format thunk.
            struct ThunkArgs
            {
                std::ostream& out_;
                const LogMessage& log_;
                const std::vector<Context>& contexts_;
            };

            using Thunk = std::function<void(ThunkArgs)>;

            /// \brief Update the thunk list according to the provided format string.
            void UpdateThunks(const std::string& format);

            /// \brief Get a thunk wrapping the function call associated to a token.
            Thunk GetTokenThunk(const std::string& token);

            std::ostream& stream_;                      ///< \brief Stream where the messages are appended to.

            std::vector<Thunk> thunks_;                 ///< \brief List of functions used to output a formatted log message to the stream.

            Severity flush_severity_;                   ///< \brief Minimum severity required in order to trigger a stream flush.
        };

    }
}

namespace syntropy 
{
    namespace diagnostics 
    {

        /************************************************************************/
        /* LOG MANAGER                                                          */
        /************************************************************************/

        template <typename TLogStream, typename... TArgs>
        const TLogStream& LogManager::CreateChannel(TArgs&&... args)
        {
            std::unique_lock<std::recursive_mutex> lock(mutex_);

            channels_.emplace_back(std::make_unique<TLogStream>(std::forward<TArgs>(args)...));

            return *static_cast<TLogStream*>(channels_.back().get());
        }

        template <Severity kSeverity, typename... TMessage>
        void LogManager::SendMessage(const StackTrace& stacktrace, std::vector<Context> contexts, TMessage&&... message)
        {
            std::unique_lock<std::recursive_mutex> lock(mutex_);

            if (channels_.size() > 0)
            {
                Insert(message_builder_, std::forward<TMessage>(message)...);

                LogMessage log;

                log.severity_ = kSeverity;
                log.stacktrace_ = stacktrace;
                log.contexts_ = std::move(contexts);
                log.message_ = message_builder_.str();

                // Send the log to all the channels
                for (auto&& channel : channels_)
                {
                    *channel << log;
                }

                // Clear the message builder
                message_builder_.clear();
                message_builder_.str("");
            }
        }

    }
}