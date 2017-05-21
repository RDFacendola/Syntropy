/// \file windows_platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains Windows-specific functionalities.
///
/// Do not include this header directly. Use os.h instead.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifndef SYNTROPY_OS_INCLUDE_GUARD
#error "You may not include this header directly. Use os.h instead."
#endif

#ifdef _WIN64

#include <memory>

#include "diagnostics/debug.h"
#include "platform/system.h"
#include "memory/memory.h"

namespace syntropy
{
    namespace platform
    {

        /// \brief Exposes Windows-specific debugging functionalities under Windows OS.
        /// \author Raffaele D. Facendola - December 2016
        class PlatformDebugger : public diagnostics::Debugger
        {
        public:

            /// \brief Check whether the debugger is attached.
            /// \return Returns true if a debugger is attached to the application, returns false otherwise.
            static bool IsDebuggerAttached();

            /// \brief Get the stack trace of the current thread.
            /// \param caller Stack trace element representing the code that called this method.
            /// \return Returns the stack trace whose head is caller.
            static diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller);
        };

        /// \brief Exposes methods to query system's capabilities under Windows OS.
        /// \author Raffaele D. Facendola 
        class PlatformSystem : public System
        {
        public:

            /// \brief Get the current CPU infos.
            /// \return Returns the current CPU infos.
            static CPUInfo GetCPUInfo();

            /// \brief Get the current storage infos.
            /// \return Returns the current storage infos.
            static StorageInfo GetStorageInfo();

            /// \brief Get the current memory infos.
            /// \return Returns the current memory infos.
            static MemoryInfo GetMemoryInfo();

            /// \brief Get the current desktop infos.
            /// \return Returns the current desktop infos.
            static DisplayInfo GetDisplayInfo();

            /// \brief Get the current platform infos.
            /// \return Returns the current platform infos.
            static PlatformInfo GetPlatformInfo();
        };

        /// \brief Wraps the low-level calls used to handle virtual memory allocation under Windows OS.
        /// \author Raffaele D. Facendola - December 2016
        class PlatformMemory
        {
        public:

            /// \brief See Memory::GetPageSize
            static size_t GetPageSize();

            /// \brief See Memory::Allocate
            static void* Allocate(size_t size);

            /// \brief See Memory::Release
            static bool Release(void* address);

            /// \brief See Memory::Reserve
            static void* Reserve(size_t size);

            /// \brief See Memory::Commit
            static bool Commit(void* address, size_t size);

            /// \brief See Memory::Decommit
            static bool Decommit(void* address, size_t size);

        };

    }
}

#endif
