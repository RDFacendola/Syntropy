#include "memory/linear_allocator.h"

namespace syntropy
{

    /************************************************************************/
    /* LINEAR ALLOCATOR                                                     */
    /************************************************************************/

    LinearAllocator::LinearAllocator(size_t capacity, size_t alignment)
        : memory_pool_(capacity, alignment)             // Allocate a new virtual address range.
        , memory_range_(memory_pool_)                   // Get the full range out of the memory pool.
        , head_(*memory_range_)
        , page_head_(*memory_range_)
    {

    }

    LinearAllocator::LinearAllocator(const MemoryRange& memory_range, size_t alignment)
        : memory_range_(memory_range, alignment)        // Copy the memory range without taking ownership.
        , head_(*memory_range_)
        , page_head_(*memory_range_)
    {

    }

    void* LinearAllocator::Allocate(size_t size, size_t alignment)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = Memory::Align(head_, alignment);

        head_ = Memory::AddOffset(head_, size);

        SYNTROPY_ASSERT(Memory::GetDistance(memory_range_.GetTop(), head_) <= 0);   // Out-of-memory check.

        // Commit each memory page between the old page head and the new one

        auto next_page = Memory::Align(head_, Memory::GetPageSize());               // Ceil to next memory page boundary.

        auto allocation_size = Memory::GetDistance(page_head_, next_page);

        if (allocation_size > 0)
        {
            Memory::Commit(page_head_, static_cast<size_t>(allocation_size));       // Commit the missing memory pages.

            page_head_ = next_page;                                                 // Update the page head.
        }

        MemoryDebug::MarkUninitialized(block, head_);

        return block;
    }

    void* LinearAllocator::Reserve(size_t size, size_t alignment)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = Memory::Align(head_, Memory::GetPageSize());                   // Reserve at page boundary, we don't want to share memory pages with other allocations.
        
        block = Memory::Align(block, alignment);                                    // Align the block to the requested value.

        head_ = Memory::AddOffset(block, size);                                     // Reserve the requested size.

        head_ = Memory::Align(head_, Memory::GetPageSize());                        // Move the head to the next memory page so that following allocations won't share any memory page with this one.

        SYNTROPY_ASSERT(Memory::GetDistance(memory_range_.GetTop(), head_) <= 0);   // Out-of-memory check.

        page_head_ = head_;                                                         // Update the page head.

        return block;
    }

    void LinearAllocator::Free()
    {
        Memory::Decommit(*memory_range_, MemoryRange(*memory_range_, page_head_).GetSize());        // Decommit everything.

        head_ = *memory_range_;
        page_head_ = head_;
    }

    const MemoryRange& LinearAllocator::GetRange() const
    {
        return memory_range_;
    }

}