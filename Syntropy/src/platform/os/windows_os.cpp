#include "platform/os/windows_os.h"

#ifdef _WIN64

#pragma comment(lib, "DbgHelp.lib")

#include "scope_guard.h"

namespace syntropy
{
    namespace platform
    {

        //////////////// DEBUGGER ////////////////

        diagnostics::Debugger& WindowsDebugger::GetInstance()
        {
            static WindowsDebugger instance;
            return instance;
        }

        WindowsDebugger::WindowsDebugger()
            : process_(GetCurrentProcess())
        {
            has_symbols_ = SymInitialize(process_, nullptr, true) != 0;

            if (has_symbols_)
            {
                SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
            }
        }

        WindowsDebugger::~WindowsDebugger()
        {
            if (has_symbols_)
            {
                SymCleanup(process_);
            }
        }

        bool WindowsDebugger::IsDebuggerAttached() const
        {
            return IsDebuggerPresent() != 0;
        }

        diagnostics::StackTrace WindowsDebugger::GetStackTrace(diagnostics::StackTraceElement caller) const
        {
            auto stacktrace = GetStackTrace(1, true);                   // Discard this stack frame and trace everything else

            // Ensures that the most recent element in the stack trace is always present, even if the symbols are not loaded.
            stacktrace.elements_.front() = std::move(caller);

            return stacktrace;
        }

        diagnostics::StackTrace WindowsDebugger::GetCallTrace(size_t count) const
        {
            return GetStackTrace(count + 1, false);                                 // Discard this function call as well
        }

        diagnostics::StackTrace WindowsDebugger::GetStackTrace(size_t discard_count, bool trace_all) const
        {
            std::unique_lock<std::mutex> lock(mutex_);                  // Stackwalking is not thread-safe

            ++discard_count;                                            // Discard this stack frame as well

            diagnostics::StackTrace stacktrace;

            stacktrace.elements_.reserve(64);                           // An educated guess of the stack trace depth

            CONTEXT context;
            STACKFRAME64 stackframe;

            RtlCaptureContext(&context);                                // Initialize the current stack frame and the context
            GetStackFrame(context, stackframe);

            // Stack walking
            bool keep_walking;

            do
            {
                keep_walking = StackWalk64(IMAGE_FILE_MACHINE_AMD64,
                                           process_,
                                           GetCurrentThread(),
                                           &stackframe,
                                           &context,
                                           nullptr,
                                           SymFunctionTableAccess64,
                                           SymGetModuleBase64,
                                           nullptr) != 0;

                if (discard_count == 0)
                {
                    stacktrace.elements_.emplace_back(GetStackTraceElement(stackframe));

                    keep_walking &= trace_all;                          // Exit on the first non-discarded element if trace_all is false
                }
                else
                {
                    --discard_count;
                }

            } while (keep_walking);

            return stacktrace;
        }

        void WindowsDebugger::GetStackFrame(const CONTEXT& context, STACKFRAME64& stackframe) const
        {
            memset(&stackframe, 0, sizeof(stackframe));

            // https://msdn.microsoft.com/it-it/library/windows/desktop/ms680646(v=vs.85).aspx
            stackframe.AddrPC.Offset = context.Rip;
            stackframe.AddrPC.Mode = AddrModeFlat;
            stackframe.AddrStack.Offset = context.Rsp;
            stackframe.AddrStack.Mode = AddrModeFlat;
            stackframe.AddrFrame.Offset = context.Rbp;
            stackframe.AddrFrame.Mode = AddrModeFlat;
        }

        diagnostics::StackTraceElement WindowsDebugger::GetStackTraceElement(const STACKFRAME64& stackframe) const
        {
            diagnostics::StackTraceElement element;

            IMAGEHLP_LINE64 line_info;
            SymbolInfo symbol_info;

            DWORD displacement;
            
            line_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

            symbol_info.symbol.SizeOfStruct = sizeof(SYMBOL_INFO);
            symbol_info.symbol.MaxNameLen = kMaxSymbolLength;

            // File and line
            if (SymGetLineFromAddr64(process_, stackframe.AddrPC.Offset, &displacement, &line_info))
            {
                element.file_ = line_info.FileName;
                element.line_ = line_info.LineNumber;
            }
            else
            {
                element.line_ = 0;
            }

            // Symbol name
            if (SymFromAddr(process_, stackframe.AddrPC.Offset, nullptr, &symbol_info.symbol))
            {
                element.function_ = symbol_info.symbol.Name;
            }

            return element;
        }

    }
}

#endif
