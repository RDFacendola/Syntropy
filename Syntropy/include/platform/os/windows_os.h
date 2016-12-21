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
            /// \return Returns the singleton instance;
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

        class WindowsMemory : public Memory
        {
        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance;
            static Memory& GetInstance();

            virtual size_t GetPageSize() const override;

            virtual size_t GetAllocationGranularity() const override;

            virtual VirtualMemoryRange ReserveVirtualRange(size_t count) override;

            virtual bool ReleaseVirtualRange(const VirtualMemoryRange& range) override;

            virtual MemoryBlock AllocMemoryBlock(const VirtualMemoryRange& range, size_t offset, size_t size) override;

            virtual MemoryBlock FreeMemoryBlock(const VirtualMemoryRange& range, size_t offset, size_t size) override;

            virtual bool FreeMemoryBlock(const MemoryBlock& block) override;

        private:

            /// \brief Default constructor.
            WindowsMemory();

            VirtualMemoryInfo virtual_memory_info_;         ///< \brief Virtual memory info.

            size_t page_size_;                              ///< \brief Memory page size, in bytes.

            size_t allocation_granularity_;                 ///< \brief Memory allocation granularity, in bytes.

        };

        ///< \brief Utility type definition so that the code can refer to "platform::PlatformMemory" without having to know the actual concrete class.
        using PlatformMemory = WindowsMemory;

    }
}

#endif
