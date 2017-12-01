
/// \file system.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains threading and scheduler-related classes and functionalities.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <thread>
#include <bitset>

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

    /// \brief Type used to specify an affinity mask for both threads and processes.
    /// Each bit represents the affinity for a particular core.
    using AffinityMask = std::bitset<64>;

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

}