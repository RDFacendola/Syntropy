#include "diagnostics/log.h"

#include <algorithm>
#include "algorithm.h"
#include "date.h"

namespace syntropy {

    namespace diagnostics {

        //////////////// LOG MESSAGE ////////////////

        LogMessage::LogMessage()
            : time_(std::chrono::system_clock::now())
            , thread_id_(std::this_thread::get_id())
        {}
        
        //////////////// LOG STREAM ////////////////

        LogStream::LogStream()
            : verbosity_(Severity::kInformative)
        {

        }

        LogStream& LogStream::operator<<(const LogMessage& log)
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

            return *this;
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
            contexts_.insert(contexts);
        }

        void LogStream::UnbindContext(const Context& context)
        {
            contexts_.erase(context);
        }

        const std::set<Context>& LogStream::GetBoundContexts() const
        {
            return contexts_;
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

        const std::string& LogManager::MessageBuilder::GetMessage() const
        {
            return message_;
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

        //////////////// STREAM LOGGER ////////////////

        StreamLogger::StreamLogger(std::ostream& stream, std::string format, Severity flush_severity)
            : stream_(stream)
            , format_(std::move(format))
            , flush_severity_(flush_severity)
        {

        }

        void StreamLogger::OnSendMessage(const LogMessage& log)
        {
            auto it = format_.begin();

            while (it != format_.end())
            {
                auto token = GetToken(it, format_.end(), '{', '}');

                // Add a constant string
                while (it != token.first)
                {
                    stream_ << *it++;
                }

                // Translate via token
                if (token.first != token.second)
                {
                    OutputByToken(std::string(token.first, token.second), log);
                }

                // Next
                it = token.second;
            }

            stream_ << "\n";

            if (log.severity_ >= flush_severity_)
            {
                stream_.flush();            // Ensures that no log is lost if the application crashed before the stream gets flushed.
            }
        }

        void StreamLogger::OutputByToken(const std::string& token, const LogMessage& log)
        {
            if (token == "{time}")
            {
                stream_ << GetTimeOfDay(log.time_);
            }
            else if (token == "{date}")
            {
                stream_ << GetDate(log.time_);
            }
            else if (token == "{severity}")
            {
                stream_ << log.severity_;
            }
            else if (token == "{threadid}")
            {
                stream_ << log.thread_id_;
            }
            else if (token == "{contexts}")
            {
                stream_ << log.contexts_;
            }
            else if (token == "{stacktrace}")
            {
                stream_ << log.stacktrace_;
            }
            else if (token == "{function}")
            {
                stream_ << log.stacktrace_.elements_[0];
            }
            else if (token == "{message}")
            {
                stream_ << log.message_;
            }
            else
            {
                stream_ << token;
            }
        }

    }
    
}