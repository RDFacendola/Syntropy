
/// \file linear_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains sequential and linear allocators.
///
/// \author Raffaele D. Facendola - 2017

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
    /* LINEAR ALLOCATOR                                                     */
    /************************************************************************/

    /// \brief Allocator used to allocate memory over a contiguous range of memory addresses.
    /// Memory is allocated sequentially on demand. Pointer-level deallocations are not supported, unless they refer to the last allocation performed.
    /// The allocator can be rewound to a previous state, undoing all the allocations that were performed from that point on.
    /// \author Raffaele D. Facendola - January 2017, August 2018
    class LinearAllocator
    {
    public:

        /// \brief Default constructor.
        LinearAllocator() = default;

        /// \brief Create a new allocator.
        /// \param memory_range Memory range the allocator will operate on.
        LinearAllocator(const MemoryRange& memory_range);

        /// \brief No copy constructor.
        LinearAllocator(const LinearAllocator&) = delete;

        /// \brief Move constructor.
        LinearAllocator(LinearAllocator&& rhs) noexcept;

        /// \brief Default destructor.
        ~LinearAllocator() = default;

        /// \brief Unified assignment operator.
        LinearAllocator& operator=(LinearAllocator rhs) noexcept;

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
        void Deallocate(const MemoryRange& block) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment) noexcept;

        /// \brief Deallocate every allocation performed so far on this allocator.
        void DeallocateAll() noexcept;

        /// \brief Restore the allocator to a previous state.
        /// \param state State to restore the allocator to. Must match any value returned by SaveState() otherwise the behaviour is undefined.
        void RestoreState(MemoryAddress state);

        /// \brief Get the current state of the allocator.
        /// The returned value can be used to restore the allocator to a previous state via the method RestoreState(state);
        /// \return Returns the current state of the allocator.
        MemoryAddress SaveState() const noexcept;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(LinearAllocator& rhs) noexcept;

    private:

        MemoryRange memory_range_;          ///< \brief Memory range managed by this allocator.

        MemoryAddress head_;                ///< \brief Pointer past the last allocated address.
    };

}

/// \brief Swaps two syntropy::LinearAllocator instances.
void swap(syntropy::LinearAllocator& lhs, syntropy::LinearAllocator& rhs) noexcept;

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/
 
namespace syntropy
{
    inline LinearAllocator::LinearAllocator(const MemoryRange& memory_range)
        : memory_range_(memory_range)
        , head_(memory_range_.Begin())
    {

    }

    inline LinearAllocator::LinearAllocator(LinearAllocator&& rhs) noexcept
        : memory_range_(rhs.memory_range_)
        , head_(rhs.head_)
    {
        memory_range_ = MemoryRange();
        head_ = memory_range_.Begin();
    }

    inline LinearAllocator& LinearAllocator::operator=(LinearAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    inline MemoryRange LinearAllocator::Allocate(Bytes size) noexcept
    {
        return Allocate(size, Alignment());
    }

    inline MemoryRange LinearAllocator::Allocate(Bytes size, Alignment alignment) noexcept
    {
        auto block = head_.GetAligned(alignment);

        auto head = block + size;

        if (head < memory_range_.End())
        {
            head_ = head;

            return { block, head_ };
        }

        return {};
    }

    inline void LinearAllocator::Deallocate(const MemoryRange& block) noexcept
    {
        // Only the last block can be deallocated.

        if (block.End() == head_)
        {
            head_ = block.Begin();
        }
    }

    inline void LinearAllocator::Deallocate(const MemoryRange& block, Alignment /*alignment*/) noexcept
    {
        Deallocate(block);
    }

    inline void LinearAllocator::DeallocateAll() noexcept
    {
        // Unwind the head pointer.

        head_ = memory_range_.Begin();
    }

    inline void LinearAllocator::RestoreState(MemoryAddress head)
    {
        SYNTROPY_ASSERT(head >= memory_range_.Begin() && head <= memory_range_.End());

        head_ = head;
    }

    inline MemoryAddress LinearAllocator::SaveState() const noexcept
    {
        return head_;
    }

    inline const MemoryRange& LinearAllocator::GetRange() const noexcept
    {
        return memory_range_;
    }

    inline void LinearAllocator::Swap(LinearAllocator& rhs) noexcept
    {
        using std::swap;

        swap(memory_range_, rhs.memory_range_);
        swap(head_, rhs.head_);
    }

}

inline void swap(syntropy::LinearAllocator& lhs, syntropy::LinearAllocator& rhs) noexcept
{
    lhs.Swap(rhs);
}
