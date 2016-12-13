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
            if (Match(log))
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

        std::set<Context> LogStream::GetHitContexts(const LogMessage& log) const
        {
            std::set<Context> contexts;

            for (auto&& log_context : log.contexts_)
            {
                for (auto&& context : GetBoundContexts())
                {
                    if (context.Contains(log_context))
                    {
                        contexts.emplace(log_context);
                    }
                }
            }

            return contexts;
        }

        const std::set<Context>& LogStream::GetBoundContexts() const
        {
            return contexts_;
        }

        bool LogStream::Match(const LogMessage& log) const
        {
            return log.severity_ >= verbosity_ &&
                   std::any_of(std::begin(log.contexts_),
                               std::end(log.contexts_),
                               [this](const Context& log_context)
                               {
                                   return std::any_of(std::begin(contexts_),
                                                      std::end(contexts_),
                                                      [&log_context](const Context& context)
                                                      {
                                                           return context.Contains(log_context);
                                                      });
                               });
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

        const std::string StreamLogger::kTimeToken("{time}");
        const std::string StreamLogger::kDateToken("{date}");
        const std::string StreamLogger::kSeverityToken("{severity}");
        const std::string StreamLogger::kThreadToken("{thread}");
        const std::string StreamLogger::kContextsToken("{context}");
        const std::string StreamLogger::kStackTraceToken("{trace}");
        const std::string StreamLogger::kFunctionToken("{function}");
        const std::string StreamLogger::kMessageToken("{message}");

        const char StreamLogger::kTokenStart = '{';
        const char StreamLogger::kTokenEnd = '}';

        StreamLogger::StreamLogger(std::ostream& stream, const std::string& format, Severity flush_severity)
            : stream_(stream)
            , flush_severity_(flush_severity)
        {
            UpdateThunks(format);
        }

        void StreamLogger::UpdateThunks(const std::string& format)
        {
            thunks_.clear();

            std::pair<std::string::const_iterator, std::string::const_iterator> token;

            for (auto it = format.cbegin(); it != format.cend(); it = token.second)
            {
                token = GetToken(it, format.cend(), kTokenStart, kTokenEnd);

                // Add a constant string
                if (it != token.first)
                {
                    std::string message(it, token.first);
                    thunks_.emplace_back([message](std::ostream& out, const LogMessage&) { out << message; });
                }

                // Translate via token
                if (token.first != token.second)
                {
                    thunks_.emplace_back(GetTokenThunk(std::string(token.first, token.second)));
                }
            }
        }

        void StreamLogger::OnSendMessage(const LogMessage& log)
        {
            for (auto&& thunk : thunks_)
            {
                thunk(stream_, log);
            }

            if (thunks_.size() > 0)
            {
                stream_ << "\n";

                // Ensures that no log is lost if the application crashed before the stream gets flushed.
                if (log.severity_ >= flush_severity_)
                {
                    stream_.flush();
                }
            }
        }

        StreamLogger::Thunk StreamLogger::GetTokenThunk(const std::string& token)
        {
            // Match each token with the proper thunk
            if (token == kTimeToken)
            {
                return [](std::ostream& out, const LogMessage& log) { out << GetTimeOfDay(log.time_); };
            }
            else if (token == kDateToken)
            {
                return [](std::ostream& out, const LogMessage& log) { out << GetDate(log.time_); };
            }
            else if (token == kSeverityToken)
            {
                return [](std::ostream& out, const LogMessage& log) { out << log.severity_; };
            }
            else if (token == kThreadToken)
            {
                return [](std::ostream& out, const LogMessage& log) { out << log.thread_id_; };
            }
            else if (token == kContextsToken)
            {
                return [this](std::ostream& out, const LogMessage& log) { out << GetHitContexts(log); };
            }
            else if (token == kStackTraceToken)
            {
                return [](std::ostream& out, const LogMessage& log) { out << log.stacktrace_; };
            }
            else if (token == kFunctionToken)
            {
                return [](std::ostream& out, const LogMessage& log) { out << log.stacktrace_.elements_[0]; };
            }
            else if (token == kMessageToken)
            {
                return [](std::ostream& out, const LogMessage& log) { out << log.message_;  };
            }
            
            return [token](std::ostream& out, const LogMessage&) { out << token; };
        }

    }
}