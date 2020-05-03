#define SYNTROPY_OS_INCLUDE_GUARD

#include "syntropy/platform/os/windows_os.h"

#undef SYNTROPY_OS_INCLUDE_GUARD

#ifdef _WIN64

#pragma comment(lib, "DbgHelp.lib")

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#undef max

#pragma warning(pop)

#include "syntropy/types/string.h"
#include <mutex>
#include <thread>
#include <algorithm>
#include <memory>
#include "syntropy/containers/map.h"

#include "syntropy/math/math.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/virtual_memory_page.h"
#include "syntropy/diagnostics/assert.h"

#include "syntropy/syntropy.h"

namespace syntropy::platform
{
    /************************************************************************/
    /* PLATFORM THREADING                                                   */
    /************************************************************************/

    size_t PlatformThreading::GetCPUIndex()
    {
        // Group size based on: https://msdn.microsoft.com/en-us/library/windows/desktop/dd405503(v=vs.85).aspx
        static constexpr size_t kGroupSize = 64;

        PROCESSOR_NUMBER processor_number;

        GetCurrentProcessorNumberEx(&processor_number);

        return processor_number.Number + (processor_number.Group * kGroupSize);
    }

    AffinityMask PlatformThreading::GetSystemAffinity()
    {
        size_t process_affinity;
        size_t system_affinity;

        auto result = ::GetProcessAffinityMask(GetCurrentProcess(), &process_affinity, &system_affinity) != 0;

        return result ? system_affinity : size_t(0);
    }

    bool PlatformThreading::SetProcessAffinity(const AffinityMask& affinity_mask)
    {
        HANDLE process_handle = GetCurrentProcess();

        return SetProcessAffinityMask(process_handle, affinity_mask.to_ullong()) != 0;
    }

    AffinityMask PlatformThreading::GetProcessAffinity()
    {
        size_t process_affinity;
        size_t system_affinity;

        auto result = GetProcessAffinityMask(GetCurrentProcess(), &process_affinity, &system_affinity) != 0;

        return result ? process_affinity : size_t(0);
    }

    bool PlatformThreading::SetThreadAffinity(const AffinityMask& affinity_mask, std::thread* thread)
    {
        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        return SetThreadAffinityMask(thread_handle, affinity_mask.to_ullong()) != 0;
    }

    AffinityMask PlatformThreading::GetThreadAffinity(std::thread* thread)
    {
        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        auto process_affinity = GetProcessAffinity();

        auto affinity_mask = SetThreadAffinityMask(thread_handle, process_affinity.to_ullong());        // Returns the old affinity mask.

        SetThreadAffinityMask(thread_handle, affinity_mask);                                            // Restore the original affinity mask. That API, though.

        return affinity_mask;
    }

    bool PlatformThreading::SetThreadPriority(ThreadPriority priority, std::thread* thread)
    {
        static Map<ThreadPriority, int> priority_table = 
        {
            { ThreadPriority::kLowest, THREAD_PRIORITY_IDLE },
            { ThreadPriority::kLower, THREAD_PRIORITY_LOWEST },
            { ThreadPriority::kLow, THREAD_PRIORITY_BELOW_NORMAL },
            { ThreadPriority::kNormal, THREAD_PRIORITY_NORMAL },
            { ThreadPriority::kHigh, THREAD_PRIORITY_ABOVE_NORMAL },
            { ThreadPriority::kHigher, THREAD_PRIORITY_HIGHEST },
            { ThreadPriority::kHighest, THREAD_PRIORITY_TIME_CRITICAL }
        };

        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        return ::SetThreadPriority(thread_handle, priority_table[priority]) != 0;
    }

    ThreadPriority PlatformThreading::GetThreadPriority(std::thread* thread)
    {
        static Map<int, ThreadPriority> priority_table =
        {
            { THREAD_PRIORITY_IDLE, ThreadPriority::kLowest },
            { THREAD_PRIORITY_LOWEST, ThreadPriority::kLower },
            { THREAD_PRIORITY_BELOW_NORMAL, ThreadPriority::kLow },
            { THREAD_PRIORITY_NORMAL, ThreadPriority::kNormal },
            { THREAD_PRIORITY_ABOVE_NORMAL, ThreadPriority::kHigh },
            { THREAD_PRIORITY_HIGHEST, ThreadPriority::kHigher },
            { THREAD_PRIORITY_TIME_CRITICAL, ThreadPriority::kHighest }
        };

        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        auto priority = ::GetThreadPriority(thread_handle);

        return priority_table[priority];
    }

}

#endif
