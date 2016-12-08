/// \file log.h
/// \brief This header is part of the syntropy diagnostic system. It contains classes used to handle logs used for debug and diagnostics.
///
/// \author Raffaele D. Facendola - 2016

#pragma once


#include <chrono>
#include <mutex>
#include <vector>
#include <sstream>

#include "macro.h"
#include "diagnostics.h"
#include "platform/platform.h"

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

        /// \brief Represents a single log message.
        /// \author Raffaele D. Facendola - November 2016
        struct LogMessage : Event 
        {
            /// \brief Create a new log message.
            LogMessage(std::initializer_list<Context> contexts, const StackTrace& stacktrace, Severity severity);

            const char* message_;                                           ///< \brief Log message.

        };

        /// \brief Used to format a log message event to a string.
        /// \author Raffaele D. Facendola - December 2016
        struct LogMessageFormatter : EventFormatter
        {

        };

        class LogStream 
        {

        public:

            LogStream();

            void SendMessage(const LogMessage& log);

            void SetVerbosity(Severity verbosity);

            Severity GetVerbosity() const;

            void BindContext(std::initializer_list<Context> contexts);

            void UnbindContext(const Context& context);

        protected:

            virtual void OnSendMessage(const LogMessage& log) = 0;

        private:

            std::vector<Context> contexts_;                 ///< \brief Contexts this stream is bound to.

            Severity verbosity_;                            ///< \brief Minimum severity needed to log a message.

        };

        LogStream& operator<<(LogStream& log_stream, const LogMessage& log);

        /// \brief Singleton used to issue log messages and events.
        /// \author Raffaele D. Facendola - November 2016
        class LogManager 
        {

        public:

            /// \brief Get the log manager instance.
            static LogManager& GetInstance();

            void AttachStream(std::shared_ptr<LogStream> log_stream);

            template <typename TLogStream, typename... TArgs>
            std::shared_ptr<TLogStream> CreateStream(TArgs&&... args);

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

            /// \brief Prevent direct instantiation.
            LogManager() = default;

            std::mutex mutex_;                                              ///< \brief Used to synchronize various logging threads.

            std::vector<std::shared_ptr<LogStream>> streams_;               ///< \brief List of log streams.

        };
        
        class StreamLogStream : public LogStream
        {

        public:

            StreamLogStream(std::ostream& stream)
                : stream_(stream)
            {

            }

        protected:

            virtual void OnSendMessage(const LogMessage& log) override
            {
                stream_ << "[" << log.thread_id_ << "] " << log.message_ << " \n" << log.stacktrace_ << "\n";
            }

        private:

            std::ostream& stream_;
            
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

            LogMessage log_message(contexts, stacktrace, kSeverity);
            
            log_message.message_ = message_builder.GetMessage();

            // Send the log to the streams
            for (auto&& stream : streams_)
            {
                *stream << log_message;
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

    }

}