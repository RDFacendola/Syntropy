/// \file log.h
/// \brief This header is part of the syntropy diagnostic system. It contains logging classes and functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/types/sstream.h"
#include "syntropy/containers/vector.h"
#include "syntropy/types/string.h"
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

            Vector<std::unique_ptr<LogChannel>> channels_;             /// \brief List of log channels.
        };

        /// \brief Get a reference to the LogManager singleton.
        LogManager& GetLogManager();

        /// \brief Import a log manager configuration from a JSON file.
        /// Existing log channels are preserved: avoid importing the same file more than once.
        /// \param Path of the file containing the configuration.
        /// \return Returns true if the configuration could be imported successfully, returns false otherwise.
        bool ImportLogConfigurationFromJSON(const String& path);

    }
}

namespace syntropy
{
    namespace diagnostics
    {

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

