/// \file windows_platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains Windows-specific functionalities.
///
/// Do not use this header directly!
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifdef _WIN64

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#pragma warning(pop)

#include <mutex>
#include <thread>

#include "diagnostics/debug.h"
#include "platform/system.h"

namespace syntropy
{
    namespace platform
    {

        /// \brief Exposes Windows-specific debugging functionalities under Windows OS.
        /// \author Raffaele D. Facendola - December 2016
        class WindowsDebugger : public diagnostics::Debugger
        {
        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance;
            static diagnostics::Debugger& GetInstance();

            /// \brief Destructor.
            virtual ~WindowsDebugger();

            virtual bool IsDebuggerAttached() const override;
            
            virtual diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller) const override;

            virtual diagnostics::StackTrace GetCallTrace(size_t count) const override;

        private:

            /// \brief Maximum symbol length.
            static const size_t kMaxSymbolLength = 1024;

            /// \brief Contains a properly-sized SYMBOL_INFO structure that accounts for the actual maximum symbol length.
            union SymbolInfo
            {
                char buffer[sizeof(SYMBOL_INFO) + kMaxSymbolLength * sizeof(TCHAR)];        ///< \brief Raw buffer.

                SYMBOL_INFO symbol;                                                         ///< \brief Actual symbol info.
            };

            /// \brief Default constructor.
            WindowsDebugger();

            /// \brief Get the limited stacktrace.
            /// \param discard_count Number of stack frames to discard, starting from the most recent one.
            /// \param trace_all Whether to perform a full stack walk (true) or not. If false returns a single function call.
            /// \return Returns the stack trace whose head is the discard_count-th stack frame starting from this call (excluded) and with a maximum depth of trace_count.
            diagnostics::StackTrace GetStackTrace(size_t discard_count, bool trace_all) const;

            /// \brief Get the stackframe from context.
            /// \param context Contains the current context.
            /// \param stackframe Contains the current stackframe.
            void GetStackFrame(const CONTEXT& context, STACKFRAME64& stackframe) const;

            /// \brief Get a StackTraceElement from a stack frame.
            /// \param stackframe Stackframe to convert.
            /// \return Returns the StackTraceElement associated to the provided stackframe.
            diagnostics::StackTraceElement GetStackTraceElement(const STACKFRAME64& stackframe) const;

            mutable std::mutex mutex_;                      ///< \brief Used for synchronization. Symbol loading and stack walking are not thread safe!

            HANDLE process_;                                ///< \brief Current process handle.

            bool has_symbols_;                              ///< \brief Whether the symbols for this process were loaded correctly.

        };

        ///< \brief Utility type definition so that the code can refer to "platform::PlatformDebugger" without having to know the actual concrete class.
        using PlatformDebugger = WindowsDebugger;

        /// \brief Exposes methods to query system's capabilities under Windows OS.
        /// \author Raffaele D. Facendola 
        class WindowsSystem : public System
        {
        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance;
            static System& GetInstance();

            virtual CPUInfo GetCPUInfo() const override;

            virtual StorageInfo GetStorageInfo() const override;

            virtual MemoryInfo GetMemoryInfo() const override;

            virtual DisplayInfo GetDisplayInfo() const override;

            virtual PlatformInfo GetPlatformInfo() const override;
        };

        ///< \brief Utility type definition so that the code can refer to "platform::PlatformSystem" without having to know the actual concrete class.
        using PlatformSystem = WindowsSystem;

    }
}

#endif
