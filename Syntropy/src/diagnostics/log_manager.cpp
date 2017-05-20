#include "diagnostics/log_manager.h"

namespace syntropy
{
    namespace diagnostics
    {

        /************************************************************************/
        /* LOG MESSAGE                                                          */
        /************************************************************************/

        LogMessage::LogMessage(StackTrace stacktrace, std::vector<Context> contexts, Severity severity, std::string message)
            : time_(std::chrono::system_clock::now())
            , severity_(severity)
            , thread_id_(std::this_thread::get_id())
            , contexts_(std::move(contexts))
            , stacktrace_(std::move(stacktrace))
            , message_(std::move(message))
        {

        }

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

        void LogManager::ImportConfiguration(const std::string& /*path*/)
        {
            // Read the file inside the JSON object.

            //std::ifstream file(path);

            //nlohmann::json json;

            //file >> json;

            // Deserialize the channel list.

            //if (!serialization::DeserializeObjectFromJSON(channels_, json))
            //{
            //    SYNTROPY_ERROR((DiagnosticsCtx), "Unable to import log configuration from file '", path, "'.");
            //}

        }

        void LogManager::Send(const LogMessage& log_message)
        {
            std::unique_lock<std::recursive_mutex> lock(mutex_);

            for (auto&& channel : channels_)
            {
                *channel << log_message;
            }
        }

    }
}