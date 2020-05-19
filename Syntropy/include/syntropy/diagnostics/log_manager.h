
/// \file log_manager.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for logger classes.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <algorithm>
#include <mutex>

#include "syntropy/language/macro.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/allocators/memory_context.h"

#include "syntropy/diagnostics/severity.h"
#include "syntropy/diagnostics/verbosity.h"
#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/diagnostics/debugger.h"
#include "syntropy/diagnostics/log_channel.h"
#include "syntropy/diagnostics/log_event.h"

#include "syntropy/containers/vector.h"

namespace syntropy
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    /// \brief Log a debug message.
    /// \usage SYNTROPY_DEBUG(context, "This is the number: ", 2, "!");
    #define SYNTROPY_DEBUG(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log an informative message.
    /// \usage SYNTROPY_INFO(context, "This is the number: ", 2, "!");
    #define SYNTROPY_INFO(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log a warning message.
    /// \usage SYNTROPY_WARNING(context, "This is the number: ", 2, "!");
    #define SYNTROPY_WARNING(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log an error message, causing the debugger to break.
    /// \usage SYNTROPY_ERROR(context, "This is the number: ", 2, "!");
    #define SYNTROPY_ERROR(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log a critical error message with full stack trace and causing the debugger to break.
    /// \usage SYNTROPY_CRITICAL(context, "This is the number: ", 2, "!");
    #define SYNTROPY_CRITICAL(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log a fatal error message with full stack trace causing the debugger to break and the application to crash.
    /// \usage SYNTROPY_FATAL(context, "This is the number: ", 2, "!");
    #define SYNTROPY_FATAL(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /************************************************************************/
    /* LOG MANAGER                                                          */
    /************************************************************************/

    /// \brief Singleton class used to send log events.
    /// \author Raffaele D. Facendola - November 2016.
    class LogManager
    {
    public:

        /// \brief Get the singleton instance.
        static LogManager& GetSingleton();

        /// \brief Create a new log channel.
        template <typename TLogChannel, typename... TArguments>
        void CreateChannel(Verbosity verbosity, Vector<Context> contexts, TArguments&&... arguments);

        /// \brief Send a log event to all channels.
        /// \return Returns a reference to this.
        LogManager& Send(const LogEvent& log_event);

        /// \brief Flush all channels.
        void Flush();

    private:

        /// \brief Private default constructor.
        LogManager(MemoryResource& memory_resource);

        /// \brief Used to synchronize logging threads. Recursive since channel creation may log.
        std::recursive_mutex mutex_;

        /// \brief Underlying memory resource.
        MemoryResource* memory_resource_ = &GetDefaultMemoryResource();

        /// \brief Log channels.
        Vector<UniquePtr<LogChannel>> log_channels_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get the log manager instance.
    LogManager& GetLogManager();

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Macros.

    #undef  SYNTROPY_DEBUG
    #define SYNTROPY_DEBUG(context, ...) \
        syntropy::GetLogManager().Send(syntropy::MakeLogEvent(syntropy::Severity::kDebug, context, SYNTROPY_HERE, __VA_ARGS__))

    #undef  SYNTROPY_INFO
    #define SYNTROPY_INFO(context, ...) \
        syntropy::GetLogManager().Send(syntropy::MakeLogEvent(syntropy::Severity::kInformative, context, SYNTROPY_HERE, __VA_ARGS__))

    #undef  SYNTROPY_WARNING
    #define SYNTROPY_WARNING(context, ...) \
        syntropy::GetLogManager().Send(syntropy::MakeLogEvent(syntropy::Severity::kWarning, context, SYNTROPY_HERE, __VA_ARGS__))

    #undef  SYNTROPY_ERROR
    #define SYNTROPY_ERROR(context, ...) \
        { \
            syntropy::GetLogManager().Send(syntropy::MakeLogEvent(syntropy::Severity::kError, context, SYNTROPY_HERE, __VA_ARGS__)) \
            SYNTROPY_BREAK; \
        }

    #undef  SYNTROPY_CRITICAL
    #define SYNTROPY_CRITICAL(context, ...) \
        { \
            syntropy::GetLogManager().Send(syntropy::MakeLogEvent(syntropy::Severity::kCritical, context, syntropy::Debugger::GetStackTrace( SYNTROPY_HERE ), __VA_ARGS__)).Flush(); \
            SYNTROPY_BREAK; \
        }

    #undef  SYNTROPY_FATAL
    #define SYNTROPY_FATAL(context, ...) \
        { \
            syntropy::GetLogManager().Send(syntropy::MakeLogEvent(syntropy::Severity::kFatal, context, syntropy::Debugger::GetStackTrace( SYNTROPY_HERE ), __VA_ARGS__)).Flush(); \
            SYNTROPY_TRAP; \
        }

    // LogManager.

    inline LogManager& LogManager::GetSingleton()
    {
        static auto singleton = LogManager{GetSystemMemoryResource()};

        return singleton;
    }

    LogManager::LogManager(MemoryResource& memory_resource)
        : memory_resource_(&memory_resource)
        , log_channels_(memory_resource)
    {

    }

    template <typename TLogChannel, typename... TArguments>
    void LogManager::CreateChannel(Verbosity verbosity, Vector<Context> contexts, TArguments&&... arguments)
    {
        auto memory_context = MemoryContext(*memory_resource_);

        auto log_channel = MakeUnique<LogChannelT<TLogChannel>>(verbosity, contexts, std::forward<TArguments>(arguments)...);

        log_channels_.emplace_back(std::move(log_channel));
    }

    inline LogManager& LogManager::Send(const LogEvent& log_event)
    {
        auto lock = std::unique_lock<std::recursive_mutex>(mutex_);

        for (auto&& log_channel : log_channels_)
        {
            log_channel->Send(log_event);
        }

        return *this;
    }

    inline void LogManager::Flush()
    {
        auto lock = std::unique_lock<std::recursive_mutex>(mutex_);

        for (auto&& log_channel : log_channels_)
        {
            log_channel->Flush();
        }
    }

    // Non-member functions.

    inline LogManager& GetLogManager()
    {
        return LogManager::GetSingleton();
    }

}
