/// \file log_channels.h
/// \brief This header is part of the syntropy diagnostic system. It contains the definition of some basic log channels.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>
#include <ostream>
#include <fstream>
#include <vector>
#include <functional>

#include "diagnostics.h"
#include "log.h"

namespace syntropy
{
    namespace diagnostics
    {

        /// \brief Channel used to redirect formatted log messages to an output stream.
        /// \author Raffaele D. Facendola - December 2016
        class StreamLogChannel : public LogChannel
        {
        public:

            // Tokens for the format string

            static const std::string kTimeToken;                ///< \brief {time} Output the time of the day of the log message in the format hh:mm:ss.ms
            static const std::string kDateToken;                ///< \brief {date} Output the date of the log message in the format YYYY-MM-DD
            static const std::string kSeverityToken;            ///< \brief {severity} Output the severity of the log message.
            static const std::string kThreadToken;              ///< \brief {thread} Output the thread that caused the log message.
            static const std::string kContextsToken;            ///< \brief {context} Output the list of contexts the channel reacted to in the format Context1, ..., ContextN
            static const std::string kStackTraceToken;          ///< \brief {trace} Output the stack trace that caused the log message if available, output the last function name otherwise.
            static const std::string kFunctionToken;            ///< \brief {function} Output the function that caused the log message.
            static const std::string kMessageToken;             ///< \brief {message} Output the actual log message text.

            static const char kTokenStart;                      ///< \brief Character delimiting the begin of a token. '{'.
            static const char kTokenEnd;                        ///< \brief Character delimiting the end of a token. '}'.

                                                                /// \brief Configuration for a StreamLogChannel.
            struct Configuration
            {
                std::ostream& stream_;              ///< \brief Stream where the messages will be redirected to.
                std::string format_;                ///< \brief Format of the messages. Example of a valid format string: "{date} {time} [{context}]: {message}". Unrecognized tokens are considered plain strings.
                std::vector<Context> contexts_;     ///< \brief Contexts the channel should react to.
                Severity verbosity_;                ///< \brief Minimum required severity for which a message is processed.
                Severity flush_severity_;           ///< \brief Minimum severity required for a stream flush. Used to prevent log message loss after an application crash.
            };

            /// \brief Create a new stream log channel.
            /// \param configuration Configuration for this channel.
            StreamLogChannel(const Configuration& configuration);

            /// \brief Virtual destructor.
            virtual ~StreamLogChannel() = default;

        private:

            virtual void OnSendMessage(const LogMessage& log, const std::vector<Context>& contexts) override;

            /// \brief Arguments passed to the format thunk.
            struct ThunkArgs
            {
                std::ostream& out_;                         ///< \brief Output stream.
                const LogMessage& log_;                     ///< \brief Log message to append.
                const std::vector<Context>& contexts_;      ///< \brief Filtered log message contexts (intersection between the channel contexts and the log message contexts).
            };

            using Thunk = std::function<void(ThunkArgs)>;

            /// \brief Update the thunk list according to the provided format string.
            void UpdateThunks(const std::string& format);

            /// \brief Get a thunk wrapping the function call associated to a token.
            Thunk GetTokenThunk(const std::string& token);

            std::ostream& stream_;                      ///< \brief Stream where the messages are appended to.

            std::vector<Thunk> thunks_;                 ///< \brief List of functions used to output a formatted log message to the stream.

            Severity flush_severity_;                   ///< \brief Minimum severity required in order to trigger a stream flush.
        };

        /// \brief Channel used to redirect formatted log messages to a file.
        /// \author Raffaele D. Facendola - May 2017
        class FileLogChannel : public StreamLogChannel
        {
        public:

            /// \brief Configuration for a StreamLogChannel.
            struct Configuration
            {
                std::string file_;                  ///< \brief Name of the file the output will be redirected to.
                std::string format_;                ///< \brief Format of the messages. Example of a valid format string: "{date} {time} [{context}]: {message}". Unrecognized tokens are considered plain strings.
                std::vector<Context> contexts_;     ///< \brief Contexts the channel should react to.
                Severity verbosity_;                ///< \brief Minimum required severity for which a message is processed.
                Severity flush_severity_;           ///< \brief Minimum severity required for a stream flush. Used to prevent log message loss after an application crash.
            };

            /// \brief Create a new file log channel.
            /// \param configuration Configuration for this channel.
            FileLogChannel(const Configuration& configuration);

            /// \brief Virtual destructor.
            virtual ~FileLogChannel();

        private:

            std::ofstream file_stream_;             ///< \brief Stream associated to the file stream.

        };

    }
}
