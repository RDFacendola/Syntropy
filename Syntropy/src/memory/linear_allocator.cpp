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

    void* LinearAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = head_;

        head_ = Memory::AddOffset(head_, size);

        SYNTROPY_ASSERT(Memory::GetDistance(memory_range_.GetTop(), head_) <= 0);   // Out-of-memory check.

        // Commit each memory page between the old page head and the new one
        auto next_page = Memory::Align(head_, Memory::GetPageSize());               // Ceil to next memory page boundary.

        auto allocation_size = Memory::GetDistance(page_head_, next_page);

        if (allocation_size > 0)
        {
            Memory::Commit(page_head_, static_cast<size_t>(allocation_size));       // Commit the missing memory pages.

            page_head_ = next_page;
        }

        MemoryDebug::MarkUninitialized(block, head_);

        return block;
    }

    void LinearAllocator::Free(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        head_ = Memory::SubOffset(head_, size);

        SYNTROPY_ASSERT(Memory::GetDistance(*memory_range_, head_) >= 0);           // Can't free more memory than it was allocated.

        // Decommit each memory page between the old page head and the new one
        auto next_page = Memory::Align(head_, Memory::GetPageSize());               // Ceil to previous memory page boundary.

        auto free_size = Memory::GetDistance(next_page, page_head_);

        if(free_size > 0)
        {
            page_head_ = next_page;

            Memory::Decommit(page_head_, static_cast<size_t>(free_size));           // Free the extra amount of memory.
        }

        MemoryDebug::MarkFree(head_, page_head_);                                   // Marks up to the first unallocated memory page. Accessing other addresses will cause the application to crash anyway.

    }

    size_t LinearAllocator::GetAllocationSize() const
    {
        return static_cast<size_t>(Memory::GetDistance(*memory_range_, head_));
    }

    size_t LinearAllocator::GetCommitSize() const
    {
        return static_cast<size_t>(Memory::GetDistance(*memory_range_, page_head_));
    }

    const MemoryRange& LinearAllocator::GetRange() const
    {
        return memory_range_;
    }

}