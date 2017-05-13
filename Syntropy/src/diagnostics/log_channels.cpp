#include "diagnostics/log_channels.h"

#include "algorithm.h"

#include "containers/range.h"

namespace syntropy
{
    namespace diagnostics
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

        StreamLogChannel::StreamLogChannel(const Configuration& configuration)
            : LogChannel(configuration.contexts_, configuration.verbosity_)
            , stream_(configuration.stream_)
            , flush_severity_(configuration.flush_severity_)
        {
            UpdateThunks(configuration.format_);
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
                ThunkArgs args{ stream_, log, contexts };

                for (auto&& thunk : thunks_)
                {
                    thunk(args);
                }

                stream_ << "\n";

                if (log.severity_ >= flush_severity_)
                {
                    stream_.flush();                        // Ensures that no log is lost if the application is about to crash after this call.
                }
            }
        }

        StreamLogChannel::Thunk StreamLogChannel::GetTokenThunk(const std::string& token)
        {
            // Match each token with the proper thunk
            if (token == kTimeToken)
            {
                return [](const ThunkArgs& args) { args.out_ << GetTimeOfDay(args.log_.time_); };
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

        FileLogChannel::FileLogChannel(const Configuration& configuration)
            : StreamLogChannel(StreamLogChannel::Configuration
        {
            file_stream_
            , configuration.format_
                , configuration.contexts_
                , configuration.verbosity_
                , configuration.flush_severity_
        })
        {
            file_stream_.open(configuration.file_);
        }

        FileLogChannel::~FileLogChannel()
        {
            file_stream_.close();
        }

    }
}