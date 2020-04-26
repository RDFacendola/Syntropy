/// \file log_channels.h
/// \brief This header is part of the syntropy diagnostic system. It contains the definition of some basic log channels.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/types/string.h"
#include <ostream>
#include <fstream>
#include "syntropy/containers/vector.h"
#include <functional>

#include "syntropy/containers/context.h"

#include "syntropy/diagnostics/log.h"
#include "syntropy/diagnostics/diagnostics.h"

namespace syntropy::diagnostics
{
    /// \brief Channel used to redirect formatted log messages to an output stream.
    /// \author Raffaele D. Facendola - December 2016
    class StreamLogChannel : public LogChannel
    {
    public:

        // Tokens for the format string

        static const String kTimeToken;                ///< \brief {time} Output the time of the day of the log message in the format hh:mm:ss.ms
        static const String kDateToken;                ///< \brief {date} Output the date of the log message in the format YYYY-MM-DD
        static const String kSeverityToken;            ///< \brief {severity} Output the severity of the log message.
        static const String kThreadToken;              ///< \brief {thread} Output the thread that caused the log message.
        static const String kContextsToken;            ///< \brief {context} Output the list of contexts the channel reacted to in the format Context1, ..., ContextN
        static const String kStackTraceToken;          ///< \brief {trace} Output the stack trace that caused the log message if available, output the last function name otherwise.
        static const String kFunctionToken;            ///< \brief {function} Output the function that caused the log message.
        static const String kMessageToken;             ///< \brief {message} Output the actual log message text.

        static const char kTokenStart;                      ///< \brief Character delimiting the begin of a token. '{'.
        static const char kTokenEnd;                        ///< \brief Character delimiting the end of a token. '}'.

        /// \brief Create a new stream log channel.
        /// \param format Format of the messages. Example of a valid format string: "{date} {time} [{context}]: {message}". Unrecognized tokens are considered plain strings.
        /// \param contexts Contexts the channel should react to.
        /// \param verbosity Minimum required severity for which a message is processed.
        StreamLogChannel(const String& format, Vector<Context> contexts, Severity verbosity);

        /// \brief Virtual destructor.
        virtual ~StreamLogChannel() = default;

    protected:

        /// \brief Get the output stream any incoming message will be appended to.
        /// \brief Return the output stream any incoming message will be appended to.
        virtual std::ostream& GetStream() = 0;

    private:

        virtual void OnSendMessage(const LogMessage& log, const Vector<Context>& contexts) override;

        virtual void Flush() override;

        /// \brief Arguments passed to the format thunk.
        struct ThunkArgs
        {
            std::ostream& out_;                         ///< \brief Output stream.
            const LogMessage& log_;                     ///< \brief Log message to append.
            const Vector<Context>& contexts_;      ///< \brief Filtered log message contexts (intersection between the channel contexts and the log message contexts).
        };

        using Thunk = std::function<void(ThunkArgs)>;

        /// \brief Update the thunk list according to the provided format string.
        void UpdateThunks(const String& format);

        /// \brief Get a thunk wrapping the function call associated to a token.
        Thunk GetTokenThunk(const String& token);

        Vector<Thunk> thunks_;                 ///< \brief List of functions used to output a formatted log message to the stream.
    };

    /// \brief Channel used to redirect formatted log messages to a file.
    /// \author Raffaele D. Facendola - May 2017
    class FileLogChannel : public StreamLogChannel
    {
    public:

        /// \brief Create a new file log channel.
        /// \param file Name of the file the output will be redirected to.
        /// \param format Format of the messages. Example of a valid format string: "{date} {time} [{context}]: {message}". Unrecognized tokens are considered plain strings.
        /// \param contexts Contexts the channel should react to.
        /// \param verbosity Minimum required severity for which a message is processed.
        FileLogChannel(const String& file, const String& format, Vector<Context> contexts, Severity verbosity);

        /// \brief Default move constructor.
        FileLogChannel(FileLogChannel&&) = default;

        /// \brief Virtual destructor.
        virtual ~FileLogChannel();

    private:

        virtual std::ostream& GetStream() override;

        std::ofstream file_stream_;             ///< \brief Stream associated to the file stream.

    };
}
