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

#include "syntropy/diagnostics/diagnostics.h"

#include "syntropy/platform/system.h"
#include "syntropy/platform/threading.h"

#include "syntropy/memory/memory_range.h"

#include <thread>

namespace syntropy::platform
{
    /************************************************************************/
    /* PLATFORM DEBUGGER                                                    */
    /************************************************************************/

    /// \brief Exposes Windows-specific debugging functionalities under Windows OS.
    /// \author Raffaele D. Facendola - December 2016
    class PlatformDebugger
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

    /************************************************************************/
    /* PLATFORM SYSTEM                                                      */
    /************************************************************************/

    /// \brief Exposes methods to query system's capabilities under Windows OS.
    /// \author Raffaele D. Facendola 
    class PlatformSystem
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

    /************************************************************************/
    /* PLATFORM THREADING                                                   */
    /************************************************************************/

    /// \brief Exposes threading and scheduler's functionalities under Windows OS.
    /// \author Raffaele D. Facendola - November 2017
    class PlatformThreading
    {
    public:

        /// \brief Get the index of the CPU on which the calling thread is running.
        /// \return Returns the index of the CPU on which the calling thread is running.
        static size_t GetCPUIndex();

        /// \brief Get the cores the calling process is allowed to run on.
        /// This method returns the cores a process can specify an affinity for. To get the actual affinity use GetProcessAffinity().
        /// \return Returns the cores the calling process is allowed to run on.
        static AffinityMask GetSystemAffinity();

        /// \brief Set the cores the calling process can be run on.
        /// \param affinity_mask New process affinity. Must be a subset of the affinity returned by GetSystemAffinity().
        /// \return Returns true if the method succeeded, returns false otherwise.
        static bool SetProcessAffinity(const AffinityMask& affinity_mask);

        /// \brief Get the cores the calling process can be run on.
        /// \return Returns the cores the calling process can be run on.
        static AffinityMask GetProcessAffinity();

        /// \brief Set the cores a thread can be run on.
        /// \param affinity_mask New thread affinity. Must be a subset of the affinity returned by GetProcessAffinity().
        /// \param thread Thread to change the affinity of. If this parameter is nullptr, the calling thread will be used.
        /// \return Returns true if the method succeeded, returns false otherwise.
        static bool SetThreadAffinity(const AffinityMask& affinity_mask, std::thread* thread = nullptr);

        /// \brief Get the cores a thread can be run on.
        /// \param thread Thread to get the affinity of. If this parameter is nullptr, the calling thread will be used.
        /// \return Return the cores the specified thread can be run on.
        static AffinityMask GetThreadAffinity(std::thread* thread = nullptr);

        /// \brief Set the priority of a thread.
        /// \param priority New priority for the thread.
        /// \param thread Thread to change the priority of. If this parameter is nullptr, the calling thread will be used.
        /// \return Returns true if the method succeeded, returns false otherwise.
        static bool SetThreadPriority(ThreadPriority priority, std::thread* thread = nullptr);

        /// \brief Get the priority of a thread.
        /// \param thread Thread to get the priority of. If this parameter is nullptr, the calling thread will be used.
        /// \return Return the priority of the specified thread.
        static ThreadPriority GetThreadPriority(std::thread* thread = nullptr);
    };

    /************************************************************************/
    /* PLATFORM MEMORY                                                      */
    /************************************************************************/

    /// \brief Wraps the low-level calls used to handle virtual memory allocation under Windows OS.
    /// \author Raffaele D. Facendola - December 2016
    class PlatformMemory
    {
    public:

        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size, in bytes.
        static Bytes GetPageSize();

        /// \brief Get the virtual memory page alignment.
        /// \return Returns the virtual memory page alignment, in bytes.
        static Alignment GetPageAlignment();

        /// \brief Reserve a range of virtual memory addresses.
        /// Reserved memory region must be committed via Commit() before accessing it.
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved memory range. If the method fails returns an empty range.
        static MemoryRange Reserve(Bytes size);

        /// \brief Allocate a range of virtual memory addresses.
        /// This method has the same effect as a Reserve() followed by a Commit().
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved virtual memory range. If the method fails returns an empty range.
        static MemoryRange Allocate(Bytes size);

        /// \brief Release a range of virtual memory addresses.
        /// \param address First address of the range to release. Must match any return value of a previous Reserve() / Allocate(), otherwise the behaviour is unspecified.
        /// \return Returns true if the range could be released, returns false otherwise.
        static bool Release(const MemoryRange& memory_range);

        /// \brief Commit a reserved virtual memory block.
        /// This method allocates all the pages containing at least one byte in the provided range and makes them accessible by the application.
        /// \param memory_range Memory range to commit.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        /// \remarks The provided memory range must refer to a memory region that was previously reserved via Reserve().
        static bool Commit(const MemoryRange& memory_range);

        /// \brief Decommit a virtual memory block.
        /// This method decommits all the pages containing at least one byte in the provided range.
        /// \param memory_range Memory range to decommit.
        static bool Decommit(const MemoryRange& memory_range);

    };
}

#endif
