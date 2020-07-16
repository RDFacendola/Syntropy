#ifdef _WIN64

#include "syntropy/hal/hal_debugger.h"

#include <mutex>

#include "syntropy/memory/memory.h"

/************************************************************************/
/* HEADERS & LIBRARIES                                                  */
/************************************************************************/

#pragma comment(lib, "DbgHelp.lib")

#pragma warning(push)
#pragma warning(disable:4091)

 #include <Windows.h>
 #include <DbgHelp.h>

#undef max

#pragma warning(pop)

namespace syntropy
{
    /************************************************************************/
    /* WINDOWS DEBUGGER                                                     */
    /************************************************************************/

    /// \brief Represents a stateful Windows debugger.
    /// \author Raffaele D. Facendola - May 2020.
    class WindowsDebugger
    {
    public:

        /// \brief Maximum symbol length.
        static const size_t kMaxSymbolLength = 1024;

        /// \brief Get the singleton instance.
        static WindowsDebugger& GetSingleton();

        /// \brief Destructor.
        ~WindowsDebugger();

        /// \brief Get the current stack trace.
        StackTrace GetStackTrace(StackTraceElement caller) const;

    private:

        /// \brief Create a new Windows debugger.
        WindowsDebugger();

        /// \brief Get the stack frame from processor-specific register data.
        STACKFRAME64 GetStackFrame(const CONTEXT& context) const;

        /// \brief Get a StackTraceElement from a stack frame.
        StackTraceElement GetStackTraceElement(const STACKFRAME64& stack_frame) const;

        /// \brief Used for synchronization as debugger functionalities are not thread safe.
        mutable std::mutex mutex_;

        /// \brief Current process handle.
        HANDLE process_{ nullptr };

        /// \brief Whether symbols for this process exists and are loaded.
        Bool has_symbols_{ false };

    };

    /************************************************************************/

    WindowsDebugger& WindowsDebugger::GetSingleton()
    {
        static WindowsDebugger singleton;
        return singleton;
    }

    WindowsDebugger::WindowsDebugger()
        : process_(GetCurrentProcess())
    {
        has_symbols_ = (SymInitialize(process_, nullptr, true) != 0);

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

    StackTrace WindowsDebugger::GetStackTrace(StackTraceElement caller) const
    {
        auto lock = std::unique_lock<std::mutex>{ mutex_ };

        auto context = CONTEXT{};
        auto stack_trace = StackTrace{ caller };

        RtlCaptureContext(&context);

        auto stack_frame = GetStackFrame(context);

        for (auto keep_walking = true; keep_walking;)
        {
            keep_walking = StackWalk64(IMAGE_FILE_MACHINE_AMD64, process_, GetCurrentThread(), &stack_frame, &context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr) != 0;

            stack_trace += GetStackTraceElement(stack_frame);
        }

        return stack_trace;
    }

    STACKFRAME64 WindowsDebugger::GetStackFrame(const CONTEXT& context) const
    {
        auto stack_frame = STACKFRAME64{};

        Memory::Zero(RWByteSpan{ reinterpret_cast<RWBytePtr>(&stack_frame), ToInt(BytesOf(stack_frame)) });

        // https://msdn.microsoft.com/it-it/library/windows/desktop/ms680646(v=vs.85).aspx

        stack_frame.AddrPC.Offset = context.Rip;
        stack_frame.AddrPC.Mode = AddrModeFlat;
        stack_frame.AddrStack.Offset = context.Rsp;
        stack_frame.AddrStack.Mode = AddrModeFlat;
        stack_frame.AddrFrame.Offset = context.Rbp;
        stack_frame.AddrFrame.Mode = AddrModeFlat;

        return stack_frame;
    }

    StackTraceElement WindowsDebugger::GetStackTraceElement(const STACKFRAME64& stack_frame) const
    {
        auto element = StackTraceElement{};

        // File and line.

        auto displacement = DWORD{};
        auto line_info = IMAGEHLP_LINE64{};

        line_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        if (SymGetLineFromAddr64(process_, stack_frame.AddrPC.Offset, &displacement, &line_info))
        {
            element.file_ = line_info.FileName;
            element.line_ = line_info.LineNumber;
        }

        // Symbol name.

        union SymbolInfo
        {
            /// \brief Buffer containing actual symbol data.
            char buffer[sizeof(SYMBOL_INFO) + kMaxSymbolLength * sizeof(TCHAR)];

            /// \brief Actual symbol info.
            SYMBOL_INFO symbol;
        };

        auto symbol_info = SymbolInfo{};

        symbol_info.symbol.SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol_info.symbol.MaxNameLen = kMaxSymbolLength;

        if (SymFromAddr(process_, stack_frame.AddrPC.Offset, nullptr, &symbol_info.symbol))
        {
            element.function_ = symbol_info.symbol.Name;
        }

        return element;
    }

    /************************************************************************/
    /* HAL DEBUGGER                                                         */
    /************************************************************************/

    Bool HALDebugger::IsAttached()
    {
        return IsDebuggerPresent() != 0;
    }

    StackTrace HALDebugger::GetStackTrace(StackTraceElement here)
    {
        return WindowsDebugger::GetSingleton().GetStackTrace(std::move(here));
    }

}

#endif