
/// \file diagnostics.h
/// \brief This header is part of the syntropy diagnostic system. It contains base definitions for diagnostics functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <chrono>
#include <thread>
#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <iomanip>

#include "hinnant/date/date.h"
#include "containers/hashed_string.h"
#include "debug.h"

namespace syntropy 
{
    namespace diagnostics 
    {

        /// \brief Severity of an event.
        /// \author Raffaele D. Facendola - November 2016
        enum class Severity : uint8_t
        {
            kInformative = 0u,                                              /// \brief Severity used for informative and tracing events that cause no side-effects on the application.
            kWarning = 1u,                                                  /// \brief Severity used for uncommon events that may cause undesired, yet acceptable, side-effects on the application.
            kError = 2u,                                                    /// \brief Severity used for rare events that have substantial impact on the application. Those events should not occur on a shipping version of the application.
            kCritical = 3u                                                  /// \brief Severity used for exceptional events that will cause the application to crash. Those events should never occur!
        };

        /// \brief Context used to categorize events.
        /// \author Raffaele D. Facendola - November 2016
        class Context
        {

        public:

            /// \brief character used to separate two context levels.
            static const char kSeparator = '|';

            /// \brief Create a root context.
            Context();

            /// \brief Create a new context from a name.
            Context(const HashedString& name);

            bool operator==(const Context& other) const;

            bool operator!=(const Context& other) const;

            /// \brief Get the context name.
            const HashedString& GetName() const;

            /// \brief Check whether this context is equal to another context or is a more general context than it.
            /// \return Returns true if this context is equal to other or any of its ancestors, returns false otherwise.
            bool Contains(const Context& other) const;

        private:

            class Pool;

            struct InnerContext;

            std::shared_ptr<InnerContext> context_;                ///< \brief Current context.

        };

        /// \brief Stream insertion for Context.
        std::ostream& operator<<(std::ostream& out, const Context& context);
        
        /// \brief Base struct for application events.
        /// \author Raffaele D. Facendola - November 2016
        struct Event
        {
            /// \brief Create a new event.
            Event(std::initializer_list<Context> contexts, const StackTrace& stacktrace, Severity severity);

            std::chrono::system_clock::time_point time_;                    ///< \brief Point in time where this event was created.
            Severity severity_;                                             ///< \brief Severity of the event.
            std::thread::id thread_id_;                                     ///< \brief Id of the thread that issued the event.
            std::vector<Context> contexts_;                                 ///< \brief Contexts used to categorize the event.
            StackTrace stacktrace_;                                         ///< \brief Stack trace.
        };

        /// \brief Used to format an event to a string.
        /// \author Raffaele D. Facendola - December 2016
        template <typename TEvent = Event>
        class EventFormatter
        {
            static_assert(std::is_base_of_v<Event, TEvent>, "TEvent must derive from syntropy::diagnostics::Event");

        public:

            EventFormatter(const char* format);

            std::ostream& operator()(std::ostream& out, const TEvent& event) const;

        protected:

            struct Appender
            {
                virtual void Append(std::ostream& out, const TEvent& event) const = 0;
            };

            struct ConstantAppender : Appender
            {
                ConstantAppender(const char* string);

                void Append(std::ostream& stream, const TEvent&) const override;

                std::string string_;
            };

            struct TimeAppender : Appender
            {
                void Append(std::ostream& stream, const TEvent& event) const override;
            };

            struct DateAppender : Appender
            {
                void Append(std::ostream& stream, const TEvent& event) const override;
            };

            struct SeverityAppender : Appender
            {
                void Append(std::ostream& stream, const TEvent& event) const override;
            };

            struct ThreadIdAppender : Appender
            {
                void Append(std::ostream& stream, const TEvent& event) const override;
            };

            struct ContextsAppender : Appender
            {
                void Append(std::ostream& stream, const TEvent& event) const override;
            };

            struct StackTraceAppender : Appender
            {
                void Append(std::ostream& stream, const TEvent& event) const override;
            };

            struct FunctionAppender : Appender
            {
                void Append(std::ostream& stream, const TEvent& event) const override;
            };

        private:

