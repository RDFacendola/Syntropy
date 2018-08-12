
/// \file frame_allocator.h
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
    /// Memory is allocated sequentially on demand. Pointer-level deallocations are not supported.
    /// The allocator can be rewound to a previous state, undoing all the allocations that were performed from that point on.
    /// \author Raffaele D. Facendola - January 2017
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
        LinearAllocator(LinearAllocator&& other) noexcept;

        /// \brief Default destructor.
        ~LinearAllocator() = default;

        /// \brief Unified assignment operator.
        LinearAllocator& operator=(LinearAllocator rhs) noexcept;

        /// \brief Allocate a memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns a pointer to the allocated memory block.
        MemoryRange Allocate(Bytes size);

        /// \brief Allocate an aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Alignment of the block.
        /// \return Returns a pointer to the allocated memory block.
        MemoryRange Allocate(Bytes size, Alignment alignment);

        /// \brief Free all the allocations performed so far.
        void Free() noexcept;

        /// \brief Restore the allocator to a previous state.
        /// \param head Head status to restore. Must match any value returned by GetHead() otherwise the behaviour is undefined.
        void Restore(MemoryAddress head);

        /// \brief Get the pointer to the head of the allocator.
        /// \return Returns the pointer to the head of the allocator.
        MemoryAddress GetHead() const noexcept;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(LinearAllocator& rhs) noexcept;

    private:

        MemoryRange memory_range_;          ///< \brief Memory range manager by this allocator.

        MemoryAddress head_;                ///< \brief Pointer past the last allocated address.
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline LinearAllocator::LinearAllocator(const MemoryRange& memory_range)
        : memory_range_(memory_range)
        , head_(memory_range_.Begin())
    {

    }

    inline LinearAllocator::LinearAllocator(LinearAllocator&& other) noexcept
        : memory_range_(other.memory_range_)
        , head_(other.head_)
    {
        memory_range_ = MemoryRange();
        head_ = memory_range_.Begin();
    }

    inline LinearAllocator& LinearAllocator::operator=(LinearAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    inline MemoryRange LinearAllocator::Allocate(Bytes size)
    {
        auto block = head_;

        head_ = block + size;

        SYNTROPY_ASSERT(head_ < memory_range_.End());           // Out-of-memory check.

        return { block, head_ };
    }

    inline MemoryRange LinearAllocator::Allocate(Bytes size, Alignment alignment)
    {
        head_ = head_.GetAligned(alignment);

        return Allocate(size);
    }

    inline void LinearAllocator::Free() noexcept
    {
        head_ = memory_range_.Begin();
    }

    inline void LinearAllocator::Restore(MemoryAddress head)
    {
        SYNTROPY_ASSERT(head >= memory_range_.Begin() && head <= memory_range_.End());

        head_ = head;
    }

    inline MemoryAddress LinearAllocator::GetHead() const noexcept
    {
        return head_;
    }

    inline const MemoryRange& LinearAllocator::GetRange() const noexcept
    {
        return memory_range_;
    }

    inline void LinearAllocator::Swap(LinearAllocator& rhs) noexcept
    {
        std::swap(memory_range_, rhs.memory_range_);
        std::swap(head_, rhs.head_);
    }

}
