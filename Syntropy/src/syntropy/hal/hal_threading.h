/// \file threading.h
/// \brief This header is part of the Syntropy hardware abstraction layer module. It contains threading and scheduler-related classes and functionalities.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <thread>

#include "syntropy/core/vector.h"
#include "syntropy/language/language.h"
#include "syntropy/platform/threading.h"

namespace Syntropy
{
    /************************************************************************/
    /* HAL THREADING                                                        */
    /************************************************************************/

    /// \brief Exposes threading and scheduler's functionalities.
    /// \author Raffaele D. Facendola - November 2017
    namespace HALThreading
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
        Bool SetProcessAffinity(const AffinityMask& affinity_mask);

        /// \brief Get the cores the calling process can be run on.
        /// \return Returns the cores the calling process can be run on.
        AffinityMask GetProcessAffinity();

        /// \brief Set the cores a thread can be run on.
        /// \param affinity_mask New thread affinity. Must be a subset of the affinity returned by GetProcessAffinity().
        /// \param thread Thread to change the affinity of. If this parameter is nullptr, the calling thread will be used.
        /// \return Returns true if the method succeeded, returns false otherwise.
        Bool SetThreadAffinity(const AffinityMask& affinity_mask, std::thread* thread = nullptr);

        /// \brief Get the cores a thread can be run on.
        /// \param thread Thread to get the affinity of. If this parameter is nullptr, the calling thread will be used.
        /// \return Return the cores the specified thread can be run on.
        AffinityMask GetThreadAffinity(std::thread* thread = nullptr);

        /// \brief Set the priority of a thread.
        /// \param priority New priority for the thread.
        /// \param thread Thread to change the priority of. If this parameter is nullptr, the calling thread will be used.
        /// \return Returns true if the method succeeded, returns false otherwise.
        Bool SetThreadPriority(ThreadPriority priority, std::thread* thread = nullptr);

        /// \brief Get the priority of a thread.
        /// \param thread Thread to get the priority of. If this parameter is nullptr, the calling thread will be used.
        /// \return Return the priority of the specified thread.
        ThreadPriority GetThreadPriority(std::thread* thread = nullptr);
    };

}