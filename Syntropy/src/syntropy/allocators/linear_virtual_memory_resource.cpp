#include "syntropy/allocators/linear_virtual_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* LINEAR VIRTUAL MEMORY RESOURCE                                       */
    /************************************************************************/

    RWByteSpan LinearVirtualMemoryResource::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto aligned_free = Memory::Align(free_, alignment); Memory::Size(aligned_free) >= size)
        {
            auto block = Front(aligned_free, ToInt(size));

            free_ = PopFront(aligned_free, ToInt(size));

            auto commit_head = Memory::Align(block, granularity_).GetData();
            auto commit_tail = Memory::Align(free_, granularity_).GetData();

            VirtualMemory::Commit({ commit_head, commit_tail });

            return block;
        }

        return {};          // Out-of-memory.
    }

}