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

/// \brief Log an informative message.
/// \usage SYNTROPY_LOG((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_LOG(contexts, ...) \
    { \
        syntropy::diagnostics::LogMessageBuilder message; \
        message.Append(__VA_ARGS__); \
        syntropy::diagnostics::LogManager::GetInstance().SendMessage<syntropy::diagnostics::Severity::kInformative>({ SYNTROPY_EXPAND contexts }, (*message).str().c_str(), SYNTROPY_TRACE); \
    }

/// \brief Log a warning message.
/// \usage SYNTROPY_WARNING((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_WARNING(contexts, ...) \
    { \
        syntropy::diagnostics::LogMessageBuilder message; \
        message.Append(__VA_ARGS__); \
        syntropy::diagnostics::LogManager::GetInstance().SendMessage<syntropy::diagnostics::Severity::kWarning>({ SYNTROPY_EXPAND contexts }, (*message).str().c_str(), SYNTROPY_TRACE); \
    }

/// \brief Log an informative message.
/// Causes the debugger to break.
/// \usage SYNTROPY_ERROR((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_ERROR(contexts, ...) \
    { \
        syntropy::diagnostics::LogMessageBuilder message; \
        message.Append(__VA_ARGS__); \
        syntropy::diagnostics::LogManager::GetInstance().SendMessage<syntropy::diagnostics::Severity::kError>({ SYNTROPY_EXPAND contexts }, (*message).str().c_str(), SYNTROPY_TRACE); \
        SYNTROPY_DEBUG_BREAK; \
    }

/// \brief Log an informative message.
/// Causes the debugger to break and the application to crash.
/// \usage SYNTROPY_CRITICAL((Context1, Context2, ...), "This is the number: ", 2, "!");
#define SYNTROPY_CRITICAL(contexts, ...) \
    { \
        syntropy::diagnostics::LogMessageBuilder message; \
        message.Append(__VA_ARGS__); \
        syntropy::diagnostics::LogManager::GetInstance().SendMessage<syntropy::diagnostics::Severity::kCritical>({ SYNTROPY_EXPAND contexts }, (*message).str().c_str(), SYNTROPY_TRACE); \
        SYNTROPY_DEBUG_BREAK; \
    }

namespace syntropy 
{

    namespace diagnostics 
    {

        /// \brief Single log message.
        /// \author Raffaele D. Facendola - November 2016
        struct LogMessage : Event 
        {
            /// \brief Create a new log message.
            LogMessage(std::initializer_list<Context> contexts, const Callstack& callstack, Severity severity);

            const char* message_;                                           ///< \brief Log message.

        };

        class LogAppender {

        public:

            void SendMessage(const LogMessage& log) {
                (log);
            }

        };

        /// \brief Singleton used to issue log messages and events.
        /// \author Raffaele D. Facendola - November 2016
        class LogManager 
        {

        public:

            /// \brief Get the log manager instance.
            static LogManager& GetInstance();

            /// \brief Send a log message.
            /// \tparam kSeverity Severity of the message.
            /// \param context Log context used to categorize the log message.
            /// \param callstack Callstack that caused the log.
            template <Severity kSeverity>
            void SendMessage(std::initializer_list<Context> contexts, const char* message, const Callstack& callstack);

        private:

            /// \brief Prevent direct instantiation.
            LogManager() = default;

            std::mutex mutex_;                                              ///< \brief Used to synchronize various logging threads.

            std::vector<std::unique_ptr<LogAppender>> appenders_;           ///< \brief List of log appenders

        };

        /// \brief Utility class used build a log message via a stream.
        /// This class is used to share a pool of streams without the need of creating a new one every single time.
        /// \author Raffaele D. Facendola - November 2016
        class LogMessageBuilder
        {

        public:

            /// \brief Create a new builder. Recycle an existing stream if possible.
            LogMessageBuilder();

            /// \brief Default destructor. Send the current stream to the recycle pool.
            ~LogMessageBuilder();

            /// \brief Move constructor.
            LogMessageBuilder(LogMessageBuilder&& other) = default;

            std::ostringstream& operator*() const;

            template <typename THead, typename... TRest>
            void Append(THead head, TRest... rest);

            template <typename THead>
            void Append(THead head);

        private:
            
            /// \brief Get the mutex used for synchronization purposes.
            static std::mutex& GetMutex();

            /// \brief Get the pool of recyclable streams.
            static std::vector<std::unique_ptr<std::ostringstream>>& GetPool();
            
            std::unique_ptr<std::ostringstream> stream_;                    ///< \brief Stream associated to the builder.

        };

    }
    
}

namespace syntropy {

    namespace diagnostics {

        // Implementation

        //////////////// LOG MANAGER ////////////////

        template <Severity kSeverity>
        void LogManager::SendMessage(std::initializer_list<Context> contexts, const char* message, const Callstack& callstack) {

            std::unique_lock<std::mutex> lock(mutex_);      // Needed to guarantee the order of the log messages.

            LogMessage log_message(contexts, callstack, kSeverity);

            log_message.message_ = message;

            // Send the log to the appenders
            for (auto&& appender : appenders_) {

                appender->SendMessage(log_message);

            }

        }

        //////////////// LOG MESSAGE BUILDER ////////////////

        template <typename THead, typename... TRest>
        void LogMessageBuilder::Append(THead head, TRest... rest)
        {
            Append(head);
            Append(rest...);
        }

        template <typename THead>
        void LogMessageBuilder::Append(THead head)
        {
            (*stream_) << head;
        }

    }

}