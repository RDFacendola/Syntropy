
/// \file pool_allocator_policy.h
/// \brief This header is part of the syntropy memory management system. It contains different pool allocator policies.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

#include "memory/virtual_memory.h"
#include "memory/virtual_memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* DEFAULT POOL ALLOCATOR POLICY                                        */
    /************************************************************************/

    /// \brief Represents a syntropy::PoolAllocator policy that is used to recycle allocated memory blocks.
    /// This policy uses the freed block itself intrusively in order to store a linked-list of free blocks ready to be recycled.
    /// The allocation of a free block is O(1).
    /// \author Raffaele D. Facendola - August 2018
    struct DefaultPoolAllocatorPolicy
    {

        /// \brief Attempts to recycle a previously deallocated memory block.
        /// \param size Size of the block to recycle.
        /// \return Returns a memory range representing a free block. If no such block exists returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Deallocate a memory block making it free for recycling.
        /// \param block Block to free .
        void Deallocate(const MemoryRange& block);

    private:

        /// \brief Represents a free block: the memory block itself is used to store a pointer to the next free block in the list.
        struct FreeBlock
        {
            FreeBlock* next_{ nullptr };        ///< \brief Next free block in the pool.
        };

        FreeBlock* free_{ nullptr };            ///< \brief Next free block in the pool. nullptr if no previous block was freed.
    };

    /************************************************************************/
    /* NON-INTRUSIVE POOL ALLOCATOR POLICY                                  */
    /************************************************************************/

    /// \brief Represents a syntropy::PoolAllocator policy that is used to recycle allocated memory blocks.
    /// This policy uses extra space to store the linked-list of free blocks but doesn't access any of them directly.
    /// The policy is non-intrusive and should be used when storing data inside a free block is not an option (for example when virtual memory gets decommitted).
    /// The allocation of a free block is O(1).
    /// \author Raffaele D. Facendola - August 2018
    struct NonIntrusivePoolAllocatorPolicy
    {
    private:

        /// \brief Contains non-intrusive data about a free block such as its memory address and the next free block.
        struct FreeBlock
        {
            MemoryAddress block_;               ///< \brief First address in the free block.

            FreeBlock* next_{ nullptr };        ///< \brief Next free block in the pool.
        };

    };

}

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    MemoryRange DefaultPoolAllocatorPolicy::Allocate(Bytes size) noexcept
    {
        if (free_)
        {
            auto block = MemoryAddress(free_);

            free_ = free_->next_;

            return { block, block + size };
        }

        return {};
    }

    void DefaultPoolAllocatorPolicy::Deallocate(const MemoryRange& block)
    {
        auto free_block = block.Begin().As<FreeBlock>();

        free_block->next_ = free_;                          // Chain the free block to the free list.

        free_ = free_block;
    }

}

