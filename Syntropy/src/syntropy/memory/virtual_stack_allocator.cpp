#include "syntropy/memory/virtual_stack_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL STACK ALLOCATOR                                              */
    /************************************************************************/

    RWByteSpan VirtualStackAllocator::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = Reserve(size, alignment))
        {
            auto commit_head = Memory::Align(block, granularity_).GetData();
            auto commit_tail = Memory::Align(virtual_unallocated_, granularity_).GetData();

            Memory::Commit({ commit_head, commit_tail });

            return block;
        }

        return {};          // Out-of-memory.
    }

    RWByteSpan VirtualStackAllocator::Reserve(Bytes size, Alignment alignment) noexcept
    {
        if (auto aligned_free = Memory::Align(virtual_unallocated_, alignment); Memory::Size(aligned_free) >= size)
        {
            auto block = Front(aligned_free, ToInt(size));

            virtual_unallocated_ = PopFront(aligned_free, ToInt(size));

            return block;
        }

        return {};          // Out-of-memory.
    }

}