#include "syntropy/allocators/virtual_stack_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL STACK ALLOCATOR                                              */
    /************************************************************************/

    RWByteSpan VirtualStackAllocator::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto virtual_span = Memory::Align(free_span_, alignment); Memory::Size(virtual_span) >= size)
        {
            auto [block, free_span] = SliceFront(virtual_span, ToInt(size));

            free_span_ = free_span;

            if (auto intersection_span = Intersection(committed_span_, block); intersection_span != block)                          // Portion of the block that was already committed.
            {
                auto uncommitted_span = PopFront(virtual_span_.GetData(), ToInt(Memory::Size(committed_span_)));

                auto commit_size = Math::Ceil(size - Memory::Size(intersection_span), granularity_);                                // Round at granularity boundary to reduce overall kernel calls.

                auto commit_span = Front(uncommitted_span, ToInt(commit_size));

                Memory::VirtualCommit(commit_span);                                                                                 // Kernel call.

                committed_span_ = Union(committed_span_, commit_span);
            }

            return block;
        }

        return {};          // Out-of-memory.
    }

    RWByteSpan VirtualStackAllocator::Reserve(Bytes size, Alignment alignment) noexcept
    {
        auto virtual_size = Memory::VirtualCeil(size);
        auto virtual_alignment = Memory::VirtualCeil(alignment);

        if (auto virtual_span = Memory::Align(free_span_, virtual_alignment); Memory::Size(virtual_span) >= virtual_size)           // Jump next to a virtual page to avoid interference with previous allocations\reservations.
        {
            auto [block, free_span] = SliceFront(virtual_span, ToInt(virtual_size));

            free_span_ = free_span;

            committed_span_ = Union(committed_span_, block);                                                                        // Jump after the reserved block to avoid interference with following allocations\reservations.

            return Front(block, ToInt(size));
        }

        return {};          // Out-of-memory.
    }

}