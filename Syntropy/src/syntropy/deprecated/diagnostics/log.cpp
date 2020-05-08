/// \file log.cpp
/// The point of this file is to break the dependency of the log macros from the log manager.
/// Most of the code may want to use the logging macros without ever knowing about the manager.

#include "syntropy/diagnostics/log.h"

#include "syntropy/diagnostics/diagnostics_meta.h"

namespace syntropy::diagnostics
{
    /************************************************************************/
    /* LOG MANAGER                                                          */
    /************************************************************************/

    LogManager& LogManager::GetInstance()
    {
        static LogManager instance;

        return instance;
    }

    void LogManager::Send(const LogMessage& log_message)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_);

        for (auto&& channel : channels_)
        {
            *channel << log_message;
        }

        // Errors and more severe logs usually anticipate an application crash:
        // flushing the channels ensures that no log message is lost.
        if (log_message.severity_ >= Severity::kError)
        {
            Flush();
        }
    }

    void LogManager::Flush()
    {
        for (auto&& channel : channels_)
        {
            channel->Flush();
        }
    }

    LogManager& GetLogManager()
    {
        return LogManager::GetInstance();
    }
}