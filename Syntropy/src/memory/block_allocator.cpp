#include "memory/allocators/block_allocator.h"

#include <algorithm>

#include "platform/system.h"
#include "diagnostics/assert.h"

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

    void* BlockAllocator::Allocate(Bytes commit_size)
    {
        SYNTROPY_ASSERT(commit_size <= block_size_);

        commit_size = Math::Ceil(commit_size, VirtualMemory::GetPageSize());

        auto block = Reserve();

        VirtualMemory::Commit(MemoryRange(block, commit_size));

        return block;
    }

    void* BlockAllocator::Reserve()
    {
        if (!free_list_)
        {
            // No free block

            return allocator_.Reserve(block_size_, Alignment(block_size_));     // Reserve a new block aligned to its own size.
        }
        else if(free_list_->IsEmpty())
        {
            // No free block referenced by the current chunk

            auto block = free_list_;                                            // Reuse the chunk itself.
            free_list_ = free_list_->next_;                                     // Move to the next free list block. The block is already committed.

            return block;
        }
        else
        {
            // Restore a deallocated memory block.

            return free_list_->PopBlock();                                      // Pop a free block address.
        }
    }

    void BlockAllocator::Free(void* block)
    {
        if (!free_list_ || free_list_->IsFull())
        {
            // Recycle the block itself as a new free list chunk.

            auto capacity = (block_size_ - Bytes(sizeof(FreeBlock))) / Bytes(sizeof(uintptr_t)) + 1;

            VirtualMemory::Commit(MemoryRange(block, block_size_));                     // Make sure the block is mapped to the system memory.

            free_list_ = new (block) FreeBlock(free_list_, capacity);
        }
        else
        {
            // Add the block to the current free list chunk.

            free_list_->PushBlock(block);                                               // Push the address of the free block.

            VirtualMemory::Decommit(MemoryRange(block, block_size_));                   // Unmap the block from the system memory.
        }

    }

}