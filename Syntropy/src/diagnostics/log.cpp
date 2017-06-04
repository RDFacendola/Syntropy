/// \file log.cpp
/// The point of this file is to break the dependency of the log macros from the log manager.
/// Most of the code may want to use the logging macros without ever knowing about the manager.

#include "diagnostics/log.h"

#include "diagnostics/diagnostics_meta.h"

namespace syntropy 
{
    namespace diagnostics
    {

        /************************************************************************/
        /* LOG CHANNEL                                                          */
        /************************************************************************/

        LogChannel::LogChannel(std::vector<Context> contexts, Severity verbosity)
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
                std::vector<Context> contexts;

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

        const std::vector<Context>& LogChannel::GetContexts() const
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

        bool LogManager::ImportConfiguration(const std::string& path)
        {
            // Read the file inside the JSON object.

            std::ifstream file(path);

            nlohmann::json json;

            file >> json;

            //Deserialize the channel list.

            auto channels = serialization::DeserializeObjectFromJSON<decltype(channels_)>(json);

            if (channels && !channels->empty())
            {
                channels_ = std::move(*channels);
                return true;
            }

            return false;
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
}