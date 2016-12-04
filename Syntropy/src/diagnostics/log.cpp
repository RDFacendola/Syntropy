#include "diagnostics/log.h"

#include <algorithm>

namespace syntropy {

    namespace diagnostics {

        //////////////// LOG MESSAGGE ////////////////

        LogMessage::LogMessage(std::initializer_list<Context> contexts, const StackTrace& stacktrace, Severity severity)
            : Event(contexts, stacktrace, severity)
        {}

        //////////////// BASE LOG APPENDER ////////////////

        BaseLogAppender::BaseLogAppender()
            : verbosity_(Severity::kInformative)
        {

        }

        void BaseLogAppender::SendMessage(const LogMessage& log)
        {
            if (log.severity_ >= verbosity_ &&
                std::any_of(log.contexts_.begin(), 
                            log.contexts_.end(), 
                            [this](const Context& message_context) 
                            { 
                                return std::any_of(contexts_.begin(), 
                                                   contexts_.end(), 
                                                   [message_context](const Context& appender_context)
                                                   {
                                                        return appender_context.Contains(message_context);
                                                   });
                            }))
            {
                OnSendMessage(log);
            }
        }

        void BaseLogAppender::SetVerbosity(Severity verbosity)
        {
            verbosity_ = verbosity;
        }

        Severity BaseLogAppender::GetVerbosity() const
        {
            return verbosity_;
        }

        void BaseLogAppender::ObserveContext(std::initializer_list<Context> contexts)
        {
            contexts_.insert(contexts_.end(),
                             contexts.begin(),
                             contexts.end());
        }

        void BaseLogAppender::IgnoreContext(const Context& context)
        {
            contexts_.erase(std::remove(contexts_.begin(),
                                        contexts_.end(),
                                        context),
                            contexts_.end());
        }

        //////////////// LOG MANAGER ////////////////

        LogManager& LogManager::GetInstance() {

            static LogManager instance;

            return instance;

        }

        //////////////// LOG MANAGER :: MESSAGE BUILDER ////////////////

        void LogManager::MessageBuilder::AcquireStream()
        {
            auto& pool = GetPool();

            std::unique_lock<std::mutex> lock(GetMutex());

            if (pool.size() > 0)
            {
                // Recycle an existing stream from the pool
                stream_ = std::move(pool[pool.size() - 1]);
                pool.pop_back();
            }
            else
            {
                // Create a new stream
                stream_ = std::make_unique<std::ostringstream>();
            }
        }

        void LogManager::MessageBuilder::ReleaseStream()
        {
            stream_->clear();
            stream_->str(std::string());

            std::unique_lock<std::mutex> lock(GetMutex());

            GetPool().emplace_back(std::move(stream_));         // Send the stream to the pool
        }

        void LogManager::AttachAppender(std::shared_ptr<BaseLogAppender> appender)
        {
            std::unique_lock<std::mutex> lock(mutex_);

            if (std::find(appenders_.begin(),
                          appenders_.end(),
                          appender) == appenders_.end())
            {
                appenders_.push_back(appender);                 // Avoids duplicated appenders
            }
        }

        void LogManager::DetachAppender(std::shared_ptr<BaseLogAppender> appender)
        {
            std::unique_lock<std::mutex> lock(mutex_);

            appenders_.erase(std::remove(appenders_.begin(),
                                         appenders_.end(),
                                         appender),
                             appenders_.end());
        }

        void LogManager::MessageBuilder::Append()
        {
            // Append nothing
        }

        LogManager::MessageBuilder::operator const char*() const
        {
            return message_.c_str();
        }

        std::mutex& LogManager::MessageBuilder::GetMutex()
        {
            static std::mutex mutex;
            return mutex;
        }

        std::vector<std::unique_ptr<std::ostringstream>>& LogManager::MessageBuilder::GetPool()
        {
            static std::vector<std::unique_ptr<std::ostringstream>> pool;
            return pool;
        }

    }
    
}