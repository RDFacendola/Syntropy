#include "patterns/sync_counter.h"

namespace syntropy::synergy
{
    /************************************************************************/
    /* SYNC COUNTER                                                         */
    /************************************************************************/

    SyncCounter::SyncCounter(size_t count)
    {
        count_.store(count, std::memory_order_release);
    }

    void SyncCounter::Decrement(bool wait)
    {
        if (count_.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            wait_.notify_all();         // Notify everyone and return.
        }
        else if(wait)
        {
            Wait();                     // Wait until the counter reaches zero.
        }
    }

    void SyncCounter::Wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        wait_.wait(lock, [this]
        {
            return count_.load(std::memory_order_relaxed) == 0;
        });
    }

    void SyncCounter::WaitAndReset(size_t count)
    {
        Wait();

        if (count != 0)
        {
            count_.store(count, std::memory_order_release);
        }
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
        counter_.Decrement(wait_);
    }

}
