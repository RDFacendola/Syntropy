#include "diagnostics/log.h"

namespace syntropy {

    namespace diagnostics {

        //////////////// LOG MESSAGGE ////////////////

        LogMessage::LogMessage(std::initializer_list<Context> contexts, const Callstack& callstack, Severity severity)
            : Event(contexts, callstack, severity)
        {}

        //////////////// LOG MANAGER ////////////////

        LogManager& LogManager::GetInstance() {

            static LogManager instance;

            return instance;

        }

        //////////////// LOG MESSAGE BUILDER ////////////////

        LogMessageBuilder::LogMessageBuilder()
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

        LogMessageBuilder::~LogMessageBuilder()
        {
            std::unique_lock<std::mutex> lock(GetMutex());

            // Send the stream to the pool
            stream_->clear();
            stream_->str(std::string());
            GetPool().push_back(std::move(stream_));
        }

        std::ostringstream& LogMessageBuilder::operator*() const
        {
            return *stream_;
        }

        std::mutex& LogMessageBuilder::GetMutex()
        {
            static std::mutex mutex;
            return mutex;
        }

        std::vector<std::unique_ptr<std::ostringstream>>& LogMessageBuilder::GetPool()
        {
            static std::vector<std::unique_ptr<std::ostringstream>> pool;
            return pool;
        }

    }
    
}