            std::vector<std::unique_ptr<Appender>> appenders_;                  ///< \brief Used to append strings to the output stream.

        };
        
        /// \brief Severity type traits.
        template <Severity kSeverity>
        struct SeverityTraits {};

        /// \brief Specialization for informative severity.
        template <>
        struct SeverityTraits<Severity::kInformative>
        {
            static const char* ToString()
            {
                return "Info";
            }
        };

        /// \brief Specialization for warning severity.
        template <>
        struct SeverityTraits<Severity::kWarning>
        {
            static const char* ToString()
            {
                return "Warning";
            }
        };

        /// \brief Specialization for error severity.
        template <>
        struct SeverityTraits<Severity::kError>
        {
            static const char* ToString()
            {
                return "Error";
            }
        };

        /// \brief Specialization for critical severity.
        template <>
        struct SeverityTraits<Severity::kCritical>
        {
            static const char* ToString()
            {
                return "Critical";
            }
        };

        const char* ToString(Severity severity);
    }
}

namespace std 
{

    template <>
    inline void swap(syntropy::diagnostics::StackTrace& first, syntropy::diagnostics::StackTrace& second)
    {
        first.Swap(second);
    }

}

namespace syntropy
{
    namespace diagnostics
    {

        // Implementation

        //////////////// EVENT FORMATTER ////////////////

        template <typename TEvent>
        EventFormatter<TEvent>::EventFormatter(const char* /*format*/)
        {
            appenders_.emplace_back(std::make_unique<ConstantAppender>("["));
            appenders_.emplace_back(std::make_unique<ContextsAppender>());
            appenders_.emplace_back(std::make_unique<ConstantAppender>("] "));
            appenders_.emplace_back(std::make_unique<StackTraceAppender>());
        }

        template <typename TEvent>
        std::ostream& EventFormatter<TEvent>::operator()(std::ostream& out, const TEvent& event) const
        {
            for (auto&& appender : appenders_)
            {
                appender->Append(out, event);
            }
            return out;
        }

        //////////////// EVENT FORMATTER :: APPENDERS ////////////////

        template <typename TEvent>
        EventFormatter<TEvent>::ConstantAppender::ConstantAppender(const char* string)
            : string_(string)
        {

        }

        template <typename TEvent>
        void EventFormatter<TEvent>::ConstantAppender::Append(std::ostream& stream, const TEvent&) const
        {
            stream << string_;
        }

        template <typename TEvent>
        void EventFormatter<TEvent>::TimeAppender::Append(std::ostream& stream, const TEvent& event) const
        {
            auto today = date::floor<date::days>(event.time_);
            auto time = date::make_time(std::chrono::duration_cast<std::chrono::milliseconds>(event.time_ - today));

            stream << time;
        }

        template <typename TEvent>
        void EventFormatter<TEvent>::DateAppender::Append(std::ostream& stream, const TEvent& event) const
        {
            auto today = date::year_month_day{ date::floor<date::days>(event.time_) };

            stream << today;
        }

        template <typename TEvent>
        void EventFormatter<TEvent>::SeverityAppender::Append(std::ostream& stream, const TEvent& event) const
        {
            stream << ToString(event.severity_);
        }

        template <typename TEvent>
        void EventFormatter<TEvent>::ThreadIdAppender::Append(std::ostream& stream, const TEvent& event) const
        {
            stream << event.thread_id_;
        }

        template <typename TEvent>
        void EventFormatter<TEvent>::ContextsAppender::Append(std::ostream& stream, const TEvent& event) const
        {
            if (event.contexts_.size() > 0)
            {
                auto&& it = event.contexts_.begin();

                stream << *it;                      // First context

                for(++it; it != event.contexts_.end(); ++it)
                {
                    stream << ", " << *it;          // Remaining contexts
                }
            }
        }

        template <typename TEvent>
        void EventFormatter<TEvent>::StackTraceAppender::Append(std::ostream& stream, const TEvent& event) const
        {
            stream << event.stacktrace_;
        }

        template <typename TEvent>
        void EventFormatter<TEvent>::FunctionAppender::Append(std::ostream& stream, const TEvent& event) const
        {
            stream << event.stacktrace_.elements_[0];
        }

    }
}