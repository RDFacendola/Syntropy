#include "platform/threading.h"

#include "platform/os/os.h"

namespace syntropy::platform
{
    /************************************************************************/
    /* THREADING                                                            */
    /************************************************************************/

    size_t Threading::GetCPUIndex()
    {
        return PlatformThreading::GetCPUIndex();
    }

    AffinityMask Threading::GetSystemAffinity()
    {
        return PlatformThreading::GetSystemAffinity();
    }

    bool Threading::SetProcessAffinity(const AffinityMask& affinity_mask)
    {
        return PlatformThreading::SetProcessAffinity(affinity_mask);
    }

    AffinityMask Threading::GetProcessAffinity()
    {
        return PlatformThreading::GetProcessAffinity();
    }

    bool Threading::SetThreadAffinity(const AffinityMask& affinity_mask, std::thread* thread)
    {
        return PlatformThreading::SetThreadAffinity(affinity_mask, thread);
    }

    AffinityMask Threading::GetThreadAffinity(std::thread* thread)
    {
        return PlatformThreading::GetThreadAffinity(thread);
    }

    bool Threading::SetThreadPriority(ThreadPriority priority, std::thread* thread)
    {
        return PlatformThreading::SetThreadPriority(priority, thread);
    }

    ThreadPriority Threading::GetThreadPriority(std::thread* thread)
    {
        return PlatformThreading::GetThreadPriority(thread);
    }
}