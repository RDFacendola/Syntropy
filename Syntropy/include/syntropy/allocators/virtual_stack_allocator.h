
/// \file virtual_stack_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains stack allocators growing in a contiguous virtual memory space.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/memory.h"
#include "syntropy/virtual_memory/virtual_memory.h"
#include "syntropy/virtual_memory/virtual_buffer.h"

#include "syntropy/math/math.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL STACK ALLOCATOR                                              */
    /************************************************************************/

    /// \brief Tier 0 allocator that grows in a contiguous virtual memory space.
    /// Memory is allocated sequentially. Pointer-level deallocation is not supported.
    /// \author Raffaele D. Facendola - May 2020
    class VirtualStackAllocator
    {
    public:

        /// \brief A checkpoint used to restore the allocator status.
        class TCheckpoint;

        /// \brief Create a new allocator.
        /// \param capacity Virtual memory capacity to reserve. This amount is not committed initially, therefore it can be much higher than system physical memory size.
        /// \param granularity Granularity size when committing new virtual memory pages.
        VirtualStackAllocator(Bytes capacity, Bytes granularity) noexcept;

        /// \brief No copy constructor.
        VirtualStackAllocator(const VirtualStackAllocator&) = delete;

        /// \brief Move constructor.
        VirtualStackAllocator(VirtualStackAllocator&& rhs) noexcept;

        /// \brief Destructor.
        ~VirtualStackAllocator() = default;

        /// \brief Unified assignment operator.
        VirtualStackAllocator& operator=(VirtualStackAllocator rhs) noexcept;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Reserve a new memory block.
        /// Reserved blocks must be committed via Memory::Commit before accessing them.
        /// If a memory block could not be reserved, returns an empty block.
        RWByteSpan Reserve(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Deallocate every allocation performed on this allocator so far, invalidating all outstanding checkpoints.
        void DeallocateAll() noexcept;

        /// \brief Check whether this allocator owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Swap this allocator with another one.
        void Swap(VirtualStackAllocator& rhs) noexcept;

        /// \brief Get the current state of the allocator.
        TCheckpoint Checkpoint() const noexcept;

        /// \brief Restore the allocator to a previous state.
        /// If the provided checkpoint wasn't obtained by means of ::Checkpoint(), the behavior of this method is undefined.
        /// This method invalidates all checkpoints obtained after the provided checkpoint. Rewinding to an invalid checkpoint results in undefined behavior.
        void Rewind(const TCheckpoint& checkpoint) noexcept;

    private:

        /// \brief Virtual memory range reserved for this allocator.
        VirtualBuffer virtual_span_;

        /// \brief Free memory range.
        RWByteSpan free_span_;

        /// \brief Committed memory range. This always represents an integer number of virtual memory pages.
        RWByteSpan committed_span_;

        /// \brief Commit granularity. This is always a multiple of the virtual memory's page size.
        Bytes granularity_;

    };

    /************************************************************************/
    /* VIRTUAL STACK ALLOCATOR :: CHECKPOINT                                */
    /************************************************************************/

    /// \brief Represents a checkpoint used to rewind a virtual stack allocator back to a previous state.
    class VirtualStackAllocator::TCheckpoint
    {
        friend class VirtualStackAllocator;

        /// \brief Free memory range when the checkpoint was initially created.
        RWByteSpan free_span_;

        /// \brief Committed memory range when the checkpoint was initially created.
        RWByteSpan committed_span_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualStackAllocator.
    // ======================

    inline VirtualStackAllocator::VirtualStackAllocator(Bytes capacity, Bytes granularity) noexcept
        : virtual_span_(capacity)
        , free_span_(virtual_span_.GetData())
        , committed_span_()
        , granularity_(VirtualMemory::Ceil(granularity))
    {

    }

    inline VirtualStackAllocator::VirtualStackAllocator(VirtualStackAllocator&& rhs) noexcept
        : virtual_span_(std::move(rhs.virtual_span_))
        , free_span_(rhs.free_span_)
        , committed_span_()
        , granularity_(rhs.granularity_)
    {

    }

    inline VirtualStackAllocator& VirtualStackAllocator::operator=(VirtualStackAllocator rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    inline void VirtualStackAllocator::Deallocate(const RWByteSpan& block, Alignment /*alignment*/) noexcept
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(Owns(block), "The provided block doesn't belong to this allocator instance.");
    }

    inline void VirtualStackAllocator::DeallocateAll() noexcept
    {
        VirtualMemory::Decommit(committed_span_);

        free_span_ = virtual_span_.GetData();
        committed_span_ = {};
    }

    inline Bool VirtualStackAllocator::Owns(const ByteSpan& block) const noexcept
    {
        auto allocated_span = PopBack(virtual_span_.GetData(), ToInt(Memory::Size(free_span_)));

        return Contains(allocated_span, block);
    }

    inline void VirtualStackAllocator::Swap(VirtualStackAllocator& rhs) noexcept
    {
        using std::swap;

        swap(virtual_span_, rhs.virtual_span_);
        swap(free_span_, rhs.free_span_);
        swap(granularity_, rhs.granularity_);
    }

    inline VirtualStackAllocator::TCheckpoint VirtualStackAllocator::Checkpoint() const noexcept
    {
        auto checkpoint = TCheckpoint{};

        checkpoint.free_span_ = free_span_;
        checkpoint.committed_span_ = committed_span_;

        return checkpoint;
    }

    inline void VirtualStackAllocator::Rewind(const TCheckpoint& checkpoint) noexcept
    {
        auto decommit_span = PopFront(committed_span_, ToInt(Memory::Size(checkpoint.committed_span_)));

        VirtualMemory::Decommit(decommit_span);                 // Kernel call.

        free_span_ = checkpoint.free_span_;
        committed_span_ = checkpoint.committed_span_;
    }

}
