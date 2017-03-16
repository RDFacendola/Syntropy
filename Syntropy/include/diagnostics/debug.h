
/// \file debug.h
/// \brief This header is part of the syntropy diagnostic system. It contains base definitions for debugging functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>
#include <vector>

#include "syntropy.h"
#include "platform/compiler.h"

/// \brief Causes the debugger to break. If no debugger is present, does nothing.
#define SYNTROPY_BREAK \
    if(syntropy::diagnostics::GetDebugger().IsDebuggerAttached()) { SYNTROPY_TRAP; }

/// \brief Expands to an object representing the location of the current line of code.
#define SYNTROPY_HERE \
    syntropy::diagnostics::StackTraceElement(SYNTROPY_FILE, SYNTROPY_FUNCTION, SYNTROPY_LINE)

/// \brief Expands to an object representing the current stack trace.
#define SYNTROPY_STACKTRACE \
    syntropy::diagnostics::GetDebugger().GetStackTrace(SYNTROPY_HERE)

/// \brief If the condition is not verified causes the debugger to break or the application to crash.
#define SYNTROPY_ASSERT(condition) \
    if(!(condition)) { SYNTROPY_TRAP; }

/// \brief If the condition is not verified causes the debugger to break or the application to crash. This check is stripped away on release builds.
#define SYNTROPY_CHECK(condition) \
    SYNTROPY_DEBUG_ONLY( SYNTROPY_ASSERT(condition); )

/// \brief Identical to SYNTROPY_CHECK, this macro is intended to check preconditions on a function.
#define SYNTROPY_PRECONDITION(condition) \
    SYNTROPY_CHECK(condition)

/// \brief Identical to SYNTROPY_CHECK, this macro is intended to check postconditions on a function.
#define SYNTROPY_POSTCONDITION(condition) \
    SYNTROPY_CHECK(condition)

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

        /// \brief Output a stack trace element inside a stream.
        std::ostream& operator<< (std::ostream &out, const syntropy::diagnostics::StackTraceElement& element);

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
        void swap(syntropy::diagnostics::StackTrace& first, syntropy::diagnostics::StackTrace& second);

        /// \brief Output a stack trace inside a stream.
        std::ostream& operator<< (std::ostream &out, const syntropy::diagnostics::StackTrace& stacktrace);

        /// \brief Exposes platform-specific debugging functionalities.
        /// \author Raffaele D. Facendola - December 2016
        class Debugger
        {
        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance;
            static Debugger& GetInstance();

            /// \brief Default virtual destructor.
            virtual ~Debugger() = default;

            /// \brief Check whether the debugger is attached.
            /// \return Returns true if a debugger is attached to the application, returns false otherwise.
            virtual bool IsDebuggerAttached() const = 0;

            /// \brief Get the stack trace of the current thread.
            /// \param caller Stack trace element representing the code that called this method.
            /// \return Returns the stack trace whose head is caller.
            virtual diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller) const = 0;

        protected:

            /// \brief Default constructor.
            Debugger() = default;

        };

        /// \brief Get the current debugger instance.
        /// \brief Returns the current debugger instance.
        Debugger& GetDebugger();

    }
}

namespace syntropy
{
    namespace diagnostics
    {

        /************************************************************************/
        /* STACK TRACE ELEMENT                                                  */
        /************************************************************************/

        inline constexpr StackTraceElement::StackTraceElement(const char* file, const char* function, size_t line)
            : file_(file)
            , function_(function)
            , line_(line)
        {

        }

    }
}