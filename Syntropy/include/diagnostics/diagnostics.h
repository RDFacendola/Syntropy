
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
#include <set>

#include "containers/hashed_string.h"
#include "debug.h"
#include "date.h"
#include "base_io.h"

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

        /// \brief Stream insertion for Severity.
        std::ostream& operator<<(std::ostream& out, Severity severity);

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

        /// \brief Used to sort contexts.
        bool operator<(const Context& first, const Context& second);

        /// \brief Stream insertion for Context.
        std::ostream& operator<<(std::ostream& out, const Context& context);

        /// \brief Stream insertion for set of Contexts.
        std::ostream& operator<<(std::ostream& out, const std::set<Context>& contexts);

        /// \brief Base struct for application events.
        /// \author Raffaele D. Facendola - November 2016
        struct Event
        {
            /// \brief Create a new event.
            Event(std::initializer_list<Context> contexts, const StackTrace& stacktrace, Severity severity);

            std::chrono::system_clock::time_point time_;                    ///< \brief Point in time where this event was created.
            Severity severity_;                                             ///< \brief Severity of the event.
            std::thread::id thread_id_;                                     ///< \brief Id of the thread that issued the event.
            std::set<Context> contexts_;                                    ///< \brief Contexts used to categorize the event.
            StackTrace stacktrace_;                                         ///< \brief Stack trace.
        };
 
        template <typename TEvent>
        struct EventTokenTranslator
        {
            using TThunk = std::function<void(std::ostream&, const TEvent&)>;

            virtual TThunk operator()(const std::string& token) const;
        };

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
        //////////////// EVENT TOKEN TRANSLATOR ////////////////

        template <typename TEvent>
        typename EventTokenTranslator<TEvent>::TThunk EventTokenTranslator<TEvent>::operator()(const std::string& token) const
        {
            if (token == "{time}")
            {
                return [](std::ostream& out, const Event& event) { out << GetTime(event.time_); };
            }
            else if (token == "{date}")
            {
                return [](std::ostream& out, const Event& event) { out << GetDate(event.time_); };
            }
            else if (token == "{severity}")
            {
                return [](std::ostream& out, const Event& event) { out << event.severity_; };
            }
            else if (token == "{threadid}")
            {
                return [](std::ostream& out, const Event& event) { out << event.thread_id_; };
            }
            else if (token == "{contexts}")
            {
                return [](std::ostream& out, const Event& event) { out << event.contexts_; };
            }
            else if (token == "{stacktrace}")
            {
                return [](std::ostream& out, const Event& event) { out << event.stacktrace_; };
            }
            else if (token == "{function}")
            {
                return [](std::ostream& out, const Event& event) { out << event.stacktrace_.elements_[0]; };
            }

            return ConstValueInserter<std::string, TEvent>(token);
        }
    }
}