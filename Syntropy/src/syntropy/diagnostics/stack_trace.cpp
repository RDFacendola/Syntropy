#include "syntropy/diagnostics/stack_trace.h"

#include "syntropy/hal/hal_debugger.h"

namespace Syntropy
{
    /************************************************************************/
    /* DEBUGGER                                                             */
    /************************************************************************/

    StackTrace Debugger::GetStackTrace(StackTraceElement here)
    {
        return HALDebugger::GetStackTrace(std::move(here));
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    std::ostream& operator<< (std::ostream &out, const StackTrace& stack_trace)
    {
        auto unknown_elements = 0;

        // Collapse many unknown elements.

        auto dump_unknown_elements = [&unknown_elements, &out]()
        {
            if (unknown_elements > 0)
            {
                out << "   " << unknown_elements << " unknown symbol(s)\n";

                unknown_elements = 0;
            }
        };

        // Dump the first symbol.

        out << "> " << Front(stack_trace.GetData()) << "\n";

        // Dump a line for each remaining stack trace element.

        for (auto&& stack_trace_element : PopFront(stack_trace.GetData()))
        {
            if (stack_trace_element.file_.size() > 0 && stack_trace_element.function_.size() > 0)
            {
                dump_unknown_elements();

                out << "   " << stack_trace_element << "\n";
            }
            else
            {
                ++unknown_elements;
            }
        }

        // Dump in case the base of the stack trace is unknown as well.

        dump_unknown_elements();

        return out;
    }

    std::ostream& operator<< (std::ostream &out, const StackTraceElement& element)
    {
        if (element.function_.size() > 0)
        {
            out << element.function_;
        }
        else
        {
            out << "<unknown-symbol>";
        }

        if (element.file_.size() > 0)
        {
            out << " (" << element.file_.c_str() << ":" << element.line_ << ")";
        }

        return out;
    }

}