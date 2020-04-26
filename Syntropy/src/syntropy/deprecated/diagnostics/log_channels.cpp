#include "syntropy/diagnostics/log_channels.h"

#include "syntropy/time/time.h"

#include "syntropy/patterns/algorithm.h"

#include "syntropy/types/range.h"

namespace syntropy::diagnostics
{
    /************************************************************************/
    /* STREAM LOG CHANNEL                                                   */
    /************************************************************************/

    const String StreamLogChannel::kTimeToken("{time}");
    const String StreamLogChannel::kDateToken("{date}");
    const String StreamLogChannel::kSeverityToken("{severity}");
    const String StreamLogChannel::kThreadToken("{thread}");
    const String StreamLogChannel::kContextsToken("{context}");
    const String StreamLogChannel::kStackTraceToken("{trace}");
    const String StreamLogChannel::kFunctionToken("{function}");
    const String StreamLogChannel::kMessageToken("{message}");

    const char StreamLogChannel::kTokenStart = '{';
    const char StreamLogChannel::kTokenEnd = '}';

    StreamLogChannel::StreamLogChannel(const String& format, Vector<Context> contexts, Severity verbosity)
        : LogChannel(std::move(contexts), verbosity)
    {
        UpdateThunks(format);
    }

    void StreamLogChannel::UpdateThunks(const String& format)
    {
        thunks_.clear();

        std::pair<String::const_iterator, String::const_iterator> token;

        for (auto it = format.cbegin(); it != format.cend(); it = token.second)
        {
            token = GetToken(it, format.cend(), kTokenStart, kTokenEnd);

            // Add a constant string
            if (it != token.first)
            {
                String message(it, token.first);
                thunks_.emplace_back([message](const ThunkArgs& args) { args.out_ << message; });
            }

            // Translate via token
            if (token.first != token.second)
            {
                thunks_.emplace_back(GetTokenThunk(String(token.first, token.second)));
            }
        }
    }

    void StreamLogChannel::OnSendMessage(const LogMessage& log, const Vector<Context>& contexts)
    {
        if (thunks_.size() > 0)
        {
            auto& stream = GetStream();

            ThunkArgs args{ stream, log, contexts };

            for (auto&& thunk : thunks_)
            {
                thunk(args);
            }

            stream << "\n";
        }
    }

    void StreamLogChannel::Flush()
    {
        GetStream().flush();
    }

    StreamLogChannel::Thunk StreamLogChannel::GetTokenThunk(const String& token)
    {
        // Match each token with the proper thunk
        if (token == kTimeToken)
        {
            return [](const ThunkArgs& args) { args.out_ << GetTimeOfDay<>(args.log_.time_); };
        }
        else if (token == kDateToken)
        {
            return [](const ThunkArgs& args) { args.out_ << GetDate(args.log_.time_); };
        }
        else if (token == kSeverityToken)
        {
            return [](const ThunkArgs& args) { args.out_ << args.log_.severity_; };
        }
        else if (token == kThreadToken)
        {
            return [](const ThunkArgs& args) { args.out_ << args.log_.thread_id_; };
        }
        else if (token == kContextsToken)
        {
            return [](const ThunkArgs& args) { args.out_ << MakeRange(args.contexts_.begin(), args.contexts_.end()); };
        }
        else if (token == kStackTraceToken)
        {
            return [](const ThunkArgs& args) { args.out_ << args.log_.stacktrace_; };
        }
        else if (token == kFunctionToken)
        {
            return [](const ThunkArgs& args) { args.out_ << args.log_.stacktrace_.elements_[0]; };
        }
        else if (token == kMessageToken)
        {
            return [](const ThunkArgs& args) { args.out_ << args.log_.message_; };
        }

        return [token](const ThunkArgs& args) { args.out_ << token; };
    }

    /************************************************************************/
    /* FILE LOG CHANNEL                                                     */
    /************************************************************************/

    FileLogChannel::FileLogChannel(const String& file, const String& format, Vector<Context> contexts, Severity verbosity)
        : StreamLogChannel(format, std::move(contexts), verbosity)
    {
        file_stream_.open(file.c_str());
    }

    FileLogChannel::~FileLogChannel()
    {
        file_stream_.close();
    }

    std::ostream& FileLogChannel::GetStream()
    {
        return file_stream_;
    }
}