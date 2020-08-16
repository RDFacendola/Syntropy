
/// \file log_event_formatter.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definitions for objects used to format log events during stream insertion.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <ostream>

#include "syntropy/diagnostics/log_event.h"
#include "syntropy/core/string.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/time/time_of_day.h"
#include "syntropy/time/date.h"

namespace Syntropy
{
    /************************************************************************/
    /* LOG EVENT FORMATTER                                                  */
    /************************************************************************/

    class LogEventManipulator;

    /// \brief Functor object used to format log events according to a format string.
    /// The format string supports the following tokens: %time, %date, %severity, %thread, %context, %trace, %function, %message, %%. Substrings that match no token are written without any change.
    /// \author Raffaele D. Facendola - May 2020.
    class LogEventFormatter
    {
    public:

        /// \brief Create a new event formatter.
        LogEventFormatter(const String& format);

        /// \brief Copy constructor.
        LogEventFormatter(const LogEventFormatter& other);

        /// \brief Default move ctor.
        LogEventFormatter(LogEventFormatter&&) = default;

        /// \brief Copy-assignment operator.
        LogEventFormatter& operator=(const LogEventFormatter& other);

        /// \brief Default move-assignment operator.
        LogEventFormatter& operator=(LogEventFormatter&&) = default;

        /// \brief Default destructor.
        ~LogEventFormatter() = default;

        /// \brief Get the manipulator used to insert a log event inside an output stream.
        LogEventManipulator operator()(const LogEvent& log_event) const;

        /// \brief Serialize a log event to a stream via this formatter.
        void Serialize(std::ostream& stream, const LogEvent& log_event) const;

    private:

        /// \brief Type of a translation function.
        using TTranslationFunction = void(*)(std::ostream&, const LogEvent&, const StringView&);

        /// \brief Represents a translation rule.
        struct TranslationRule
        {
            /// \brief Non-terminal token to translate.
            String token_;

            /// \brief Translation function.
            TTranslationFunction translation_function_;

            /// \brief Next translation rule.
            Pointer<TranslationRule> next_;
        };

        /// \brief Represents a tokenized part of the format string which can be translated to an output stream.
        struct Token
        {
            /// \brief Token to translate within the format string.
            StringView token_;

            /// \brief Translation rule.
            Pointer<TranslationRule> translation_rule_;
        };

