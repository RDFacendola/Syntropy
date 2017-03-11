#include "memory/block_allocator.h"

#include "platform/system.h"
#include "diagnostics/diagnostics.h"

namespace syntropy
{

    //////////////// BLOCK ALLOCATOR ////////////////

    BlockAllocator::BlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system page size.
        , memory_pool_(capacity, block_size_)                       // Reserve the virtual memory range upfront without allocating.
        , memory_range_(memory_pool_)                               // Get the full range out of the memory pool.
        , head_(*memory_range_)
        , free_list_(capacity / block_size_)                        // Worst case: the entire capacity was reserved and then freed.
    {

    }

    BlockAllocator::BlockAllocator(const MemoryRange& memory_range, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system page size.
        , memory_range_(memory_range, block_size_)
        , head_(*memory_range_)
        , free_list_(memory_range_.GetSize() / block_size_)         // Worst case: the entire capacity was reserved and then freed.
    {

    }

    void* BlockAllocator::Allocate()
    {
        return Allocate(block_size_);
    }

    void* BlockAllocator::Allocate(size_t size)
    {
        auto block = Reserve(size);

        Memory::Commit(block, size);        // Allocate the requested memory size. Rounded up to the next memory page boundary.

        MemoryDebug::MarkUninitialized(block, Memory::AddOffset(block, Memory::CeilToPageSize(size)));

        return block;
    }

    void* BlockAllocator::Reserve(size_t size)
    {
        SYNTROPY_ASSERT(size <= block_size_);

        void* block;

        if (free_list_.IsEmpty())                                   // No free block?
        {
            block = head_;                                          // Reserve a new memory block on the allocator's head.
            head_ = Memory::AddOffset(head_, block_size_);          // Move the head forward.
        }
        else
        {
            block = reinterpret_cast<void*>(free_list_.Back());     // Pop a free block address.
            free_list_.PopBack();
        }

        return block;
    }

    void BlockAllocator::Free(void* block)
    {
        block = Memory::AlignDown(block, block_size_);              // Get the base address of the block to free.

        free_list_.PushBack(reinterpret_cast<uintptr_t>(block));    // Push the address of the free block.

        Memory::Decommit(block, block_size_);                       // Unmap the block from the system memory.
    }

    size_t BlockAllocator::GetBlockSize() const
    {
        return block_size_;
    }

    size_t BlockAllocator::GetCapacity() const
    {
        return memory_range_.GetSize();
    }

    bool BlockAllocator::ContainsAddress(void* address) const
    {
        return MemoryRange(*memory_range_, head_).Contains(address);
    }

    //////////////// MONOTONIC BLOCK ALLOCATOR ////////////////

    MonotonicBlockAllocator::MonotonicBlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system allocation granularity.
        , allocator_(capacity, block_size_)                         // Reserve a contiguous virtual memory range.
        , free_(nullptr)
    {

    }

    MonotonicBlockAllocator::MonotonicBlockAllocator(const MemoryRange& memory_range, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system allocation granularity.
        , allocator_(memory_range, block_size_)
        , free_(nullptr)
    {

    }

    void* MonotonicBlockAllocator::Allocate()
    {
        // Recycle an unused memory block.
        if (free_)
        {
            auto block = free_;
            
            free_ = block->next_;                                   // Move to the next free block.

            return block;
        }

        // Allocate a new block.
        return allocator_.Allocate(block_size_);
    }

    void MonotonicBlockAllocator::Free(void* block)
    {
        SYNTROPY_ASSERT(GetRange().Contains(block));

        auto free_block = reinterpret_cast<Block*>(Memory::AlignDown(block, block_size_));      // Get the base address of the block to free.

        free_block->next_ = free_;
        free_ = free_block;
    }

    size_t MonotonicBlockAllocator::GetBlockSize() const
    {
        return block_size_;
    }

    size_t MonotonicBlockAllocator::GetAllocationSize() const
    {
        size_t free_count = 0;
        
        for (auto free_block = free_; free_block; free_block = free_block->next_)
        {
            ++free_count;
        }

        return allocator_.GetRange().GetSize() - free_count * block_size_;
    }

    size_t MonotonicBlockAllocator::GetCommitSize() const
    {
        return allocator_.GetCommitSize();
    }

    const MemoryRange& MonotonicBlockAllocator::GetRange() const
    {
        return allocator_.GetRange();
    }

}