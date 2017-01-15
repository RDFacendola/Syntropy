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
        class WindowsDebugger : public diagnostics::Debugger
        {
        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance.
            static diagnostics::Debugger& GetInstance();

            /// \brief Destructor.
            virtual ~WindowsDebugger() = default;

            virtual bool IsDebuggerAttached() const override;
            
            virtual diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller) const override;

        private:

            /// \brief Implementation details.
            struct Implementation;

            /// \brief Default constructor.
            WindowsDebugger();

            std::unique_ptr<Implementation> implementation_;    ///< \brief Opaque pointer to the implementation details

        };

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

        /// \brief Wraps the low-level calls used to handle virtual memory allocation under Windows OS.
        /// \author Raffaele D. Facendola - December 2016
        class WindowsMemory
        {
        public:

            /// \brief See Memory::GetPageSize
            static size_t GetPageSize();

            /// \brief See Memory::Allocate
            static void* Allocate(size_t size);

            /// \brief See Memory::Free
            static bool Free(void* address);

            /// \brief See Memory::Reserve
            static void* Reserve(size_t size);

            /// \brief See Memory::Commit
            static bool Commit(void* address, size_t size);

            /// \brief See Memory::Decommit
            static bool Decommit(void* address, size_t size);

        };

        ///< \brief Utility type definition so that the code can refer to "platform::PlatformDebugger" without having to know the actual concrete class.
        using PlatformDebugger = WindowsDebugger;

        ///< \brief Utility type definition so that the code can refer to "platform::PlatformSystem" without having to know the actual concrete class.
        using PlatformSystem = WindowsSystem;

        ///< \brief Utility type definition so that the code can refer to "platform::PlatformMemory" without having to know the actual concrete class.
        using PlatformMemory = WindowsMemory;

    }
}

#endif
