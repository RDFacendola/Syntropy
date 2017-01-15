#include "memory/block_allocator.h"

#include "platform/system.h"
#include "diagnostics/diagnostics.h"

namespace syntropy
{

    //////////////// BLOCK ALLOCATOR ////////////////

    BlockAllocator::BlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))                   // Round up to the next system page size.
        , memory_(Memory::Reserve(capacity, block_size_), capacity)         // Reserve the virtual memory range upfront without allocating.
        , head_(*memory_)
        , free_list_(capacity / block_size_)                                // Worst case: the entire capacity was allocated and then freed. A better estimate would be min(physical memory size, capacity).
    {

    }

    BlockAllocator::~BlockAllocator()
    {
        Memory::Release(*memory_);
    }

    void* BlockAllocator::Allocate()
    {
        return Allocate(block_size_);
    }

    void* BlockAllocator::Allocate(size_t size)
    {
        auto block = Reserve(size);

        memory_.Allocate(block, size);                              // Allocate the requested memory size. Rounded up to the next memory page boundary.

        return block;
    }

    void* BlockAllocator::Reserve(size_t size)
    {
        SYNTROPY_ASSERT(size <= block_size_);

        void* block;

        if (free_list_.IsEmpty())                                   // No free block?
        {
            block = head_;                                          // Reserve a new memory block on the allocator's head.
            head_ = Memory::Offset(head_, block_size_);             // Move the head forward.
        }
        else
        {
            block = reinterpret_cast<void*>(free_list_.Back());     // Pop a free block address.
            free_list_.PopBack();
        }

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, block_size_));   // Check block alignment.

        return block;
    }

    void BlockAllocator::Free(void* block)
    {
        block = Memory::AlignDown(block, block_size_);              // Get the base address of the block to free.

        free_list_.PushBack(reinterpret_cast<uintptr_t>(block));    // Push the address of the free block.

        memory_.Free(block, block_size_);                           // Unmap the block from the system memory.
    }

    size_t BlockAllocator::GetBlockSize() const
    {
        return block_size_;
    }

    size_t BlockAllocator::GetSize() const
    {
        // TODO: This is actually the maximum size, the committed memory may be lower.
        return Memory::GetSize(*memory_, head_) -               // Maximum amount of memory for the allocations performed so far
               free_list_.GetCount() * block_size_;             // Amount of freed memory
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
        : block_size_(Memory::CeilToPageSize(block_size))                                   // Round up to the next system allocation granularity.
        , memory_(Memory::Reserve(capacity, block_size_), capacity)                         // Reserve the virtual memory range upfront without allocating.
        , head_(*memory_)
        , free_(nullptr)
    {

    }

    MonotonicBlockAllocator::~MonotonicBlockAllocator()
    {
        Memory::Release(*memory_);
    }

    void* MonotonicBlockAllocator::Allocate()
    {
        auto block = free_;

        if (block)
        {
            free_ = block->next_;                                   // Move to the next free block.
        } 
        else
        {
            block = reinterpret_cast<Block*>(head_);                // Allocate a new memory block on the allocator's head
            memory_.Allocate(block, block_size_);                   // Allocate the new memory block
            head_ = Memory::Offset(head_, block_size_);             // Move the head forward
        }

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, block_size_));   // Check block alignment.

        return block;
    }

    void MonotonicBlockAllocator::Free(void* block)
    {
        block = Memory::AlignDown(block, block_size_);              // Get the base address of the block to free.

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
        return Memory::GetSize(*memory_, head_);
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