/// \file log.cpp
/// The point of this file is to break the dependency of the log macros from the log manager.
/// Most of the code may want to use the logging macros without ever knowing about the manager.

#include "syntropy/diagnostics/log.h"

#include "syntropy/diagnostics/diagnostics_meta.h"

namespace syntropy::diagnostics
{
    /************************************************************************/
    /* LOG CHANNEL                                                          */
    /************************************************************************/

    LogChannel::LogChannel(Vector<Context> contexts, Severity verbosity)
        : contexts_(std::move(contexts))
        , verbosity_(verbosity)
    {

    }

    LogChannel& LogChannel::operator<<(const LogMessage& log)
    {
        // Filter by severity
        if (log.severity_ >= verbosity_)
        {

            // Filter by contexts
            Vector<Context> contexts;

            contexts.reserve(contexts_.size());

            for (auto&& log_context : log.contexts_)
            {
                for (auto&& channel_context : contexts_)
                {
                    if (channel_context.Contains(log_context))
                    {
                        // Each log context should be added at most once.
                        contexts.emplace_back(log_context);
                        break;
                    }
                }
            }

            if (!contexts.empty())
            {
                OnSendMessage(log, contexts);
            }
        }

        return *this;
    }

    Severity LogChannel::GetVerbosity() const
    {
        return verbosity_;
    }

    const Vector<Context>& LogChannel::GetContexts() const
    {
        return contexts_;
    }

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

    bool ImportLogConfigurationFromJSON(const String& path)
    {
        // Read the file inside the JSON object.

        std::ifstream file(path.c_str());

        serialization::JSON json;

        file >> json;

        //Deserialize the channel list.

        auto channels = serialization::DeserializeObjectFromJSON<Vector<std::unique_ptr<LogChannel> > >(json);

        if (channels)
        {
            for (auto&& channel : *channels)
            {
                GetLogManager().AcquireChannel<>(std::move(channel));
            }
        }

        return channels && !channels->empty();
    }
}