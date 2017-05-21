/// \file log_manager.h
/// \brief This header is part of the syntropy diagnostic system. It contains classes used to handle logs and log channels.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <memory>

#include "diagnostics/diagnostics.h"

#include "reflection/class.h"

namespace syntropy
{
    namespace diagnostics
    {

        /// \brief Represents a single log message.
        /// \author Raffaele D. Facendola - November 2016
        struct LogMessage
        {
            /// \brief Create a new log message.
            /// \param stacktrace Stacktrace that caused the log. Can be either a full stack trace or the last function.
            /// \param contexts Log contexts used to categorize the log message.
            /// \param severity Severity of the log message.
            /// \param message Actual log message.
            LogMessage(StackTrace stacktrace, std::vector<Context> contexts, Severity severity, std::string message);

            std::chrono::system_clock::time_point time_;                    ///< \brief Time point associated to the message creation.
            Severity severity_;                                             ///< \brief Severity of the message.
            std::thread::id thread_id_;                                     ///< \brief Id of the thread that issued the message.
            std::vector<Context> contexts_;                                 ///< \brief Contexts used to categorize the message.
            StackTrace stacktrace_;                                         ///< \brief Stack trace.
            std::string message_;                                           ///< \brief Log message.
        };

        /// \brief Channel used to collect incoming log messages.
        /// \author Raffaele D. Facendola - November 2016
        class LogChannel
        {

        public:

            /// \brief Create a new channel.
            /// \param context Contexts bound to this channel. Used to filter log messages by context.
            /// \param verbosity Minimum required severity for which a message is processed.
            LogChannel(std::vector<Context> contexts, Severity verbosity = Severity::kInformative);

            /// \brief Virtual destructor.
            virtual ~LogChannel() = default;

            /// \brief Send a message to the channel.
            /// If the message context or verbosity do not match any of the ones specified by this channel, the message is ignored.
            /// \param log Message to send.
            LogChannel& operator<<(const LogMessage& log);

            /// \brief Get the verbosity level of the channel.
            /// This represents the minimum required severity for which a message is processed.
            /// \return Returns the verbosity level of the channel.
            Severity GetVerbosity() const;

            /// \brief Get the contexts this channel reacts to.
            /// Log messages must specify at least one of these contexts in order to be processed. Note that contexts are hierarchical.
            /// \return Returns the list of contexts this channel reacts to.
            const std::vector<Context>& GetContexts() const;

            /// \brief Flush any cached state.
            virtual void Flush() = 0;

        protected:

            /// \brief Handle a message.
            /// \param log Message to handle. The message is guaranteed to have severity equal or higher to the verbosity level and have at least one context matching at least one among the contexts bound to the stream. 
            /// \param contexts List of contexts specified by the log message that match any of the channel contexts.
            virtual void OnSendMessage(const LogMessage& log, const std::vector<Context>& contexts) = 0;

        private:

            std::vector<Context> contexts_;                 ///< \brief Contexts this channel reacts to.

            Severity verbosity_;                            ///< \brief Minimum severity needed to process a message.
        };

        /// \brief Singleton used to issue log messages and events.
        /// \author Raffaele D. Facendola - November 2016
        class LogManager
        {

        public:

            /// \brief Get the log manager instance.
            static LogManager& GetInstance();

            /// \brief Import a new configuration for the log manager.
            /// \param path Path of the file containing the configuration.
            void ImportConfiguration(const std::string& path);

            /// \brief Create new log channel.
            /// \tparam TLogChannel Type of channel to create. Must derive from LogChannel.
            /// \param args Arguments passed to the channel's constructor.
            /// \return Returns a reference to the new channel.
            template <typename TLogChannel, typename... TArgs>
            const TLogChannel& CreateChannel(TArgs&&... args);

            /// \brief Send a log message.
            /// \param log_message Log message to send.
            void Send(const LogMessage& log_message);

        private:

            /// \brief Flush any cached state.
            void Flush();

            /// \brief Prevents direct instantiation.
            LogManager() = default;

            std::recursive_mutex mutex_;                                    ///< \brief Used to synchronize various logging threads. Recursive because channel creation may cause a log.

            std::vector<std::unique_ptr<LogChannel>> channels_;             /// \brief List of log channels.
        };

    }

    namespace reflection
    {

        extern const Class& ClassOf_LogChannel;

        // Reflection specialization for LogChannel.
        template <>
        struct ClassDeclaration<diagnostics::LogChannel>
        {
            static constexpr const char* GetName() noexcept
            {
                return "diagnostics::LogChannel";
            }
        };

    }

}

namespace syntropy
{
    namespace diagnostics
    {

        /************************************************************************/
        /* LOG MANAGER                                                          */
        /************************************************************************/

        template <typename TLogStream, typename... TArgs>
        const TLogStream& LogManager::CreateChannel(TArgs&&... args)
        {
            std::unique_lock<std::recursive_mutex> lock(mutex_);

            channels_.emplace_back(std::make_unique<TLogStream>(std::forward<TArgs>(args)...));

            return *static_cast<TLogStream*>(channels_.back().get());
        }

    }
}
