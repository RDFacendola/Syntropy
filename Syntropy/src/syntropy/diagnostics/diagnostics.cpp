#include "syntropy/diagnostics/diagnostics.h"

#include <unordered_map>

#include "syntropy/platform/os/os.h"

namespace syntropy::diagnostics
{
    /************************************************************************/
    /* SEVERITY                                                             */
    /************************************************************************/

    static const std::unordered_map<Severity, const char*> severity_map = { { Severity::kInformative, "Info" },
                                                                            { Severity::kWarning, "Warning" },
                                                                            { Severity::kError, "Error" },
                                                                            { Severity::kCritical, "Critical" } };

    std::ostream& operator<<(std::ostream& out, Severity severity)
    {
        auto it = severity_map.find(severity);

        if (it != severity_map.end())
        {
            out << it->second;
        }
        else
        {
            out << "<Unknown severity>";
        }

        return out;
    }

    /************************************************************************/
    /* STACK TRACE ELEMENT                                                  */
    /************************************************************************/

    StackTraceElement::StackTraceElement(const char* file, const char* function, size_t line)
        : file_(file)
        , function_(function)
        , line_(line)
    {

    }

    bool StackTraceElement::IsUnknown() const
    {
        return file_.size() == 0 && function_.size() == 0;
    }

    std::ostream& operator<< (std::ostream &out, const StackTraceElement& element)
    {
        // Symbol
        if (element.function_.size() > 0)
        {
            out << element.function_;
        }
        else
        {
            out << "<unknown-symbol>";
        }

        // File and line
        if (element.file_.size() > 0)
        {
            out << " (" << element.file_.c_str() << ":" << element.line_ << ")";
        }

        return out;
    }

    /************************************************************************/
    /* STACK TRACE                                                          */
    /************************************************************************/

    StackTrace::StackTrace(const StackTraceElement& element)
    {
        elements_.push_back(element);
    }

    StackTrace::StackTrace(StackTrace&& other) noexcept
        : elements_(std::move(other.elements_))
    {

    }

    StackTrace& StackTrace::operator=(StackTrace other) noexcept
    {
        other.Swap(*this);
        return *this;
    }

    void StackTrace::Swap(StackTrace& other) noexcept
    {
        std::swap(elements_, other.elements_);
    }

    void swap(StackTrace& first, StackTrace& second)
    {
        first.Swap(second);
    }

    std::ostream& operator<< (std::ostream &out, const StackTrace& stacktrace)
    {
        if (stacktrace.elements_.size() > 0)
        {
            out << stacktrace.elements_[0];             // The most recent element in the stack trace is always known

            const StackTraceElement* unknown_element = nullptr;
            const StackTraceElement* current_element;

            size_t unknown_elements = 0;

            for (size_t index = 1; index < stacktrace.elements_.size(); ++index)
            {
                current_element = &stacktrace.elements_[index];

                // Accumulate unknown elements
                if (current_element->IsUnknown())
                {
                    unknown_element = current_element;
                    ++unknown_elements;
                }

                // Output unknown elements if we reached the base of the stack or the current element is no longer unknown
                if (unknown_elements > 0 &&
                    (unknown_element != current_element || index == stacktrace.elements_.size() - 1))
                {
                    out << "\n   <" << unknown_elements << " unknown symbol(s)>";
                    unknown_elements = 0;
                }

                // Output a known element
                if (!current_element->IsUnknown())
                {
                    out << "\n   " << *current_element;
                }
            }
        }
        return out;
    }

    /************************************************************************/
    /* DEBUGGER                                                             */
    /************************************************************************/

    bool Debugger::IsDebuggerAttached()
    {
        return platform::PlatformDebugger::IsDebuggerAttached();
    }

    StackTrace Debugger::GetStackTrace(StackTraceElement caller)
    {
        return platform::PlatformDebugger::GetStackTrace(std::move(caller));
    }
}
