#include "memory/block_allocator.h"

#include "platform/system.h"
#include "diagnostics/diagnostics.h"

namespace syntropy
{

    //////////////// BLOCK ALLOCATOR ////////////////

    BlockAllocator::BlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToAllocationGranularity(block_size))                          // Round up to the next system allocation granularity.
        , memory_(GetMemory().Reserve(Math::Ceil(capacity, block_size_), block_size_))          // Reserve the virtual memory range upfront without allocating.
        , head_(*memory_)
    {
        // TODO: Move to bookkeeping!!!
        // TODO: This allocation here stinks! It can waste a ton of space when using multiple block allocators.
        auto total_memory = platform::GetSystem().GetMemoryInfo().total_physical_memory_;       // Maximum amount of memory that can be allocated at any given time
        auto maximum_blocks = total_memory / block_size_;                                       // Maximum amount of blocks that can be allocated using this allocator, also upper bound for the free list
        auto bookkeeping_size = maximum_blocks * sizeof(size_t*);                               // Size of the internal stack

        free_base_ = reinterpret_cast<uintptr_t*>(GetMemory().Allocate(bookkeeping_size));
        free_head_ = free_base_;
    }

    BlockAllocator::~BlockAllocator()
    {
        memory_.Free();         // Free everything
    }

    void* BlockAllocator::Allocate()
    {
        return Allocate(block_size_);
    }

    void* BlockAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size <= block_size_);

        void* block;

        // TODO: Split to Reserve\Allocate (for streaming)
        if (free_base_ == free_head_)                           // No free block?
        {
            block = head_;                                      // Allocate a new memory block on the allocator's head.
            head_ = Memory::Offset(head_, block_size_);         // Move the head forward.
        }
        else
        {
            --free_head_;                                       // Pop a free block address.
            block = reinterpret_cast<void*>(*free_head_);       // Address of the free block.
        }

        memory_.Allocate(block, size);                          // Allocate the requested memory size. Rounded up to the next memory page boundary.
        return block;
    }

    void BlockAllocator::Free(void* block)
    {
        block = Memory::AlignDown(block, block_size_);          // Get the base address of the block to free.

        *free_head_ = reinterpret_cast<uintptr_t>(block);       // Save the address of the free block.
        ++free_head_;                                           // Move the head forward.
        memory_.Free(block, block_size_);                       // Unmap the block from the system memory.
    }

    size_t BlockAllocator::GetBlockSize() const
    {
        return block_size_;
    }

    size_t BlockAllocator::GetSize() const
    {
        // TODO: This is actually the maximum size, the committed memory may be lower.
        return Memory::GetRangeSize(*memory_, head_) -          // Maximum amount of memory for the allocations performed so far
               (free_head_ - free_base_) * block_size_;         // Amount of freed memory
    }

    size_t BlockAllocator::GetCapacity() const
    {
        return memory_.GetCapacity();
    }

    bool BlockAllocator::ContainsAddress(void* address) const
    {
        return Memory::IsContained(*memory_, head_, address);
    }

    //////////////// MONOTONIC BLOCK ALLOCATOR ////////////////

    MonotonicBlockAllocator::MonotonicBlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToAllocationGranularity(block_size))                          // Round up to the next system allocation granularity.
        , memory_(GetMemory().Reserve(Math::Ceil(capacity, block_size_), block_size_))          // Reserve the virtual memory range upfront without allocating.
        , head_(*memory_)
        , free_(nullptr)
    {

    }

    MonotonicBlockAllocator::~MonotonicBlockAllocator()
    {
        memory_.Free();
    }

    void* MonotonicBlockAllocator::Allocate()
    {
        auto block = free_;

        if (block)
        {
            free_ = block->next_;                               // Move to the next free block.
        } 
        else
        {
            block = reinterpret_cast<Block*>(head_);            // Allocate a new memory block on the allocator's head
            memory_.Allocate(block, block_size_);               // Allocate the new memory block
            head_ = Memory::Offset(head_, block_size_);         // Move the head forward
        }

        return block;
    }

    void MonotonicBlockAllocator::Free(void* block)
    {
        block = Memory::AlignDown(block, block_size_);          // Get the base address of the block to free.

        SYNTROPY_ASSERT(ContainsAddress(block));

        auto free = reinterpret_cast<Block*>(block);

        free->next_ = free_;
        free_ = free;
    }

    size_t MonotonicBlockAllocator::GetBlockSize() const
    {
        return block_size_;
    }

    size_t MonotonicBlockAllocator::GetSize() const
    {
        return Memory::GetRangeSize(*memory_, head_);
    }

    size_t MonotonicBlockAllocator::GetCapacity() const
    {
        return memory_.GetCapacity();
    }

    bool MonotonicBlockAllocator::ContainsAddress(void* address) const
    {
        return Memory::IsContained(*memory_, head_, address);
    }

}