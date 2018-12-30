
/// \file diagnostics.h
/// \brief This header is part of the syntropy diagnostic system. It contains base definitions for diagnostics and debugging functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "syntropy/platform/macros.h"

/// \brief Causes the debugger to break. If no debugger is present, does nothing.
#define SYNTROPY_BREAK \
    if(syntropy::diagnostics::Debugger::IsDebuggerAttached()) { SYNTROPY_TRAP; }

/// \brief Expands to an object representing the location of the current line of code.
#define SYNTROPY_HERE \
    syntropy::diagnostics::StackTraceElement(SYNTROPY_FILE, SYNTROPY_FUNCTION, SYNTROPY_LINE)

/// \brief Expands to an object representing the current stack trace.
#define SYNTROPY_STACKTRACE \
    syntropy::diagnostics::Debugger::GetStackTrace(SYNTROPY_HERE)

namespace syntropy::diagnostics
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

    /// \brief Represents the location of a line of code within a source file.
    /// \author Raffaele D. Facendola - November 2016
    struct StackTraceElement
    {
        /// \brief Create a new empty element.
        StackTraceElement() = default;

        /// \brief Create a new element.
        StackTraceElement(const char* file, const char* function, size_t line);

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

}
