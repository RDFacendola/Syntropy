
/// \file pool_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains fixed-size allocators.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <algorithm>

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

#include "memory/allocators/linear_allocator.h"

#include "diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* POOL ALLOCATOR                                                       */
    /************************************************************************/

    /// \brief Allocator used to allocate fixed-size memory blocks over a contiguous range of memory addresses.
    /// Allocated blocks are guaranteed to be aligned to their own size.
    /// \author Raffaele D. Facendola - August 2018
    class PoolAllocator
    {
    public:

        /// \brief Default constructor.
        PoolAllocator() = default;

        /// \brief Create a new allocator.
        /// \param memory_range Memory range the allocator will operate on.
        /// \param allocation_size Size of each allocation.
        PoolAllocator(const MemoryRange& memory_range, Bytes allocation_size);

        /// \brief No copy constructor.
        PoolAllocator(const PoolAllocator&) = delete;

        /// \brief Move constructor.
        PoolAllocator(PoolAllocator&& rhs) noexcept;

        /// \brief Default destructor.
        ~PoolAllocator() = default;

        /// \brief Unified assignment operator.
        PoolAllocator& operator=(PoolAllocator rhs) noexcept;

        /// \brief Allocate a new memory block.
        /// \return Returns a pointer to the allocated memory block. If no allocation could be performed returns nullptr.
        MemoryAddress Allocate();

        /// \brief Free a memory block.
        /// \param block Memory block to free.
        void Free(MemoryAddress block) noexcept;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const noexcept;

        /// \brief Get the size of each allocated memory block.
        /// \return Returns the size of each allocated memory block.
        Bytes GetAllocationSize() const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(PoolAllocator& rhs) noexcept;

    private:

        /// \brief Represents a free block in the pool.
        struct FreeBlock
        {
            FreeBlock* next_{ nullptr };    ///< \brief Next free block.
        };

        MemoryRange memory_range_;          ///< \brief Memory range manager by this allocator.

        Bytes allocation_size_;             ///< \brief Size of each allocated block.

        FreeBlock* free_{ nullptr };        ///< \brief Next free block in the pool, nullptr if the pool is full.
    };

}

/// \brief Swaps two syntropy::PoolAllocator instances.
void swap(syntropy::PoolAllocator& lhs, syntropy::PoolAllocator& rhs) noexcept;

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/

namespace syntropy
{

    inline PoolAllocator::PoolAllocator(const MemoryRange& memory_range, Bytes allocation_size)
        : memory_range_(memory_range)
        , allocation_size_(allocation_size_)
    {
        // Initialize the free list.
        
        for (auto free = memory_range.End().GetAlignedDown(Alignment(allocation_size)) - allocation_size;
             free > memory_range.Begin();
             free -= allocation_size)
        {
            free.As<FreeBlock>()->next_ = free_;
            free_ = free.As<FreeBlock>();
        }
    }

    inline PoolAllocator::PoolAllocator(PoolAllocator&& rhs) noexcept
        : memory_range_(std::move(rhs.memory_range_))
        , allocation_size_(std::move(rhs.allocation_size_))
        , free_(std::move(rhs.free_))
    {

    }

    inline PoolAllocator& PoolAllocator::operator=(PoolAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    inline MemoryAddress PoolAllocator::Allocate()
    {
        MemoryAddress block = free_;

        if (free_)
        {
            free_ = free_->next_;
        }

        return block;
    }

    inline void PoolAllocator::Free(MemoryAddress block) noexcept
    {
        SYNTROPY_ASSERT(memory_range_.Contains(block));             // Check whether the block was allocated by this allocator.

        auto free_block = block.As<FreeBlock>();

        free_block->next_ = free_;
        free_ = free_block;
    }

    inline const MemoryRange& PoolAllocator::GetRange() const noexcept
    {
        return memory_range_;
    }

    inline Bytes PoolAllocator::GetAllocationSize() const noexcept
    {
        return allocation_size_;
    }

    inline void PoolAllocator::Swap(PoolAllocator& rhs) noexcept
    {
        using std::swap;

        swap(memory_range_, rhs.memory_range_);
        swap(allocation_size_, rhs.allocation_size_);
        swap(free_, rhs.free_);
    }

}

void swap(syntropy::PoolAllocator& lhs, syntropy::PoolAllocator& rhs) noexcept
{
    lhs.Swap(rhs);
}
