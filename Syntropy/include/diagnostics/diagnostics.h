
/// \file diagnostics.h
/// \brief This header is part of the syntropy diagnostic system. It contains base definitions for diagnostics and debugging functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "platform/macros.h"

#include "containers/hashed_string.h"

/// \brief Causes the debugger to break. If no debugger is present, does nothing.
#define SYNTROPY_BREAK \
    if(syntropy::diagnostics::Debugger::IsDebuggerAttached()) { SYNTROPY_TRAP; }

/// \brief Expands to an object representing the location of the current line of code.
#define SYNTROPY_HERE \
    syntropy::diagnostics::StackTraceElement(SYNTROPY_FILE, SYNTROPY_FUNCTION, SYNTROPY_LINE)

/// \brief Expands to an object representing the current stack trace.
#define SYNTROPY_STACKTRACE \
    syntropy::diagnostics::Debugger::GetStackTrace(SYNTROPY_HERE)

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

            /// \brief Check whether two contexts are the same.
            /// \param other Other context to  check against.
            /// \return Returns true if the two contexts are the same, returns false otherwise.
            bool operator==(const Context& other) const;

            /// \brief Check whether two contexts are not the same.
            /// \param other Other context to  check against.
            /// \return Returns true if the two contexts are not the same, returns false otherwise.
            bool operator!=(const Context& other) const;

            /// \brief Append a subcontext to this one.
            /// \param subcontext Name of the subcontext to append. May contain sub-subcontexts.
            /// \return Returns a context associated with the provided subcontext whose parent is this context.
            Context operator |(const HashedString& subcontext) const;

            /// \brief Get the context name.
            const HashedString& GetName() const;

            /// \brief Check whether this context is equal to another context or is a more general context than it.
            /// \return Returns true if this context is equal to other or any of its ancestors, returns false otherwise.
            bool Contains(const Context& other) const;

        private:

            class Pool;

            struct InnerContext;

            const InnerContext* context_;               ///< \brief Pointer to the actual context flyweight. Non-owning pointer.
        };

        /// \brief Used to sort contexts.
        bool operator<(const Context& first, const Context& second);

        /// \brief Stream insertion for Context.
        std::ostream& operator<<(std::ostream& out, const Context& context);

        /// \brief Represents the location of a line of code within a source file.
        /// \author Raffaele D. Facendola - November 2016
        struct StackTraceElement
        {
            /// \brief Create a new empty element.
            StackTraceElement() = default;

            /// \brief Create a new element.
            constexpr StackTraceElement(const char* file, const char* function, size_t line);

            /// \brief Whether the symbol referenced by this element is unknown.
            /// \return Returns true if the symbol referenced by this element is unknown, returns false otherwise.
            bool IsUnknown() const;

            std::string file_;              ///< \brief Name of the file.
            std::string function_;          ///< \brief Name of the function.
            size_t line_;                   ///< \brief Line inside the source file.
        };

        /// \brief Output a stack trace element inside a stream.
        std::ostream& operator<< (std::ostream &out, const StackTraceElement& element);

        /// \brief Represents a stack trace.
        /// \author Raffaele D. Facendola - November 2016
        struct StackTrace
        {
            /// \brief Create an empty stacktrace.
            StackTrace() = default;

            /// \brief Create a new callstack from a single element.
            StackTrace(const StackTraceElement& element);

            /// \brief Copy ctor.
            StackTrace(const StackTrace& other) = default;

            ~StackTrace() = default;

            /// \brief Move ctor.
            StackTrace(StackTrace&& other) noexcept;

            /// \brief Unified assignment operator.
            StackTrace& operator=(StackTrace other) noexcept;

            /// \brief Swap the content of two callstack instances.
            void Swap(StackTrace& other) noexcept;

            std::vector<StackTraceElement> elements_;                       ///< \brief Elements inside the stack trace, from the most recent one.
        };

        /// \brief Swaps two stacktraces.
        void swap(StackTrace& first, StackTrace& second);

        /// \brief Output a stack trace inside a stream.
        std::ostream& operator<< (std::ostream &out, const StackTrace& stacktrace);

        /// \brief Exposes platform-specific debugging functionalities.
        /// \author Raffaele D. Facendola - December 2016
        class Debugger
        {
        public:

            /// \brief Check whether the debugger is attached.
            /// \return Returns true if a debugger is attached to the application, returns false otherwise.
            static bool IsDebuggerAttached();

            /// \brief Get the stack trace of the current thread.
            /// \param caller Stack trace element representing the code that called this method.
            /// \return Returns the stack trace whose head is caller.
            static StackTrace GetStackTrace(StackTraceElement caller);

        };

        /// \brief Log context for the syntropy diagnostic system.
        extern const Context DiagnosticsCtx;

    }

}

namespace syntropy
{
    namespace diagnostics
    {

        /************************************************************************/
        /* STACK TRACE ELEMENT                                                  */
        /************************************************************************/

        constexpr StackTraceElement::StackTraceElement(const char* file, const char* function, size_t line)
            : file_(file)
            , function_(function)
            , line_(line)
        {

        }

    }
}