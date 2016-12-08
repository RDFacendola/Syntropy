#include "diagnostics/log.h"

#include <algorithm>

namespace syntropy {

    namespace diagnostics {

        //////////////// LOG MESSAGGE ////////////////

        LogMessage::LogMessage(std::initializer_list<Context> contexts, const StackTrace& stacktrace, Severity severity)
            : Event(contexts, stacktrace, severity)
        {}

        //////////////// LOG STREAM ////////////////

        LogStream::LogStream()
            : verbosity_(Severity::kInformative)
        {

        }

        void LogStream::SendMessage(const LogMessage& log)
        {
            if (log.severity_ >= verbosity_ &&
                std::any_of(log.contexts_.begin(), 
                            log.contexts_.end(), 
                            [this](const Context& message_context) 
                            { 
                                return std::any_of(contexts_.begin(), 
                                                   contexts_.end(), 
                                                   [message_context](const Context& stream_context)
                                                   {
                                                        return stream_context.Contains(message_context);
                                                   });
                            }))
            {
                OnSendMessage(log);
            }
        }

        void LogStream::SetVerbosity(Severity verbosity)
        {
            verbosity_ = verbosity;
        }

        Severity LogStream::GetVerbosity() const
        {
            return verbosity_;
        }

        void LogStream::BindContext(std::initializer_list<Context> contexts)
        {
            contexts_.insert(contexts_.end(),
                             contexts.begin(),
                             contexts.end());
        }

        void LogStream::UnbindContext(const Context& context)
        {
            contexts_.erase(std::remove(contexts_.begin(),
                                        contexts_.end(),
                                        context),
                            contexts_.end());
        }

        LogStream& operator<<(LogStream& log_stream, const LogMessage& log)
        {
            log_stream.SendMessage(log);
            return log_stream;
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

        void LogManager::AttachStream(std::shared_ptr<LogStream> stream)
        {
            std::unique_lock<std::mutex> lock(mutex_);

            if (std::find(streams_.begin(),
                          streams_.end(),
                          stream) == streams_.end())
            {
                streams_.emplace_back(std::move(stream));       // Avoid duplicated streams
            }
        }

        void LogManager::DetachStream(std::shared_ptr<LogStream> stream)
        {
            std::unique_lock<std::mutex> lock(mutex_);

            streams_.erase(std::remove(streams_.begin(),
                                       streams_.end(),
                                       stream),
                           streams_.end());
        }

        void LogManager::MessageBuilder::Append()
        {
            // Append nothing
        }

        const char* LogManager::MessageBuilder::GetMessage() const
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