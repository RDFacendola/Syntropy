
/// \file system.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains threading and scheduler-related classes and functionalities.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <thread>

namespace syntropy::platform
{

    /// \brief Define the priority of a thread.
    /// \author Raffaele D. Facendola - November 2017
    enum class ThreadPriority
    {
        kLowest,            ///< \brief Lowest priority. May only run when the process is idling.
        kLower,             ///< \brief Priority for background tasks.
        kLow,               ///< \brief Priority for low-priority threads.
        kNormal,            ///< \brief Normal priority. Best suited for most threads.
        kHigh,              ///< \brief Priority for high-priority threads.
        kHigher,            ///< \brief Priority for real-time threads.
        kHighest            ///< \brief Highest priority. May interfere with basic OS threads (input, disk, ...).
    };

    /************************************************************************/
    /* THREADING                                                            */
    /************************************************************************/

    /// \brief Exposes threading and scheduler's functionalities.
    /// \author Raffaele D. Facendola - November 2017
    class Threading
    {
    public:

        /// \brief Get the index of the CPU on which the calling thread is running.
        /// \return Returns the index of the CPU on which the calling thread is running.
        static size_t GetCPUIndex();

        /// \brief Change a thread's core affinity.
        /// \param affinity_mask Affinity mask, one bit per core.
        /// \param thread Thread to change the affinity of. nullptr refers to the calling thread.
        /// \return Returns true if the method succeeded, returns false otherwise.
        static bool SetThreadAffinity(size_t affinity_mask, std::thread* thread = nullptr);

        /// \brief Get a thread's core affinity.
        /// \param thread Thread to get the affinity of. nullptr refers to the calling thread.
        /// \return Return the thread's core affinity.
        static size_t GetThreadAffinity(std::thread* thread = nullptr);

        /// \brief Change a thread's priority.
        /// \param priority New priority for the thread.
        /// \param thread Thread to change the priority of. nullptr refers to the calling thread.
        /// \return Returns true if the method succeeded, returns false otherwise.
        static bool SetThreadPriority(ThreadPriority priority, std::thread* thread = nullptr);

        /// \brief Get a thread's priority.
        /// \param thread Thread to get the priority of. nullptr refers to the calling thread.
        /// \return Returns the thread's priority.
        static ThreadPriority GetThreadPriority(std::thread* thread = nullptr);
    };

}