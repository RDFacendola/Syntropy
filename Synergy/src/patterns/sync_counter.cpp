#include "patterns/sync_counter.h"

#include "diagnostics/assert.h"

namespace syntropy::synergy
{
    /************************************************************************/
    /* SYNC COUNTER                                                         */
    /************************************************************************/

    SyncCounter::SyncCounter(size_t count)
    {
        count_.store(count, std::memory_order_release);
    }

    void SyncCounter::Reset(size_t count)
    {
        size_t zero = 0;

        auto result = count_.compare_exchange_weak(zero, count, std::memory_order_acq_rel);

        SYNTROPY_ASSERT(result);                    // This check won't cover any case, but it may be enough in practice.
    }

    void SyncCounter::Signal(bool wait)
    {
        auto previous_count = count_.fetch_sub(1, std::memory_order_acq_rel);

        SYNTROPY_ASSERT(previous_count > 0);        // The counter was decremented too much!

        if (previous_count == 1)
        {
            wait_.notify_all();                     // Notify everyone and return.
        }
        else if(wait)
        {
            Wait();                                 // Wait until the counter reaches zero.
        }
    }

    void SyncCounter::Wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        wait_.wait(lock, [this]
        {
            return count_.load(std::memory_order_acquire) == 0;
        });
    }

    /************************************************************************/
    /* SYNC COUNTER GUARD                                                   */
    /************************************************************************/

    SyncCounterGuard::SyncCounterGuard(SyncCounter& counter, bool wait)
        : counter_(counter)
        , wait_(wait)
    {

    }

    SyncCounterGuard::~SyncCounterGuard()
    {
        counter_.Signal(wait_);
    }

}
