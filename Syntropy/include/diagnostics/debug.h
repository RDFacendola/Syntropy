
/// \file debug.h
/// \brief This header is part of the syntropy diagnostic system. It contains base definitions for debugging functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>
#include <vector>

#include "platform/compiler.h"

/// \brief Causes the debugger to break. If no debugger is present, does nothing.
#define SYNTROPY_BREAK \
    if(syntropy::diagnostics::Debugger::IsDebuggerAttached()) { SYNTROPY_TRAP; }

/// \brief Expands to an object representing the location of the current line of code.
#define SYNTROPY_HERE \
    syntropy::diagnostics::StackTraceElement(SYNTROPY_FILE, SYNTROPY_FUNCTION, SYNTROPY_LINE)

/// \brief Expands to an object representing the current stack trace.
#define SYNTROPY_STACKTRACE \
    syntropy::diagnostics::Debugger::GetStackTrace(SYNTROPY_HERE)

/// \brief If the condition is not verified causes the debugger to break or the application to crash.
#define SYNTROPY_ASSERT(condition) \
    if(!(condition)) { SYNTROPY_TRAP; }

namespace syntropy
{
    namespace diagnostics
    {

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

            std::string file_;                                              ///< \brief Name of the file.
            std::string function_;                                          ///< \brief Name of the function.
            size_t line_;                                                   ///< \brief Line inside the source file.
        };

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

        /// \brief Output a stack trace element inside a stream.
        std::ostream& operator<< (std::ostream &out, const StackTraceElement& element);

        /// \brief Swaps two stacktraces.
        void swap(StackTrace& first, StackTrace& second);

        /// \brief Output a stack trace inside a stream.
        std::ostream& operator<< (std::ostream &out, const StackTrace& stacktrace);
        
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