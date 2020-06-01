#include "syntropy/platform/threading.h"

#include "syntropy/hal/hal_threading.h"

namespace syntropy
{
    /************************************************************************/
    /* THREADING                                                            */
    /************************************************************************/

    Int Threading::GetCPUIndex()
    {
        return HALThreading::GetCPUIndex();
    }

    AffinityMask Threading::GetSystemAffinity()
    {
        return HALThreading::GetSystemAffinity();
    }

    Bool Threading::SetProcessAffinity(const AffinityMask& affinity_mask)
    {
        return HALThreading::SetProcessAffinity(affinity_mask);
    }

    AffinityMask Threading::GetProcessAffinity()
    {
        return HALThreading::GetProcessAffinity();
    }

    Bool Threading::SetThreadAffinity(const AffinityMask& affinity_mask, std::thread* thread)
    {
        return HALThreading::SetThreadAffinity(affinity_mask, thread);
    }

    AffinityMask Threading::GetThreadAffinity(std::thread* thread)
    {
        return HALThreading::GetThreadAffinity(thread);
    }

    Bool Threading::SetThreadPriority(ThreadPriority priority, std::thread* thread)
    {
        return HALThreading::SetThreadPriority(priority, thread);
    }

    ThreadPriority Threading::GetThreadPriority(std::thread* thread)
    {
        return HALThreading::GetThreadPriority(thread);
    }

}