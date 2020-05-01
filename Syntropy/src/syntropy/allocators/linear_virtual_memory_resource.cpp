#include "syntropy/allocators/linear_virtual_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* LINEAR VIRTUAL MEMORY RESOURCE                                       */
    /************************************************************************/

    MemoryRange LinearVirtualMemoryResource::Allocate(Bytes size, Alignment alignment) noexcept
    {
        auto block_head = head_.GetAligned(alignment);
        auto block_tail = block_head + size;

        if (block_tail <= virtual_memory_.End())
        {
            auto block = MemoryRange{ block_head, block_tail };

            auto commit_head = head_.GetAligned(granularity_);
            auto commit_tail = block_tail.GetAligned(granularity_);

            VirtualMemory::Commit({ commit_head, commit_tail });

            head_ = block_tail;

            return block;
        }

        return {};          // Out-of-memory.
    }

}