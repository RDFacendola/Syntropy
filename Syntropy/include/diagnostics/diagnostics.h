
/// \file diagnostics.h
/// \brief This header is part of the syntropy diagnostic system. It contains base definitions for diagnostics functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <chrono>
#include <thread>

#include <vector>
#include <memory>

#include "platform/platform.h"
#include "containers/hashed_string.h"

/// \brief Expands to an object representing the location of the current line of code.
#define SYNTROPY_TRACE \
    syntropy::diagnostics::Trace(SYNTROPY_FILE, SYNTROPY_FUNCTION, SYNTROPY_LINE)

namespace syntropy 
{
    namespace diagnostics 
    {

        /// \brief Represents the location of a line of code
        /// \author Raffaele D. Facendola - November 2016
        struct Trace
        {
            /// \brief Create a new code trace.
            Trace(const char* file, const char* function, int line);

            const char* file_;                                              ///< \brief Name of the file where the log was defined.
            const char* function_;                                          ///< \brief Name of the function where the log was defined.
            int line_;                                                      ///< \brief Line inside the file where the log was defined.
        };

        /// \brief Represents a stack trace.
        /// \author Raffaele D. Facendola - November 2016
        struct StackTrace
        {
            /// \brief Create a new callstack from a single code trace.
            StackTrace(const Trace& trace);

            /// \brief Copy ctor.
            StackTrace(const StackTrace& other) = default;

            ~StackTrace() = default;

            /// \brief Move ctor.
            StackTrace(StackTrace&& other) noexcept;

            /// \brief Unified assignment operator.
            StackTrace& operator=(StackTrace other) noexcept;

            /// \brief Swap the content of two callstack instances.
            void Swap(StackTrace& other) noexcept;

            /// \brief Get the last function called by the stack.
            operator Trace&();

            /// \brief Get the last function called by the stack.
            operator const Trace&() const;

            std::vector<Trace> callstack_;                                  ///< \brief List of function calls, starting from the most recent one. Callstack must always contain at least one element.
        };

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
            Event(std::initializer_list<Context> contexts, const StackTrace& callstack, Severity severity);

            std::chrono::high_resolution_clock::time_point timestamp_;      ///< \brief Point in time where the event was created.
            Severity severity_;                                             ///< \brief Severity of the event.
            std::thread::id thread_id_;                                     ///< \brief Id of the thread that issued the event.
            std::vector<Context> contexts_;                                 ///< \brief Contexts used to categorize the event.
            StackTrace callstack_;                                          ///< \brief Callstack that caused the event.
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