/// \file log.h
/// \brief This header is part of the syntropy diagnostic system. It contains classes used to handle logs used for diagnostic purposes.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <chrono>
#include <mutex>
#include <sstream>
#include <algorithm>
#include <set>
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
            std::set<Context> contexts_;                                    ///< \brief Contexts used to categorize the message.
            StackTrace stacktrace_;                                         ///< \brief Stack trace.
            std::string message_;                                           ///< \brief Log message.
        };

        /// \brief Log stream used to output log messages.
        /// \author Raffaele D. Facendola - November 2016
        class BaseLogStream 
        {

        public:

            /// \brief Create a new log stream.
            BaseLogStream();

            /// \brief Send a message to the stream.
            /// \param log Message to send.
            BaseLogStream& operator<<(const LogMessage& log);

            /// \brief Set the verbosity level of the stream.
            /// Messages with severity lower than the stream verbosity are ignored.
            /// \param verbosity New level of verbosity to set.
            void SetVerbosity(Severity verbosity);

            /// \brief Get the verbosity level of the stream.
            /// \return Returns the verbosity level of the stream.
            Severity GetVerbosity() const;

            /// \brief Bind the log stream to one or more contexts.
            /// Messages whose contexts do not match with any of the ones bound to this stream are ignored.
            /// \param context List of contexts to bound to this stream.
            void BindContext(std::initializer_list<Context> contexts);

            /// \brief Unbind a context from the stream.
            /// \param context Context to unbind.
            void UnbindContext(const Context& context);

            /// \brief Get the set of contexts bound to this log stream.
            /// \return Returns the list of contexts bound to this log stream.
            const std::set<Context>& GetBoundContexts() const;

        protected:

            /// \brief Handle a message sent to the stream.
            /// \param log Message to handle. The message is guaranteed to have severity equal or higher to the verbosity level and have at least one context matching at least one among the contexts bound to the stream. 

            virtual void OnSendMessage(const LogMessage& log) = 0;

            /// \brief Get the contexts defined in a log message that are contained in at least one context bound to this log stream.
            std::set<Context> GetHitContexts(const LogMessage& log) const;

        private:

            /// \brief Check whether a log message matches the log stream requirements.
            /// \return Returns true if log severity and contexts match against the log stream's ones, returns false otherwise.
            bool Match(const LogMessage& log) const;

            std::set<Context> contexts_;                    ///< \brief Contexts this stream is bound to.

            Severity verbosity_;                            ///< \brief Minimum severity needed to log a message.
        };

        /// \brief Singleton used to issue log messages and events.
        /// \author Raffaele D. Facendola - November 2016
        class LogManager 
        {

        public:

            /// \brief Get the log manager instance.
            static LogManager& GetInstance();

            /// \brief Attach a log stream to the manager.
            /// If the stream is already attached to the manager, this method does nothing.
            /// \param log_stream Stream to attach.
            void AttachStream(std::shared_ptr<BaseLogStream> log_stream);

            /// \brief Create and attach a log stream to the manager.
            /// \tparam TLogStream Type of the log stream to create. Must derive from LogStream.
            /// \param args Arguments passed to TLogStream's constructor.
            /// \return Returns a pointer to the new stream.
            template <typename TLogStream, typename... TArgs>
            std::shared_ptr<TLogStream> CreateStream(TArgs&&... args);

            /// \brief Detach an existing stream from the manager.
            /// \param stream Stream to detach.
            void DetachStream(std::shared_ptr<BaseLogStream> stream);

            /// \brief Send a log message.
            /// \tparam kSeverity Severity of the message.
            /// \param stacktrace Stacktrace that caused the log.
            /// \param context Log contexts used to categorize the log message.
            template <Severity kSeverity, typename... TMessage>
            void SendMessage(const StackTrace& stacktrace, std::initializer_list<Context> contexts, TMessage&&... message);

        private:

            /// \brief Prevents direct instantiation.
            LogManager() = default;

            std::recursive_mutex mutex_;                                    ///< \brief Used to synchronize various logging threads.

            std::ostringstream message_builder_;                            ///< \brief Stream used to build log messages.

            std::set<std::shared_ptr<BaseLogStream>> streams_;              ///< \brief List of log streams.

        };

        /// \brief Used to redirect a log output to an output stream.
        /// \author Raffaele D. Facendola - December 2016
        class StreamLog : public BaseLogStream
        {

        public:

            // Valid tokens for the format string

            static const std::string kTimeToken;                ///< \brief {time} Output the time of the day of the log message in the format hh:mm:ss.ms
            static const std::string kDateToken;                ///< \brief {date} Output the date of the log message in the format YYYY-MM-DD
            static const std::string kSeverityToken;            ///< \brief {severity} Output the severity of the log message.
            static const std::string kThreadToken;              ///< \brief {thread} Output the thread that caused the log message.
            static const std::string kContextsToken;            ///< \brief {context} Output the list of contexts of the log message that were bound to the stream in the format Context1, ..., ContextN
            static const std::string kStackTraceToken;          ///< \brief {trace} Output the stack trace that caused the log message if available, output to the function name otherwise.
            static const std::string kFunctionToken;            ///< \brief {function} Output the function that caused the log message.
            static const std::string kMessageToken;             ///< \brief {message} Output the actual log message text.

            static const char kTokenStart;                      ///< \brief Character delimiting the begin of a token. '{'.
            static const char kTokenEnd;                        ///< \brief Character delimiting the end of a token. '}'.

            /// \brief Create a new stream logger.
            /// The format string is used to determine the format of the message log sent to the message stream.
            /// An example of a valid format string is "{date} {time} [{context}]: {message}.
            /// Unrecognized tokens in the format string are considered regular strings.
            /// \param stream Stream where the messages are appended to.
            /// \param format Format of the log messages. See LogMessageFormatter.
            /// \param flush_severity Minimum severity required in order to trigger a stream flush.
            StreamLog(std::ostream& stream, const std::string& format, Severity flush_severity = Severity::kCritical);

        protected:

            virtual void OnSendMessage(const LogMessage& log) override;

        private:

            using Thunk = std::function<void(std::ostream&, const LogMessage&)>;

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

        // Implementation

        //////////////// LOG MANAGER ////////////////

        template <typename TLogStream, typename... TArgs>
        std::shared_ptr<TLogStream> LogManager::CreateStream(TArgs&&... args)
        {
            auto stream = std::make_shared<TLogStream>(std::forward<TArgs>(args)...);
            
            AttachStream(stream);

            return stream;
        }

        template <Severity kSeverity, typename... TMessage>
        void LogManager::SendMessage(const StackTrace& stacktrace, std::initializer_list<Context> contexts, TMessage&&... message)
        {
            std::unique_lock<std::recursive_mutex> lock(mutex_);

            if (streams_.size() > 0)
            {
                Insert(message_builder_, std::forward<TMessage>(message)...);

                LogMessage log;
            
                log.severity_ = kSeverity;
                log.stacktrace_ = stacktrace;
                log.contexts_ = contexts;
                log.message_ = message_builder_.str();

                // Send the log to the streams
                for (auto&& stream : streams_)
                {
                    *stream << log;
                }

                // Clear the message builder
                message_builder_.clear();
                message_builder_.str("");
            }

        }

    }

}