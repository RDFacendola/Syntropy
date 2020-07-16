#include "syntropy/allocators/linear_virtual_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* LINEAR VIRTUAL MEMORY RESOURCE                                       */
    /************************************************************************/

    RWByteSpan LinearVirtualMemoryResource::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto aligned_free = Align(free_, alignment); Size(aligned_free) >= size)
        {
            auto block = First(aligned_free, ToInt(size));

            free_ = PopFront(aligned_free, ToInt(size));

            auto commit_head = Align(block, granularity_).GetData();
            auto commit_tail = Align(free_, granularity_).GetData();

            VirtualMemory::Commit({ commit_head, commit_tail });

            return block;
        }

        return {};          // Out-of-memory.
    }

}