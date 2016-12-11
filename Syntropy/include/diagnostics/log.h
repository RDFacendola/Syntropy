/// \file log.h
/// \brief This header is part of the syntropy diagnostic system. It contains classes used to handle logs used for diagnostic purposes.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <chrono>
#include <mutex>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

#include "macro.h"
#include "base_io.h"
#include "diagnostics.h"
#include "debug.h"

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
        SYNTROPY_LOG_MESSAGE(SYNTROPY_TRACE, kError, contexts, __VA_ARGS__); \
        SYNTROPY_BREAK; \
    }

/// \brief Log a critical error message with full stacktrace.
/// Causes the debugger to break and the application to crash.
/// \usage SYNTROPY_CRITICAL((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_CRITICAL(contexts, ...) \
    { \
        SYNTROPY_LOG_MESSAGE(SYNTROPY_TRACE, kCritical, contexts, __VA_ARGS__); \
        SYNTROPY_BREAK; \
    }

namespace syntropy 
{

    namespace diagnostics 
    {

        class StreamLogger;

        /// \brief Represents a single log message.
        /// \author Raffaele D. Facendola - November 2016
        struct Log : Event 
        {
            /// \brief Create a new log message.
            Log(std::initializer_list<Context> contexts, const StackTrace& stacktrace, Severity severity);

            const char* message_;                                           ///< \brief Log message.

        };

        /// \brief Log stream used to output log messages.
        /// \author Raffaele D. Facendola - November 2016
        class LogStream 
        {

        public:

            /// \brief Create a new log stream.
            LogStream();

            /// \brief Send a message to the stream.
            /// \param log Message to send.
            void SendMessage(const Log& log);

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
            virtual void OnSendMessage(const Log& log) = 0;

        private:

            std::set<Context> contexts_;                    ///< \brief Contexts this stream is bound to.

            Severity verbosity_;                            ///< \brief Minimum severity needed to log a message.
        };

        LogStream& operator<<(LogStream& log_stream, const Log& log);

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
            void AttachStream(std::shared_ptr<LogStream> log_stream);

            /// \brief Create and attach a log stream to the manager.
            /// \tparam TLogStream Type of the log stream to create. Must derive from LogStream.
            /// \param args Arguments passed to TLogStream's constructor.
            /// \return Returns a pointer to the new stream.
            template <typename TLogStream, typename... TArgs>
            std::shared_ptr<TLogStream> CreateStream(TArgs&&... args);

            /// \brief Detach an existing stream from the manager.
            /// \param stream Stream to detach.
            void DetachStream(std::shared_ptr<LogStream> stream);

            /// \brief Send a log message.
            /// \tparam kSeverity Severity of the message.
            /// \param stacktrace Stacktrace that caused the log.
            /// \param context Log contexts used to categorize the log message.
            template <Severity kSeverity, typename... TMessage>
            void SendMessage(const StackTrace& stacktrace, std::initializer_list<Context> contexts, TMessage&&... message);

        private:

            /// \brief Utility class used to build a log message
            /// This class is used to share a pool of streams without the need of creating a new one every single time.
            /// \author Raffaele D. Facendola - November 2016
            class MessageBuilder
            {

            public:

                /// \brief Create a new builder. Recycle an existing stream if possible.
                template <typename... TMessage>
                MessageBuilder(TMessage&&... message);

                /// \brief Get a pointer to the message.
                const char* GetMessage() const;

            private:

                template <typename THead, typename... TRest>
                void Append(THead head, TRest... rest);

                void Append();

                /// \brief Get the mutex used for synchronization purposes.
                static std::mutex& GetMutex();

                /// \brief Get the pool of recyclable streams.
                static std::vector<std::unique_ptr<std::ostringstream>>& GetPool();

                void AcquireStream();

                void ReleaseStream();

                std::unique_ptr<std::ostringstream> stream_;                    ///< \brief Stream associated to the builder.

                std::string message_;                                           ///< \brief Actual message.

            };

            /// \brief Prevents direct instantiation.
            LogManager() = default;

            std::mutex mutex_;                                              ///< \brief Used to synchronize various logging threads.

            std::vector<std::shared_ptr<LogStream>> streams_;               ///< \brief List of log streams.

        };

        template <typename TLog>
        struct LogTokenTranslator : EventTokenTranslator<TLog>
        {
            virtual EventTokenTranslator<TLog>::TThunk operator()(const std::string& token) const override;
        };

        class LogFormatter : public Formatter<Log, LogTokenTranslator<Log>>
        {
        public:

            LogFormatter(const std::string& format, StreamLogger& logger);

        private:

            StreamLogger& logger_;

        };

        /// \brief Used to redirect a log output to an output stream.
        /// \author Raffaele D. Facendola - December 2016
        class StreamLogger : public LogStream
        {

        public:

            /// \brief Create a new stream logger.
            /// \param stream Stream where the messages are appended to.
            /// \param format Format of the log messages. See LogMessageFormatter.
            /// \param flush_severity Minimum severity required in order to trigger a stream flush.
            StreamLogger(std::ostream& stream, const char* format, Severity flush_severity = Severity::kCritical);

        protected:

            virtual void OnSendMessage(const Log& log) override;

        private:

            std::ostream& stream_;                      ///< \brief Stream where the messages are appended to.

            LogFormatter formatter_;                    ///< \brief Used to format the log messages.

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
            MessageBuilder message_builder(std::forward<TMessage>(message)...);     // Before the lock so the message can be built while another thread is writing to the log

            std::unique_lock<std::mutex> lock(mutex_);                              // Needed to guarantee the order of the log messages

            Log log(contexts, stacktrace, kSeverity);
            
            log.message_ = message_builder.GetMessage();

            // Send the log to the streams
            for (auto&& stream : streams_)
            {
                *stream << log;
            }
        }

        //////////////// LOG MANAGER :: MESSAGE BUILDER ////////////////

        template <typename... TMessage>
        LogManager::MessageBuilder::MessageBuilder(TMessage&&... message)
        {
            AcquireStream();

            Append(std::forward<TMessage>(message)...);
            message_ = stream_->str();

            ReleaseStream();
        }

        template <typename THead, typename... TRest>
        void LogManager::MessageBuilder::Append(THead head, TRest... rest)
        {
            (*stream_) << head;
            Append(std::forward<TRest>(rest)...);
        }

        //////////////// LOG TOKEN TRANSLATOR ////////////////

        template <typename TLog>
        typename EventTokenTranslator<TLog>::TThunk LogTokenTranslator<TLog>::operator()(const std::string& token) const
        {
            if (token == "{message}")
            {
                return [](std::ostream& out, const Log& log) { out << log.message_; };
            }
            else if (token == "{logcontexts}")
            {

            }
            return EventTokenTranslator<TLog>::operator()(token);
        }

    }

}