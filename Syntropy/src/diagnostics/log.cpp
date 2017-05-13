#include "diagnostics/log.h"

#include "date.h"

namespace syntropy 
{
    namespace diagnostics
    {

        /************************************************************************/
        /* LOG MESSAGE                                                          */
        /************************************************************************/

        LogMessage::LogMessage()
            : time_(std::chrono::system_clock::now())
            , thread_id_(std::this_thread::get_id())
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
            if (log.severity_ >= verbosity_)
            {
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

    }
}