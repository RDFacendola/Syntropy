/// \file windows_platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains Windows-specific functionalities.
///
/// Do not use this header directly!
///
/// \author Raffaele D. Facendola - 2016

#pragma once

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

            static bool IsDebuggerAttached();
            
            static diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller);
        };

        /// \brief Exposes methods to query system's capabilities under Windows OS.
        /// \author Raffaele D. Facendola 
        class PlatformSystem : public System
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
