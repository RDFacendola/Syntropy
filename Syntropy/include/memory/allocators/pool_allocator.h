
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

#include "diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* POOL ALLOCATOR                                                       */
    /************************************************************************/

    /// \brief Allocator used to allocate fixed-size memory blocks.
    /// Allocated blocks are guaranteed to be aligned to their own size.
    /// \tparam TAllocator Type of the underlying allocator.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TAllocator>
    class PoolAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param Arguments used to construct the underlying allocator.
        /// \param max_size Maximum size for each allocation. Allocated blocks are guaranteed to be aligned to this value.
        template <typename... TArguments>
        PoolAllocator(Bytes max_size, TArguments&&... arguments) noexcept;

        /// \brief No copy constructor.
        PoolAllocator(const PoolAllocator&) = delete;

        /// \brief Move constructor.
        PoolAllocator(PoolAllocator&& rhs) noexcept;

        /// \brief Default destructor.
        ~PoolAllocator() = default;

        /// \brief Unified assignment operator.
        PoolAllocator& operator=(PoolAllocator rhs) noexcept;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns a range representing the requested memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size).
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this allocator owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this allocator, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(PoolAllocator& rhs) noexcept;

    private:

        /// \brief Represents a free block in the pool.
        struct FreeBlock
        {
            FreeBlock* next_{ nullptr };    ///< \brief Next free block.
        };

        TAllocator allocator_;              ///< \brief Allocator used to manage the pooled memory. Deallocated blocks are sent to the free list and never deallocated by this allocator.

        Bytes max_size_;                    ///< \brief Maximum size for each allocated block.

        FreeBlock* free_{ nullptr };        ///< \brief Next free block in the pool. nullptr if no previous block was freed.
    };

}

/// \brief Swaps two syntropy::PoolAllocator<> instances.
template <typename TAllocator>
void swap(syntropy::PoolAllocator<TAllocator>& lhs, syntropy::PoolAllocator<TAllocator>& rhs) noexcept;

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TAllocator>
    template <typename... TArguments>
    inline PoolAllocator<TAllocator>::PoolAllocator(Bytes max_size, TArguments&&... arguments) noexcept
        : allocator_(std::forward<TArguments>(arguments)...)
        , max_size_(max_size)
    {

    }

    template <typename TAllocator>
    inline PoolAllocator<TAllocator>::PoolAllocator(PoolAllocator&& rhs) noexcept
        : allocator_(std::move(rhs.allocator_))
        , max_size_(std::move(rhs.max_size_))
        , free_(std::move(rhs.free_))
    {

    }

    template <typename TAllocator>
    inline PoolAllocator<TAllocator>& PoolAllocator<TAllocator>::operator=(PoolAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    template <typename TAllocator>
    inline MemoryRange PoolAllocator<TAllocator>::Allocate(Bytes size) noexcept
    {
        if (size <= max_size_)
        {
            if (free_)
            {
                auto block = MemoryAddress(free_);

                free_ = free_->next_;

                return { block, block + max_size_ };                                // Recycle an existing free-block.
            }

            return allocator_.Allocate(max_size_, Alignment(size));                 // Allocate from the underlying allocator.
        }

        return {};
    }

    template <typename TAllocator>
    inline MemoryRange PoolAllocator<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (Bytes(alignment) <= max_size_)                                          // Power of 2 alignment guarantees that if a smaller alignment is required, the request should be fulfilled without any further alignment.
        {
            auto block = Allocate(size);                                            // Default allocations are already aligned.

            SYNTROPY_POSTCONDITION(block.Begin().IsAlignedTo(alignment));           // Right?

            return block;
        }

        return {};
    }

    template <typename TAllocator>
    inline void PoolAllocator<TAllocator>::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(allocator_.Owns(block));                                        // Check whether the block was allocated by this allocator.

        auto free_block = block.Begin().As<FreeBlock>();

        free_block->next_ = free_;                                                      // Chain the free block to the free list.
        free_ = free_block;
    }

    template <typename TAllocator>
    inline void PoolAllocator<TAllocator>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(Bytes(alignment) <= max_size_);

        Deallocate(block);
    }

    template <typename TAllocator>
    inline bool PoolAllocator<TAllocator>::Owns(const MemoryRange& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator>
    inline void PoolAllocator<TAllocator>::Swap(PoolAllocator& rhs) noexcept
    {
        using std::swap;

        swap(allocator_, rhs.allocator_);
        swap(max_size_, rhs.max_size_);
        swap(free_, rhs.free_);
    }

}

template <typename TAllocator>
void swap(syntropy::PoolAllocator<TAllocator>& lhs, syntropy::PoolAllocator<TAllocator>& rhs) noexcept
{
    lhs.Swap(rhs);
}
