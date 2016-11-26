/// \file log.h
/// \brief This header is part of the syntropy diagnostic system. It contains classes used to handle logs used for debug and diagnostics.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

#include "diagnostics.h"

#define SYNTROPY_LOG_MESSAGE(severity, context, message) \
    syntropy::diagnostics::LogManager::GetInstance().LogMessage<severity>(context, message, SYNTROPY_SOURCECODE)

#define SYNTROPY_LOG(context, message) \
    SYNTROPY_LOG_MESSAGE(syntropy::diagnostics::SeverityInformative, context, message)

#define SYNTROPY_WARNING(context, message) \
    SYNTROPY_LOG_MESSAGE(syntropy::diagnostics::SeverityWarning, context, message)

#define SYNTROPY_ERROR(context, message) \
    SYNTROPY_LOG_MESSAGE(syntropy::diagnostics::SeverityError, context, message)

#define SYNTROPY_CRITICAL(context, message) \
    SYNTROPY_LOG_MESSAGE(syntropy::diagnostics::SeverityCritical, context, message)

namespace syntropy {

    namespace diagnostics {

        /// \brief Tag used for informative logs that have no effect on the application.
        struct SeverityInformative {

        };

        /// \brief Tag used for warning logs signaling conditions that may cause undesired, yet acceptable, behaviors.
        struct SeverityWarning {
        
            static const bool kLogCallstack = true;         ///< \brief Whether to attach the callstack to the log.

        };

        /// \brief Tag used for error logs signaling conditions that will affect the application in a negative way. Causes the debugger to break.
        struct SeverityError {

            static const bool kLogCallstack = true;         ///< \brief Whether to attach the callstack to the log.

            static const bool kBreakOnLog = true;           ///< \brief Whether to break after logging. Does nothing on release builds.

        };

        /// \brief Tag used for critical logs signaling conditions that will cause the application to crash almost certainly. Causes the debugger to break and the application to crash.
        struct SeverityCritical {

            static const bool kLogCallstack = true;         ///< \brief Whether to attach the callstack to the log.

            static const bool kBreakOnLog = true;           ///< \brief Whether to break after logging. Does nothing on release builds.

            static const bool kCrashOnLog = true;           ///< \brief Whether to crash after logging.

        };

        /// \brief Represents a single log entry.
        struct LogEntry {

            LogEntry(const SourceCodeLocation& where);

            std::chrono::system_clock::time_point timestamp_;               ///< \brief System time.

            // TODO: Severity!

            std::vector<const char*> context_;                              ///< \brief Log context.

            const char* message_;                                           ///< \brief Actual log message.

            const SourceCodeLocation where_;                                ///< \brief Location where the log was issued.
            
            std::thread::id thread_id_;                                     ///< \brief Id of the thread that issued the log.

            std::vector<SourceCodeLocation> callstack_;                     ///< \brief Callstack, starting from the current location.

        };

        class LogAppender {

        public:

            void LogMessage(const LogEntry& log) {
                (log);
            }

        };

        class LogManager {

        public:

            static LogManager& GetInstance();

            template <typename TSeverity>
            void LogMessage(std::initializer_list<const char*> context, const char* message, const SourceCodeLocation& where);

        private:

            /// \brief Prevent direct instantiation.
            LogManager() = default;

            std::mutex mutex_;                                              ///< \brief Used to synchronize various logging threads.

            std::vector<std::unique_ptr<LogAppender>> appenders_;           ///< \brief List of log appenders

        };

    }
    
}

namespace syntropy {

    namespace diagnostics {

        // Implementation

        //////////////// LOG MANAGER ////////////////

        template <typename TSeverity>
        void LogManager::LogMessage(std::initializer_list<const char*> context, const char* message, const SourceCodeLocation& where) {

            std::unique_lock<std::mutex> lock(mutex_);

            // Fill the log infos
            LogEntry log(where);

            log.timestamp_ = std::chrono::system_clock::now();
            log.context_ = context;
            log.message_ = message;
            log.thread_id_ = std::this_thread::get_id();

            // Send the log to the appenders
            for (auto&& appender : appenders_) {

                appender->LogMessage(log);

            }

        }

    }

}