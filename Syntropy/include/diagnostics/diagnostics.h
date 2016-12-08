
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

        /// \brief Base struct for application events.
        /// \author Raffaele D. Facendola - November 2016
        struct Event
        {
            /// \brief Create a new event.
            Event(std::initializer_list<Context> contexts, const StackTrace& stacktrace, Severity severity);

            std::chrono::high_resolution_clock::time_point timestamp_;      ///< \brief Point in time where the event was created.
            Severity severity_;                                             ///< \brief Severity of the event.
            std::thread::id thread_id_;                                     ///< \brief Id of the thread that issued the event.
            std::vector<Context> contexts_;                                 ///< \brief Contexts used to categorize the event.
            StackTrace stacktrace_;                                         ///< \brief Stack trace.
        };

        /// \brief Used to format an event to a string.
        /// \author Raffaele D. Facendola - December 2016
        struct EventFormatter
        {
            //static const char* kSeverityToken = "$severity";
            //static const char* kThreadIdtoken = "$threadid";
            //static const char* kStackTraceToken = "$stacktrace";

            //static const char* kDateToken = "$date";                        ///< \brief YYYY-MM-DD
            //static const char* kTimeToken = "$time";                        ///< \brief hh:mm:ss
            //static const char* kTimestampToken = "$timestamp";              ///< \brief hh:mm:ss.milliseconds
            
            EventFormatter(const char* /*format*/) {}

            virtual std::string operator()(const Event& /*event*/) const { return ""; }

        };

        /// \brief Severity type traits.
        template <Severity kSeverity>
        struct SeverityTraits {};

        /// \brief Specialization for informative severity.
        template <>
        struct SeverityTraits<Severity::kInformative>
        {
            static std::string ToString()
            {
                return "Info";
            }
        };

        /// \brief Specialization for warning severity.
        template <>
        struct SeverityTraits<Severity::kWarning>
        {
            static std::string ToString()
            {
                return "Warning";
            }
        };

        /// \brief Specialization for error severity.
        template <>
        struct SeverityTraits<Severity::kError>
        {
            static std::string ToString()
            {
                return "Error";
            }
        };

        /// \brief Specialization for critical severity.
        template <>
        struct SeverityTraits<Severity::kCritical>
        {
            static std::string ToString()
            {
                return "Critical";
            }
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