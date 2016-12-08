#include "platform/os/windows_os.h"

#ifdef _WIN64

#pragma comment(lib, "DbgHelp.lib")

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
            static const size_t kFramesToDiscard = 1;           // Ignore this function call

            std::unique_lock<std::mutex> lock(mutex_);          // Stackwalking is not thread-safe

            diagnostics::StackTrace stacktrace;

            stacktrace.elements_.reserve(64);                   // An educated guess of the stack trace depth

            CONTEXT context;
            STACKFRAME64 stackframe;

            RtlCaptureContext(&context);                        // Initialize the current stack frame and the context
            GetStackFrame(context, stackframe);
            
            // Stack walking
            bool keep_walking;
            size_t frames = 0;

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
                frames++;

                if (frames > kFramesToDiscard + 1)
                {
                    stacktrace.elements_.emplace_back(GetStackTraceElement(stackframe));
                }
                else if (frames == kFramesToDiscard + 1)
                {
                    // We must ensure that the most recent element in the stack trace is always present, even if the symbols are not loaded.
                    // Replace the first element with the one that was generated at compile-time.
                    stacktrace.elements_.emplace_back(std::move(caller));
                }

            } 
            while (keep_walking);
            
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
