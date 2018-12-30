
/// \file pool_allocator_policy.h
/// \brief This header is part of the syntropy memory management system. It contains different pool allocator policies.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* DEFAULT POOL ALLOCATOR POLICY                                        */
    /************************************************************************/

    /// \brief Represents a syntropy::PoolAllocator policy that is used to recycle allocated memory blocks intrusively.
    /// This policy uses the freed block itself intrusively in order to store a linked-list of free blocks ready to be recycled again.
    /// \author Raffaele D. Facendola - August 2018
    struct DefaultPoolAllocatorPolicy
    {

        /// \brief Attempts to recycle a previously deallocated memory block.
        /// \param size Size of the block to recycle.
        /// \return Returns a memory range representing a free block. If no such block exists returns an empty range.
        MemoryRange Recycle(Bytes size) noexcept;

        /// \brief Deallocate a memory block making it free for recycling.
        /// \param block Block to free.
        /// \param max_size Maximum size for a block in the allocator.
        void Trash(const MemoryRange& block, Bytes max_size);

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

    /// \brief Represents a syntropy::PoolAllocator policy that is used to recycle allocated memory blocks non-intrusively.
    /// This policy uses some of the deallocated memory blocks to store the linked-list of other free blocks without accessing them directly.
    /// The policy is non-intrusive and should be used when storing data inside a free block is not an option (for example when virtual memory gets decommitted).
    /// \author Raffaele D. Facendola - August 2018
    struct NonIntrusivePoolAllocatorPolicy
    {

        /// \brief Attempts to recycle a previously deallocated memory block.
        /// \param size Size of the block to recycle.
        /// \return Returns a memory range representing a free block. If no such block exists returns an empty range.
        MemoryRange Recycle(Bytes size) noexcept;

        /// \brief Deallocate a memory block making it free for recycling.
        /// \param block Block to free.
        /// \param max_size Maximum size for a block in the allocator.
        void Trash(const MemoryRange& block, Bytes max_size);

    private:

        /// \brief Contains non-intrusive data about a free block such as its memory address and the next free block.
        struct FreeList
        {
            FreeList();

            FreeList* next_{ nullptr };                 ///< \brief Next free list.

            MemoryAddress* free_block_{ nullptr };      ///< \brief Next free block.

            MemoryAddress first_block_{ nullptr };      ///< \brief First free block.
        };

        FreeList* free_{ nullptr };                     ///< \brief Current free list.
    };

}

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline MemoryRange DefaultPoolAllocatorPolicy::Recycle(Bytes size) noexcept
    {
        if (free_)
        {
            auto block = MemoryAddress(free_);

            free_ = free_->next_;

            return { block, block + size };
        }

        return {};
    }

    inline void DefaultPoolAllocatorPolicy::Trash(const MemoryRange& block, Bytes /*max_size*/)
    {
        auto free = free_;

        free_ = block.Begin().As<FreeBlock>();

        new (free_) FreeBlock();

        free_->next_ = free;
    }

    inline NonIntrusivePoolAllocatorPolicy::FreeList::FreeList()
        : free_block_(&first_block_)
    {

    }

    MemoryRange NonIntrusivePoolAllocatorPolicy::Recycle(Bytes size) noexcept
    {
        if (free_)
        {
            if (free_->free_block_ != &(free_->first_block_))                                   // Take a free block in the current free list.
            {
                --free_->free_block_;

                auto block = *free_->free_block_;

                return { block, block + size };
            }
            else                                                                                // A free-list exists but is empty: recycle the list itself and move to the next free list.
            {
                auto block = MemoryAddress(free_);

                free_ = free_->next_;

                return { block, block + size };
            }
        }

        return {};                                                                              // No block to recycle.
    }

    void NonIntrusivePoolAllocatorPolicy::Trash(const MemoryRange& block, Bytes max_size)
    {
        auto next_free_block = free_->free_block_ + 1;

        if (free_ && MemoryAddress(next_free_block) <= block.Begin() + max_size)                // The current free list can fit more free blocks.
        {
            *(free_->free_block_) = block.Begin();

            free_->free_block_ = next_free_block;
        }
        else                                                                                    // The current free list doesn't exist or is full: create a new one recycling the provided memory block.
        {
            auto free = free_;

            free_ = block.Begin().As<FreeList>();

            new (free_) FreeList();

            free_->next_ = free;                                                                // Link to the previous free list.
        }
    }

}

