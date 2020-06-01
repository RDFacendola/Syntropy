#ifdef _WIN64

#include "syntropy/hal/hal_threading.h"

/************************************************************************/
/* HEADERS & LIBRARIES                                                  */
/************************************************************************/

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#undef max

#pragma warning(pop)

namespace syntropy
{
    /************************************************************************/
    /* WINDOWS THREADING                                                    */
    /************************************************************************/

    /// \brief Exposes threading-related data under Windows.
    namespace WindowsThreading
    {
        /// \brief Size of a processor group.
        /// \see Group size based on: https://msdn.microsoft.com/en-us/library/windows/desktop/dd405503(v=vs.85).aspx
        constexpr auto kProcessorGroupSize = 64;

        /// \brief Convert affinity mask from Windows to Syntropy.
        AffinityMask ToAffinityMask(std::size_t affinity_mask);

        /// \brief Convert affinity mask from Syntropy to Windows.
        std::size_t FromAffinityMask(const AffinityMask& affinity_mask);

    };

    // Implementation.

    AffinityMask WindowsThreading::ToAffinityMask(std::size_t affinity_mask_from)
    {
        auto affinity_mask_to = AffinityMask{};

        affinity_mask_to.reserve(WindowsThreading::kProcessorGroupSize);

        for (; affinity_mask_from; affinity_mask_from >>= 1)
        {
            affinity_mask_to.emplace_back((affinity_mask_from & 0b1) ? 1 : 0);
        }

        return affinity_mask_to;
    }

    std::size_t WindowsThreading::FromAffinityMask(const AffinityMask& affinity_mask_to)
    {
        auto affinity_mask_from = std::size_t{ 0 };

        for (auto&& affinity_bit : affinity_mask_to)
        {
            affinity_mask_from <<= 1;
            affinity_mask_from |= affinity_bit & 0b1;
        }

        return affinity_mask_from;
    }

    /************************************************************************/
    /* HAL THREADING                                                        */
    /************************************************************************/

    Int HALThreading::GetCPUIndex()
    {
        auto processor_number = PROCESSOR_NUMBER{};

        GetCurrentProcessorNumberEx(&processor_number);

        return processor_number.Number + (processor_number.Group * WindowsThreading::kProcessorGroupSize);
    }

    AffinityMask HALThreading::GetSystemAffinity()
    {
        auto process_affinity = std::size_t{};
        auto system_affinity = std::size_t{};

        if (auto result = ::GetProcessAffinityMask(GetCurrentProcess(), &process_affinity, &system_affinity); result != 0)
        {
            return WindowsThreading::ToAffinityMask(system_affinity);
        }

        return {};
    }

    Bool HALThreading::SetProcessAffinity(const AffinityMask& affinity_mask)
    {
        auto process_handle = GetCurrentProcess();

        auto process_affinity = WindowsThreading::FromAffinityMask(affinity_mask);

        return SetProcessAffinityMask(process_handle, process_affinity) != 0;
    }

    AffinityMask HALThreading::GetProcessAffinity()
    {
        auto process_affinity = std::size_t{};
        auto system_affinity = std::size_t{};

        if (auto result = ::GetProcessAffinityMask(GetCurrentProcess(), &process_affinity, &system_affinity); result != 0)
        {
            return WindowsThreading::ToAffinityMask(process_affinity);
        }

        return {};
    }

    Bool HALThreading::SetThreadAffinity(const AffinityMask& affinity_mask, std::thread* thread)
    {
        auto thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        auto thread_affinity = WindowsThreading::FromAffinityMask(affinity_mask);

        return SetThreadAffinityMask(thread_handle, thread_affinity) != 0;
    }

    AffinityMask HALThreading::GetThreadAffinity(std::thread* thread)
    {
        auto process_affinity = std::size_t{};
        auto system_affinity = std::size_t{};

        ::GetProcessAffinityMask(GetCurrentProcess(), &process_affinity, &system_affinity);

        auto thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        auto affinity_mask = SetThreadAffinityMask(thread_handle, process_affinity);        // Returns the old affinity mask.

        SetThreadAffinityMask(thread_handle, affinity_mask);                                // Restore the original affinity mask. That API, though.

        return WindowsThreading::ToAffinityMask(affinity_mask);
    }

    Bool HALThreading::SetThreadPriority(ThreadPriority priority, std::thread* thread)
    {
        auto thread_priority = int{};

        if (priority == ThreadPriority::kLowest)
        {
            thread_priority = THREAD_PRIORITY_IDLE;
        }
        else if (priority == ThreadPriority::kLower)
        {
            thread_priority = THREAD_PRIORITY_LOWEST;
        }
        else if (priority == ThreadPriority::kLow)
        {
            thread_priority = THREAD_PRIORITY_BELOW_NORMAL;
        }
        else if (priority == ThreadPriority::kNormal)
        {
            thread_priority = THREAD_PRIORITY_NORMAL;
        }
        else if (priority == ThreadPriority::kHigh)
        {
            thread_priority = THREAD_PRIORITY_ABOVE_NORMAL;
        }
        else if (priority == ThreadPriority::kHigher)
        {
            thread_priority = THREAD_PRIORITY_HIGHEST;
        }
        else if (priority == ThreadPriority::kHighest)
        {
            thread_priority = THREAD_PRIORITY_TIME_CRITICAL;
        }

        auto thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        return ::SetThreadPriority(thread_handle, thread_priority);
    }

    ThreadPriority HALThreading::GetThreadPriority(std::thread* thread)
    {
        HANDLE thread_handle = thread ? thread->native_handle() : GetCurrentThread();

        auto thread_priority = ::GetThreadPriority(thread_handle);

        auto priority = ThreadPriority{};

        if (thread_priority == THREAD_PRIORITY_IDLE)
        {
            priority = ThreadPriority::kLowest;
        }
        else if (thread_priority == THREAD_PRIORITY_LOWEST)
        {
            priority = ThreadPriority::kLower;
        }
        else if (thread_priority == THREAD_PRIORITY_BELOW_NORMAL)
        {
            priority = ThreadPriority::kLow;
        }
        else if (thread_priority == THREAD_PRIORITY_NORMAL)
        {
            priority = ThreadPriority::kNormal;
        }
        else if (thread_priority == THREAD_PRIORITY_ABOVE_NORMAL)
        {
            priority = ThreadPriority::kHigh;
        }
        else if (thread_priority == THREAD_PRIORITY_HIGHEST)
        {
            priority = ThreadPriority::kHigher;
        }
        else if (thread_priority == THREAD_PRIORITY_TIME_CRITICAL)
        {
            priority = ThreadPriority::kHighest;
        }

        return priority;
    }

}

#endif