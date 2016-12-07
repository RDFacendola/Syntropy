#include "platform/windows_platform.h"

#ifdef _WIN64

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#pragma warning(pop)

#include <intrin.h>
#include <mutex>

#include "syntropy.h"

#pragma comment(lib, "DbgHelp.lib")

namespace syntropy
{
    namespace platform
    {

        //////////////// DEBUG ////////////////

        struct Debug::Implementation
        {
            static const size_t kMaxSymbolLength = 1024;

            union SymbolInfo
            {
                char buffer[sizeof(SYMBOL_INFO) + kMaxSymbolLength * sizeof(TCHAR)];
                SYMBOL_INFO symbol;
            };

            Implementation();

            ~Implementation();

            diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller);

            diagnostics::StackTraceElement GetStackTraceElement(const STACKFRAME64& stackframe);
            
            std::mutex mutex_;                  ///< \brief Used for synchronization. Symbol loading and stack walking are not thread safe!

            HANDLE process_;                    ///< \brief Current process handle.

            bool has_symbols_;                  ///< \brief Whether the symbols for this process were loaded correctly.

        };

        Debug::Implementation::Implementation()
            : process_(GetCurrentProcess())
        {
            has_symbols_ = SymInitialize(process_, nullptr, true) != 0;

            if (has_symbols_)
            {
                SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
            }
        }

        Debug::Implementation::~Implementation()
        {
            if (has_symbols_)
            {
                SymCleanup(process_);
            }
        }

        diagnostics::StackTrace Debug::Implementation::GetStackTrace(diagnostics::StackTraceElement caller)
        {
            static const size_t kFramesToDiscard = 2;           // Ignore this function call and Debug::GetStackTrace

            std::unique_lock<std::mutex> lock(mutex_);

            diagnostics::StackTrace stacktrace;

            stacktrace.elements_.reserve(64);

            CONTEXT context;
            RtlCaptureContext(&context);

            STACKFRAME64 stackframe;

            memset(&stackframe, 0, sizeof(stackframe));

            // https://msdn.microsoft.com/it-it/library/windows/desktop/ms680646(v=vs.85).aspx
            stackframe.AddrPC.Offset = context.Rip;
            stackframe.AddrPC.Mode = AddrModeFlat;
            stackframe.AddrStack.Offset = context.Rsp;
            stackframe.AddrStack.Mode = AddrModeFlat;
            stackframe.AddrFrame.Offset = context.Rbp;
            stackframe.AddrFrame.Mode = AddrModeFlat;

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

        diagnostics::StackTraceElement Debug::Implementation::GetStackTraceElement(const STACKFRAME64& stackframe)
        {
            diagnostics::StackTraceElement element;

            IMAGEHLP_LINE64 line_info;
            SymbolInfo symbol_info;

            DWORD displacement;
            
            line_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

            symbol_info.symbol.SizeOfStruct = sizeof(SYMBOL_INFO);
            symbol_info.symbol.MaxNameLen = kMaxSymbolLength;

            if (SymGetLineFromAddr64(process_, stackframe.AddrPC.Offset, &displacement, &line_info))
            {
                element.file_ = line_info.FileName;
                element.line_ = line_info.LineNumber;
            }
            else
            {
                element.line_ = 0;
            }

            if (SymFromAddr(process_, stackframe.AddrPC.Offset, nullptr, &symbol_info.symbol))
            {
                element.function_ = symbol_info.symbol.Name;
            }
            return element;
        }

        //////////////// DEBUG ////////////////

        Debug& Debug::GetInstance()
        {
            static Debug instance;
            return instance;
        }

        Debug::Debug()
            : implementation_(std::make_unique<Debug::Implementation>())
        {

        }
        
        bool Debug::IsDebuggerAttached()
        {
            return IsDebuggerPresent() != 0;
        }

        diagnostics::StackTrace Debug::GetStackTrace(diagnostics::StackTraceElement caller)
        {
            return implementation_->GetStackTrace(std::move(caller));
        }

        Debug& GetDebug()
        {
            return Debug::GetInstance();
        }

    }
}

#endif
