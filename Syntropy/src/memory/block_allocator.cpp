#include "memory/block_allocator.h"

#include "platform/system.h"
#include "diagnostics/diagnostics.h"

namespace syntropy
{

    /************************************************************************/
    /* BLOCK ALLOCATOR :: FREE BLOCK                                        */
    /************************************************************************/

    BlockAllocator::FreeBlock::FreeBlock(FreeBlock* next, size_t capacity)
        : next_(next)
        , count_(0u)
        , capacity_(capacity)
    {

    }

    void* BlockAllocator::FreeBlock::PopBlock()
    {
        SYNTROPY_ASSERT(!IsEmpty());

        return reinterpret_cast<void*>((&base_)[--count_]);         // Reads the count_-th element starting from the base of the block.
    }

    void BlockAllocator::FreeBlock::PushBlock(void* block)
    {
        SYNTROPY_ASSERT(!IsFull());

        (&base_)[count_++] = reinterpret_cast<uintptr_t>(block);    // Writes the count_-th element starting from the base of the block.
    }

    bool BlockAllocator::FreeBlock::IsEmpty() const
    {
        return count_ == 0;
    }

    bool BlockAllocator::FreeBlock::IsFull() const
    {
        return count_ == capacity_;
    }

    /************************************************************************/
    /* BLOCK ALLOCATOR                                                      */
    /************************************************************************/

    BlockAllocator::BlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system page size.
        , memory_pool_(capacity, block_size_)                       // Reserve the virtual memory range upfront without allocating.
        , memory_range_(memory_pool_)                               // Get the full range out of the memory pool.
        , free_list_(nullptr)
        , head_(*memory_range_)
    {

    }

    BlockAllocator::BlockAllocator(const MemoryRange& memory_range, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system page size.
        , memory_range_(memory_range, block_size_)                  // Get the memory range without taking ownership.
        , free_list_(nullptr)
        , head_(*memory_range_)
    {

    }

    void* BlockAllocator::Allocate()
    {
        return Allocate(block_size_);
    }

    void* BlockAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size <= block_size_);

        auto block = Reserve();

        Memory::Commit(block, size);

        MemoryDebug::MarkUninitialized(block, Memory::AddOffset(block, Memory::CeilToPageSize(size)));

        return block;
    }

    void* BlockAllocator::Reserve()
    {
        void* block;

        if (!free_list_)
        {
            // No free block
            block = head_;                                          // Reserve a new memory block on the allocator's head.
            head_ = Memory::AddOffset(head_, block_size_);          // Move the head forward.

            SYNTROPY_ASSERT(Memory::GetDistance(memory_range_.GetTop(), head_) <= 0);       // Check for out-of-memory.

        }
        else if(free_list_->IsEmpty())
        {
            // No free block referenced by the current chunk
            block = free_list_;                                     // Repurpose the chunk itself.
            free_list_ = free_list_->next_;                         // Move to the next free list block. The block is already committed.
        }
        else
        {
            // Restore a deallocated memory block.
            block = free_list_->PopBlock();                         // Pop a free block address.
        }

        return block;
    }

    void BlockAllocator::Free(void* block)
    {
        SYNTROPY_ASSERT(MemoryRange(*memory_range_, head_).Contains(block));    // Check whether the block belongs to this allocator.
        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, block_size_));               // Check whether the block is properly aligned (guaranteed by Allocate()\Reserve())

        if (!free_list_ || free_list_->IsFull())
        {
            // Recycle the block itself as a new free list chunk.

            auto capacity = (block_size_ - sizeof(FreeBlock)) / sizeof(uintptr_t) + 1;

            Memory::Commit(block, block_size_);                     // Make sure the block is mapped to the system memory.

            free_list_ = new (block) FreeBlock(free_list_, capacity);
        }
        else
        {
            // Add the block to the current free list chunk.

            free_list_->PushBlock(block);                           // Push the address of the free block.

            Memory::Decommit(block, block_size_);                   // Unmap the block from the system memory.
        }

    }

    size_t BlockAllocator::GetBlockSize() const
    {
        return block_size_;
    }

    const MemoryRange& BlockAllocator::GetRange() const
    {
        return memory_range_;
    }

    /************************************************************************/
    /* MONOTONIC BLOCK ALLOCATOR                                            */
    /************************************************************************/

    MonotonicBlockAllocator::MonotonicBlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system allocation granularity.
        , allocator_(capacity, block_size_)                         // Reserve a contiguous virtual memory range.
        , free_(nullptr)
        , allocation_size_(0u)
    {

    }

    MonotonicBlockAllocator::MonotonicBlockAllocator(const MemoryRange& memory_range, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system allocation granularity.
        , allocator_(memory_range, block_size_)
        , free_(nullptr)
        , allocation_size_(0u)
    {

    }

    void* MonotonicBlockAllocator::Allocate()
    {
        allocation_size_ += block_size_;

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

        allocation_size_ -= block_size_;

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
        return allocation_size_;
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