        /// \brief Translate an unformatted token to a stream.
        static void TranslateUnformatted(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the time of day of a log event to a stream.
        static void TranslateTime(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the date of a log event to a stream.
        static void TranslateDate(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the severity of a log event to a stream.
        static void TranslateSeverity(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the thread-id of a log event to a stream.
        static void TranslateThread(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the context of a log event to a stream.
        static void TranslateContext(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the stack-trace a log event was generated from to a stream.
        static void TranslateStackTrace(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the function name a log event was generated from to a stream.
        static void TranslateFunction(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the message of a log event to a stream.
        static void TranslateMessage(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Translate the percentage token to a stream.
        static void TranslatePercentage(std::ostream& stream, const LogEvent& log_event, const StringView& token);

        /// \brief Pseudo-token associated to an unformatted string.
        static inline const auto kUnformattedToken = TranslationRule{ "", &TranslateUnformatted, nullptr };

        /// \brief Token associated to the time of the day in the format hh:mm:ss.ms.
        static inline const auto kTimeTranslationRule = TranslationRule{ "%time", &TranslateTime, nullptr };

        /// \brief Token associated to the date in the format YYYY-MM-DD.
        static inline const auto kDateToken = TranslationRule{ "%date", &TranslateDate, &kTimeTranslationRule };

        /// \brief Token associated to the severity.
        static inline const auto kSeverityToken = TranslationRule{ "%severity", &TranslateSeverity, &kDateToken };

        /// \brief Token associated to the thread the event generated from.
        static inline const auto kThreadToken = TranslationRule{ "%thread", &TranslateThread, &kSeverityToken };

        /// \brief Token associated to the context the event belongs to.
        static inline const auto kContextToken = TranslationRule{ "%context", &TranslateContext, &kThreadToken };

        /// \brief Token associated to the full stack trace, if available, or the function the log event was generated from otherwise.
        static inline const auto kStackTraceToken = TranslationRule{ "%trace", &TranslateStackTrace, &kContextToken };

        /// \brief Token associated to the function the event was generated from.
        static inline const auto kFunctionToken = TranslationRule{ "%function", &TranslateFunction, &kStackTraceToken };

        /// \brief Token associated to the log event message.
        static inline const auto kMessageToken = TranslationRule{ "%message", &TranslateMessage, &kFunctionToken };

        /// \brief Token associated to the percentage symbol, since it is used to introduce tokens.
        static inline const auto kPercentageToken = TranslationRule{ "%%", &TranslatePercentage, &kMessageToken };

        /// \brief Parse the format string and update the tokens vector accordingly.
        void Parse();

        /// \brief Format string.
        String format_;

        /// \brief Token list.
        Vector<Token> tokens_;

    };

    /************************************************************************/
    /* LOG EVENT MANIPULATOR                                                */
    /************************************************************************/

    /// \brief Represents a class used to manipulate the output format of a log event inside an output stream.
    /// \author Raffaele D. Facendola - May 2020.
    class LogEventManipulator
    {
    public:

        /// \brief Create a new log event manipulator.
        LogEventManipulator(const LogEventFormatter& log_event_formatter, const LogEvent& log_event);

        /// \brief Default copy constructor.
        LogEventManipulator(const LogEventManipulator& other) = default;

        /// \brief Default move ctor.
        LogEventManipulator(LogEventManipulator&&) = default;

        /// \brief No copy-assignment operator.
        LogEventManipulator& operator=(const LogEventManipulator& other) = delete;

        /// \brief No move-assignment operator.
        LogEventManipulator& operator=(LogEventManipulator&&) = delete;

        /// \brief Default destructor.
        ~LogEventManipulator() = default;

        /// \brief Serialize the underlying log event to an output stream.
        void Serialize(std::ostream& out) const;

    private:

        /// \brief Log event formatter.
        const LogEventFormatter& log_event_formatter_;

        /// \brief Log event.
        const LogEvent& log_event_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Stream insertion for LogEventManipulator.
    std::ostream& operator<<(std::ostream& out, const LogEventManipulator& log_event_manipulator);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LogEventFormatter.

    inline LogEventFormatter::LogEventFormatter(const String& format)
        : format_(format)
    {
        Parse();
    }

    inline LogEventFormatter::LogEventFormatter(const LogEventFormatter& other)
        : LogEventFormatter(other.format_)
    {

    }

    inline LogEventFormatter& LogEventFormatter::operator=(const LogEventFormatter& other)
    {
        // Tokens cannot be copied since they have references inside the format string.

        format_ = other.format_;

        Parse();

        return *this;
    }

    inline LogEventManipulator LogEventFormatter::operator()(const LogEvent& log_event) const
    {
        return { *this, log_event };
    }

    inline void LogEventFormatter::Serialize(std::ostream& stream, const LogEvent& log_event) const
    {
        for (auto&& token : tokens_)
        {
            token.translation_rule_->translation_function_(stream, log_event, token.token_);
        }
    }

    inline void LogEventFormatter::TranslateTime(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << TimeOfDay(log_event.GetTimestamp());
    }

    inline void LogEventFormatter::TranslateDate(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << Date(log_event.GetTimestamp());
    }

    inline void LogEventFormatter::TranslateSeverity(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << log_event.GetSeverity();
    }

    inline void LogEventFormatter::TranslateThread(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << log_event.GetThread();
    }

    inline void LogEventFormatter::TranslateContext(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << log_event.GetContext();
    }

    inline void LogEventFormatter::TranslateStackTrace(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << log_event.GetStackTrace();
    }

    inline void LogEventFormatter::TranslateFunction(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << log_event.GetStackTrace().GetFunction();
    }

    inline void LogEventFormatter::TranslateMessage(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << log_event.GetMessage();
    }

    inline void LogEventFormatter::TranslatePercentage(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << '%';
    }

    inline void LogEventFormatter::TranslateUnformatted(std::ostream& stream, const LogEvent& log_event, const StringView& token)
    {
        stream << token;
    }

    // LogEvetManipulator.

    inline LogEventManipulator::LogEventManipulator(const LogEventFormatter& log_event_formatter, const LogEvent& log_event)
        : log_event_formatter_(log_event_formatter)
        , log_event_(log_event)
    {

    }

    inline void LogEventManipulator::Serialize(std::ostream& out) const
    {
        log_event_formatter_.Serialize(out, log_event_);
    }

    // Non-member functions.

    inline std::ostream& operator<<(std::ostream& out, const LogEventManipulator& log_event_manipulator)
    {
        log_event_manipulator.Serialize(out);

        return out;
    }

}
