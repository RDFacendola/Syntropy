
/// \file sync_counter.h
/// \brief This header is part of the synergy synchronization primitives. It contains definition for classes used to synchronize threads using counters.
/// 
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace syntropy::synergy
{

    /************************************************************************/
    /* SYNC COUNTER                                                         */
    /************************************************************************/

    /// \brief Synchronization primitive used to synchronize a group of threads using an atomic, notifiable, counter.
    /// 
    /// \example 
    /// 
    /// SyncCounter counter(10);
    /// 
    /// for(size_t index = 0; index < 10; ++index)                  // Spawn a thread group.
    ///    std::thread([&counter]
    ///    {
    ///       ...some code A ...
    ///       counter.Signal();                                     // Synchronization point. Blocks execution.
    ///       ...some code given that each thread executed A...
    ///    }).detach();
    ///    
    /// counter.Wait();                                             // The caller waits for the threads to reach the synchronization point.
    /// ... do something given that each thread executed A...
    ///
    /// \author Raffaele D. Facendola - November 2017
    /// 
    /// \remarks See SyncCounterGuard for a more convenient and safe way of notifying the counter.
    class SyncCounter
    {
    public:

        /// \brief Create a new synchronization counter.
        /// \param count Initial counter value.
        SyncCounter(size_t count = 0);

        /// \brief Reset the counter.
        /// This method must be called only when there's no other thread to wait.
        void Reset(size_t count);

        /// \brief Reduce the counter by one and notify each waiting thread when the counter drops to zero.
        /// \param wait Whether to wait for the counter to drop to zero before proceeding.
        void Signal(bool wait = true);

        /// \brief Wait until the counter drops to zero.
        void Wait();

    private:

        mutable std::mutex mutex_;          ///< \brief Used for synchronization.

        std::condition_variable wait_;      ///< \brief Condition variable used to wait on the counter to reach 0.

        std::atomic_size_t count_;          ///< \brief Current number of threads to wait for.
    };

    /************************************************************************/
    /* SYNC COUNTER GUARD                                                   */
    /************************************************************************/

    /// \brief Guard used to notify a SyncCounter when going out of scope.
    /// \author Raffaele D. Facendola - November 2017
    struct SyncCounterGuard
    {
        /// \brief Create a new sync counter guard that reduces the counter by one upon destruction.
        /// \param counter Synchronization counter this guard refers to.
        /// \param wait Whether to wait for the counter to drop to zero before leaving the scope.
        SyncCounterGuard(SyncCounter& counter, bool wait = false);

        /// \brief Decrement the counter and eventually waits for it to drop to zero before leaving the scope.
        ~SyncCounterGuard();

    private:

        SyncCounter& counter_;              ///< \brief Synchronization counter this guard refers to.

        bool wait_;                         ///< \brief Whether to wait for the counter to drop to zero before leaving the scope.
    };

}

