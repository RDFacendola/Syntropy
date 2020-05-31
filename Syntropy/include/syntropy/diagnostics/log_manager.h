
/// \file log_manager.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for logger classes.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/diagnostics/log_event.h"
#include "syntropy/diagnostics/auto_log_channel.h"
#include "syntropy/diagnostics/file_log_channel.h"

namespace syntropy
{
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

        /// \brief Send a log event to all channels.
        /// \return Returns a reference to this.
        LogManager& Send(const LogEvent& log_event);

        /// \brief Flush all channels.
        /// \return Returns a reference to this.
        LogManager& Flush();

    private:

        /// \brief Private constructor.
        LogManager();

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get the log manager instance.
    LogManager& GetLogManager();

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LogManager.

    inline LogManager::LogManager()
    {
        static auto global_log_ = MakeAutoLogFile("log.txt");       // Global log file which accepts all log events. #TODO Use data-driven log file name.
    }

    inline LogManager& LogManager::GetSingleton()
    {
        static auto singleton = LogManager{};

        return singleton;
    }

    inline LogManager& LogManager::Send(const LogEvent& log_event)
    {
        AutoLogChannel::ForEach([&log_event](auto& log_channel)
        {
            log_channel.Send(log_event);
        });

        return *this;
    }

    inline LogManager& LogManager::Flush()
    {
        AutoLogChannel::ForEach([&](auto& log_channel)
        {
            log_channel.Flush();
        });

        return *this;
    }

    // Non-member functions.

    inline LogManager& GetLogManager()
    {
        return LogManager::GetSingleton();
    }

}
