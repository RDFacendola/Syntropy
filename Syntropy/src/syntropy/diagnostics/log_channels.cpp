#include "syntropy/diagnostics/log_channels.h"

#include "syntropy/time/calendar.h"

#include "syntropy/patterns/algorithm.h"

#include "syntropy/containers/range.h"

namespace syntropy::diagnostics
{
    /************************************************************************/
    /* STREAM LOG CHANNEL                                                   */
    /************************************************************************/

    const std::string StreamLogChannel::kTimeToken("{time}");
    const std::string StreamLogChannel::kDateToken("{date}");
    const std::string StreamLogChannel::kSeverityToken("{severity}");
    const std::string StreamLogChannel::kThreadToken("{thread}");
    const std::string StreamLogChannel::kContextsToken("{context}");
    const std::string StreamLogChannel::kStackTraceToken("{trace}");
    const std::string StreamLogChannel::kFunctionToken("{function}");
    const std::string StreamLogChannel::kMessageToken("{message}");

    const char StreamLogChannel::kTokenStart = '{';
    const char StreamLogChannel::kTokenEnd = '}';

    StreamLogChannel::StreamLogChannel(const std::string& format, std::vector<Context> contexts, Severity verbosity)
        : LogChannel(std::move(contexts), verbosity)
    {
        UpdateThunks(format);
    }

    void StreamLogChannel::UpdateThunks(const std::string& format)
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
                thunks_.emplace_back([message](const ThunkArgs& args) { args.out_ << message; });
            }

            // Translate via token
            if (token.first != token.second)
            {
                thunks_.emplace_back(GetTokenThunk(std::string(token.first, token.second)));
            }
        }
    }

    void StreamLogChannel::OnSendMessage(const LogMessage& log, const std::vector<Context>& contexts)
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

    StreamLogChannel::Thunk StreamLogChannel::GetTokenThunk(const std::string& token)
    {
        // Match each token with the proper thunk
        if (token == kTimeToken)
        {
            return [](const ThunkArgs& args) { args.out_ << Calendar::GetTimeOfDay(args.log_.time_); };
        }
        else if (token == kDateToken)
        {
            return [](const ThunkArgs& args) { args.out_ << Calendar::GetDate(args.log_.time_); };
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

    FileLogChannel::FileLogChannel(const std::string& file, const std::string& format, std::vector<Context> contexts, Severity verbosity)
        : StreamLogChannel(format, std::move(contexts), verbosity)
    {
        file_stream_.open(file);
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