#include "memory/block_allocator.h"

#include "platform/system.h"
#include "diagnostics/diagnostics.h"
#include "diagnostics/debug.h"

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

    BlockAllocator::BlockAllocator()
        : block_size_(0u)
        , free_list_(nullptr)
    {

    }

    BlockAllocator::BlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system page size.
        , allocator_(capacity, block_size_)                         // Reserve the virtual memory range upfront without allocating.
        , free_list_(nullptr)
    {

    }

    BlockAllocator::BlockAllocator(const MemoryRange& memory_range, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system page size.
        , allocator_(memory_range, block_size_)                     // Get the memory range without taking ownership.
        , free_list_(nullptr)
    {

    }

    BlockAllocator::BlockAllocator(BlockAllocator&& other)
        : block_size_(other.block_size_)
        , allocator_(std::move(other.allocator_))
        , free_list_(other.free_list_)
    {

    }

    void* BlockAllocator::Allocate(size_t commit_size)
    {
        commit_size = Memory::CeilToPageSize(std::min(commit_size, block_size_));

        auto block = Reserve();

        if (commit_size > 0)
        {
            Memory::Commit(block, commit_size);

            MemoryDebug::MarkUninitialized(block, Memory::AddOffset(block, commit_size));
        }

        return block;
    }

    void* BlockAllocator::Reserve()
    {
        if (!free_list_)
        {
            // No free block
            return allocator_.Reserve(block_size_, block_size_);    // Reserve a new block aligned to its own size.
        }
        else if(free_list_->IsEmpty())
        {
            // No free block referenced by the current chunk
            auto block = free_list_;                                // Reuse the chunk itself.
            free_list_ = free_list_->next_;                         // Move to the next free list block. The block is already committed.

            return block;
        }
        else
        {
            // Restore a deallocated memory block.
            return free_list_->PopBlock();                          // Pop a free block address.
        }
    }

    void BlockAllocator::Free(void* block)
    {
        SYNTROPY_ASSERT(allocator_.GetRange().Contains(block));     // Check whether the block belongs to this allocator.
        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, block_size_));   // Check whether the block is properly aligned (guaranteed by Allocate()\Reserve())

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
        return allocator_.GetRange();
    }

    /************************************************************************/
    /* STATIC BLOCK ALLOCATOR                                               */
    /************************************************************************/

    StaticBlockAllocator::StaticBlockAllocator()
        : block_size_(0u)
        , free_list_(nullptr)
    {

    }

    StaticBlockAllocator::StaticBlockAllocator(size_t capacity, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system page size.
        , allocator_(capacity, block_size_)                         // Reserve the virtual memory range upfront without allocating.
        , free_list_(nullptr)
    {

    }

    StaticBlockAllocator::StaticBlockAllocator(const MemoryRange& memory_range, size_t block_size)
        : block_size_(Memory::CeilToPageSize(block_size))           // Round up to the next system page size.
        , allocator_(memory_range, block_size_)                     // Get the memory range without taking ownership.
        , free_list_(nullptr)
    {

    }

    void* StaticBlockAllocator::Allocate()
    {
        if (free_list_)
        {
            auto block = free_list_;                                // Recycle the first unused memory block.
            free_list_ = block->next_;                              // Move to the next free block.

            MemoryDebug::MarkUninitialized(block, Memory::AddOffset(block, block_size_));

            return block;
        }
        else
        {
            return allocator_.Allocate(block_size_, block_size_);   // Allocate a new block aligned to its own size.
        }
    }

    void StaticBlockAllocator::Free(void* block)
    {
        SYNTROPY_ASSERT(allocator_.GetRange().Contains(block));     // Check whether the block belongs to this allocator.
        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, block_size_));   // Check whether the block is properly aligned (guaranteed by Allocate())

        MemoryDebug::MarkFree(block, Memory::AddOffset(block, block_size_));

        auto free_block = reinterpret_cast<Block*>(block);

        free_block->next_ = free_list_;
        free_list_ = free_block;
    }

    size_t StaticBlockAllocator::GetBlockSize() const
    {
        return block_size_;
    }

    const MemoryRange& StaticBlockAllocator::GetRange() const
    {
        return allocator_.GetRange();
    }

}