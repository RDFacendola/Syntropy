#include "syntropy/allocators/virtual_stack_allocator.h"

namespace Syntropy
{
    /************************************************************************/
    /* VIRTUAL STACK ALLOCATOR                                              */
    /************************************************************************/

    Memory::RWByteSpan VirtualStackAllocator::Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept
    {
        // Committing at higher granularity to reduce overall kernel calls.

        if (auto available_span = Memory::Align(unallocated_span_, size, alignment))
        {
            auto [block, unallocated_span] = Memory::SliceFront(available_span, size);

            auto commit_size = Math::Max(Memory::Size(uncommitted_span_) - Memory::Size(unallocated_span), Memory::ToBytes(0));

            auto [commit_span, uncommitted_span] = Memory::SliceFront(uncommitted_span_, commit_size);

            VirtualMemory::Commit(commit_span);         // Kernel call.

            unallocated_span_ = unallocated_span;
            uncommitted_span_ = uncommitted_span;

            return block;
        }

        return {};
    }

    Memory::RWByteSpan VirtualStackAllocator::Reserve(Memory::Bytes size, Memory::Alignment alignment) noexcept
    {
        // Reserve a span aligned to virtual memory's page to avoid interferences with adjacent blocks when committing\decommitting.

        auto virtual_size = VirtualMemory::Ceil(size);
        auto virtual_alignment = VirtualMemory::Ceil(alignment);

        if (auto available_span = Memory::Align(unallocated_span_, virtual_size, virtual_alignment))
        {
            auto [block, unallocated_span] = Memory::SliceFront(available_span, virtual_size);

            unallocated_span_ = unallocated_span;
            uncommitted_span_ = Memory::PopFront(uncommitted_span_, virtual_size);

            return Front(block, ToInt(size));
        }

        return {};
    }

}