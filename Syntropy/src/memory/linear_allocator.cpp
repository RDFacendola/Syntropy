#include "memory/allocators/linear_allocator.h"

#include "diagnostics/assert.h"

namespace syntropy
{

    /************************************************************************/
    /* LINEAR ALLOCATOR                                                     */
    /************************************************************************/

    LinearAllocator::LinearAllocator(Bytes capacity, Alignment alignment)
        : memory_pool_(capacity, alignment)
        , memory_range_(memory_pool_)
        , head_(memory_range_.GetBase())
        , page_head_(memory_range_.GetBase())
    {

    }

    LinearAllocator::LinearAllocator(const MemoryRange& memory_range, Alignment alignment)
        : memory_range_(memory_range.GetBase().GetAligned(alignment), memory_range.GetTop())
        , head_(memory_range_.GetBase())
        , page_head_(memory_range_.GetBase())
    {

    }

    LinearAllocator::LinearAllocator(LinearAllocator&& other)
        : memory_pool_(std::move(other.memory_pool_))
        , memory_range_(other.memory_range_)
        , head_(other.head_)
        , page_head_(other.page_head_)
    {

    }

    void* LinearAllocator::Allocate(Bytes size, Alignment alignment)
    {
        SYNTROPY_ASSERT(size > 0_Bytes);

        auto block = MemoryAddress(head_).GetAligned(alignment);

        head_ = block + size;

        SYNTROPY_ASSERT(head_ <= memory_range_.GetTop());                                                   // Out-of-memory check.

        // Commit each memory page between the old page head and the new one

        auto next_page = head_.GetAligned(Alignment(VirtualMemory::GetPageSize()));                         // Ceil to next memory page boundary.

        auto allocation_size = next_page - page_head_;

        if (allocation_size > 0)
        {
            VirtualMemory::Commit(MemoryRange(page_head_, next_page));                                      // Commit the missing memory pages.

            page_head_ = next_page;                                                                         // Update the page head.
        }

        return block;
    }

    void* LinearAllocator::Reserve(Bytes size, Alignment alignment)
    {
        SYNTROPY_ASSERT(size > 0_Bytes);

        auto block = head_.GetAligned(Alignment(VirtualMemory::GetPageSize()));                             // Reserve at page boundary, we don't want to share memory pages with other allocations.
        
        block = block.GetAligned(alignment);                                                                // Align the block to the requested value.

        head_ = block + size;                                                                               // Reserve the requested size.

        head_ = head_.GetAligned(Alignment(VirtualMemory::GetPageSize()));                                  // Move the head to the next memory page so that following allocations won't share any memory page with this one.

        SYNTROPY_ASSERT(head_ <= memory_range_.GetTop());                                                   // Out-of-memory check.

        page_head_ = head_;                                                                                 // Update the page head.

        return block;
    }

    void LinearAllocator::Free()
    {
        VirtualMemory::Decommit(MemoryRange(memory_range_.GetBase(), page_head_));                          // Decommit everything.

        head_ = memory_range_.GetBase();
        page_head_ = memory_range_.GetBase();
    }

    const MemoryRange& LinearAllocator::GetRange() const
    {
        return memory_range_;
    }

}