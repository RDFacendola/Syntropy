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

    bool Threading::SetThreadAffinity(size_t affinity_mask, std::thread* thread)
    {
        return PlatformThreading::SetThreadAffinity(affinity_mask, thread);
    }

    size_t Threading::GetThreadAffinity(std::thread* thread)
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