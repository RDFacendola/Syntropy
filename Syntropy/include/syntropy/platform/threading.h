/// \file threading.h
/// \brief This header is part of the Syntropy platform module. It contains threading and scheduler-related classes and functionalities.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <thread>

#include "syntropy/core/types.h"
#include "syntropy/containers/vector.h"

namespace syntropy
{
    /************************************************************************/
    /* THREAD PRIORITY                                                      */
    /************************************************************************/

    /// \brief Define the priority of a thread.
    /// \author Raffaele D. Facendola - November 2017
    enum class ThreadPriority
    {
        /// \brief Lowest priority. May only run when the process is idling.
        kLowest,

        /// \brief Priority for background tasks.
        kLower,

        /// \brief Priority for low-priority threads.
        kLow,

        /// \brief Normal priority. Best suited for most threads.
        kNormal,

        /// \brief Priority for high-priority threads.
        kHigh,

        ///< \brief Priority for real-time threads.
        kHigher,

        /// \brief Highest priority. May interfere with basic OS threads (input, disk, ...).
        kHighest
    };

    /************************************************************************/
    /* AFFINITY MASK                                                        */
    /************************************************************************/

    /// \brief Type used to specify an affinity mask for both threads and processes.
    /// Each element represents the affinity for a particular core.
    using AffinityMask = Vector<bool>;

    /************************************************************************/
    /* THREADING                                                            */
    /************************************************************************/

    /// \brief Exposes threading and scheduler's functionalities.
    /// \author Raffaele D. Facendola - November 2017
    namespace Threading
    {
        /// \brief Get the index of the CPU on which the calling thread is running.
        /// \return Returns the index of the CPU on which the calling thread is running.
        Int GetCPUIndex();

        /// \brief Get the cores the calling process is allowed to run on.
        /// This method returns the cores a process can specify an affinity for. To get the actual affinity use GetProcessAffinity().
        /// \return Returns the cores the calling process is allowed to run on.
        AffinityMask GetSystemAffinity();

        /// \brief Set the cores the calling process can be run on.
        /// \param affinity_mask New process affinity. Must be a subset of the affinity returned by GetSystemAffinity().
        /// \return Returns true if the method succeeded, returns false otherwise.
        bool SetProcessAffinity(const AffinityMask& affinity_mask);

        /// \brief Get the cores the calling process can be run on.
        /// \return Returns the cores the calling process can be run on.
        AffinityMask GetProcessAffinity();

        /// \brief Set the cores a thread can be run on.
        /// \param affinity_mask New thread affinity. Must be a subset of the affinity returned by GetProcessAffinity().
        /// \param thread Thread to change the affinity of. If this parameter is nullptr, the calling thread will be used.
        /// \return Returns true if the method succeeded, returns false otherwise.
        bool SetThreadAffinity(const AffinityMask& affinity_mask, std::thread* thread = nullptr);

        /// \brief Get the cores a thread can be run on.
        /// \param thread Thread to get the affinity of. If this parameter is nullptr, the calling thread will be used.
        /// \return Return the cores the specified thread can be run on.
        AffinityMask GetThreadAffinity(std::thread* thread = nullptr);

        /// \brief Set the priority of a thread.
        /// \param priority New priority for the thread.
        /// \param thread Thread to change the priority of. If this parameter is nullptr, the calling thread will be used.
        /// \return Returns true if the method succeeded, returns false otherwise.
        bool SetThreadPriority(ThreadPriority priority, std::thread* thread = nullptr);

        /// \brief Get the priority of a thread.
        /// \param thread Thread to get the priority of. If this parameter is nullptr, the calling thread will be used.
        /// \return Return the priority of the specified thread.
        ThreadPriority GetThreadPriority(std::thread* thread = nullptr);
    };

}