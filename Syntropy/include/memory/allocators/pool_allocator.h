
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

#include "memory/allocators/pool_allocator_policy.h"

#include "diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* POOL ALLOCATOR                                                       */
    /************************************************************************/

    /// \brief Allocator used to allocate fixed-size memory blocks. Deallocated blocks are kept around and recycled when possible.
    /// Allocated blocks are guaranteed to be aligned to their own size.
    /// \tparam TAllocator Type of the underlying allocator.
    /// \tparam TPolicy Policy to be used when freeing and recycling previous allocations.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TAllocator, typename TPolicy = DefaultPoolAllocatorPolicy>
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

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(PoolAllocator& rhs) noexcept;

    private:

        TAllocator allocator_;              ///< \brief Allocator used to manage the pooled memory. Deallocated blocks are sent to the free list and never deallocated by this allocator.

        Bytes max_size_;                    ///< \brief Maximum size for each allocated block.

        TPolicy policy_;                    ///< \brief Policy functor used to allocate and recycle memory blocks.
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

    template <typename TAllocator, typename TPolicy>
    template <typename... TArguments>
    inline PoolAllocator<TAllocator, TPolicy>::PoolAllocator(Bytes max_size, TArguments&&... arguments) noexcept
        : allocator_(std::forward<TArguments>(arguments)...)
        , max_size_(max_size)
    {

    }

    template <typename TAllocator, typename TPolicy>
    inline PoolAllocator<TAllocator, TPolicy>::PoolAllocator(PoolAllocator&& rhs) noexcept
        : allocator_(std::move(rhs.allocator_))
        , max_size_(std::move(rhs.max_size_))
        , policy_(std::move(rhs.policy_))
    {

    }

    template <typename TAllocator, typename TPolicy>
    inline PoolAllocator<TAllocator, TPolicy>& PoolAllocator<TAllocator, TPolicy>::operator=(PoolAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    template <typename TAllocator, typename TPolicy>
    inline MemoryRange PoolAllocator<TAllocator, TPolicy>::Allocate(Bytes size) noexcept
    {
        if (size <= max_size_)
        {
            if (auto block = policy_.Recycle(size, max_size_))                      // Attempts to recycle a previously deallocated block.
            {
                return block;
            }
            else
            {
                auto block = allocator_.Allocate(max_size_, Alignment(size));       // Allocate from the underlying allocator.

                return { block.Begin(), block.Begin() + size };                     // Returns a correctly-sized block.
            }
        }

        return {};
    }

    template <typename TAllocator, typename TPolicy>
    inline MemoryRange PoolAllocator<TAllocator, TPolicy>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (Bytes(alignment) <= max_size_)              // Allocations are aligned at block boundaries (and any smaller power-of-two alignments).
        {
            return Allocate(size);
        }

        return {};
    }

    template <typename TAllocator, typename TPolicy>
    inline void PoolAllocator<TAllocator, TPolicy>::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(allocator_.Owns(block));

        policy_.Trash(block);                                                       // Trash the block and make it available for future recycle.
    }

    template <typename TAllocator, typename TPolicy>
    inline void PoolAllocator<TAllocator, TPolicy>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(Bytes(alignment) <= max_size_);

        Deallocate(block);
    }

    template <typename TAllocator, typename TPolicy>
    inline bool PoolAllocator<TAllocator, TPolicy>::Owns(const MemoryRange& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator, typename TPolicy>
    inline Bytes PoolAllocator<TAllocator, TPolicy>::GetMaxAllocationSize() const noexcept
    {
        return max_size_;
    }

    template <typename TAllocator, typename TPolicy>
    inline void PoolAllocator<TAllocator, TPolicy>::Swap(PoolAllocator& rhs) noexcept
    {
        using std::swap;

        swap(allocator_, rhs.allocator_);
        swap(max_size_, rhs.max_size_);
        swap(policy_, rhs.policy_);
    }

}

template <typename TAllocator, typename TPolicy>
void swap(syntropy::PoolAllocator<TAllocator, TPolicy>& lhs, syntropy::PoolAllocator<TAllocator, typename TPolicy>& rhs) noexcept
{
    lhs.Swap(rhs);
}
