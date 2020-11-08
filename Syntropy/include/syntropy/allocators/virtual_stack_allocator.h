
/// \file virtual_stack_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains stack allocators growing in a contiguous virtual memory space.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/memory.h"
#include "syntropy/virtual_memory/virtual_memory.h"
#include "syntropy/virtual_memory/virtual_buffer.h"

#include "syntropy/math/math.h"

#include "syntropy/diagnostics/assert.h"

namespace Syntropy
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
        VirtualStackAllocator(Memory::Bytes capacity, Memory::Bytes granularity) noexcept;

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
        Memory::RWByteSpan Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept;

        /// \brief Reserve a new memory block.
        /// Reserved blocks must be committed via Memory::Commit before accessing them.
        /// If a memory block could not be reserved, returns an empty block.
        Memory::RWByteSpan Reserve(Memory::Bytes size, Memory::Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment) noexcept;

        /// \brief Deallocate every allocation performed on this allocator so far, invalidating all outstanding checkpoints.
        void DeallocateAll() noexcept;

        /// \brief Check whether this allocator owns a memory block.
        Bool Owns(const Memory::ByteSpan& block) const noexcept;

        /// \brief Swap this allocator with another one.
        void Swap(VirtualStackAllocator& rhs) noexcept;

        /// \brief Get the current state of the allocator.
        TCheckpoint Checkpoint() const noexcept;

        /// \brief Restore the allocator to a previous state.
        /// If the provided checkpoint wasn't obtained by means of ::Checkpoint(), the behavior of this method is undefined.
        /// This method invalidates all checkpoints obtained after the provided checkpoint. Rewinding to an invalid checkpoint results in undefined behavior.
        void Rewind(const TCheckpoint& checkpoint) noexcept;

    private:

        /// \brief Virtual memory reserved for this allocator.
        VirtualBuffer virtual_span_;

        /// \brief Memory that hasn't been allocated yet.
        Memory::RWByteSpan unallocated_span_;

        /// \brief Memory that hasn't been committed yet.
        Memory::RWByteSpan uncommitted_span_;

        /// \brief Commit granularity. This is always a multiple of the virtual memory's page size.
        Memory::Bytes commit_granularity_;

    };

    /************************************************************************/
    /* VIRTUAL STACK ALLOCATOR :: CHECKPOINT                                */
    /************************************************************************/

    /// \brief Represents a checkpoint used to rewind a virtual stack allocator back to a previous state.
    class VirtualStackAllocator::TCheckpoint
    {
        friend class VirtualStackAllocator;

        /// \brief Memory that hadn't been allocated yet when the checkpoint was initialized.
        Memory::RWByteSpan unallocated_span_;

        /// \brief Memory that hadn't been committed yet when the checkpoint was initialized.
        Memory::RWByteSpan uncommitted_span_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualStackAllocator.
    // ======================

    inline VirtualStackAllocator::VirtualStackAllocator(Memory::Bytes capacity, Memory::Bytes granularity) noexcept
        : virtual_span_(capacity)
        , unallocated_span_(virtual_span_.GetData())
        , uncommitted_span_(unallocated_span_)
        , commit_granularity_(VirtualMemory::Ceil(granularity))
    {

    }

    inline VirtualStackAllocator::VirtualStackAllocator(VirtualStackAllocator&& rhs) noexcept
        : virtual_span_(Move(rhs.virtual_span_))
        , unallocated_span_(rhs.unallocated_span_)
        , uncommitted_span_(rhs.uncommitted_span_)
        , commit_granularity_(rhs.commit_granularity_)
    {

    }

    inline VirtualStackAllocator& VirtualStackAllocator::operator=(VirtualStackAllocator rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    inline void VirtualStackAllocator::Deallocate(const Memory::RWByteSpan& block, Memory::Alignment /*alignment*/) noexcept
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(Owns(block), "The provided block doesn't belong to this allocator instance.");
    }

    inline void VirtualStackAllocator::DeallocateAll() noexcept
    {
        auto committed_span = DifferenceFront(virtual_span_.GetData(), uncommitted_span_);

        VirtualMemory::Decommit(committed_span);                // Kernel call.

        unallocated_span_ = virtual_span_.GetData();
        uncommitted_span_ = unallocated_span_;
    }

    inline Bool VirtualStackAllocator::Owns(const Memory::ByteSpan& block) const noexcept
    {
        auto allocated_span = DifferenceFront(virtual_span_.GetData(), unallocated_span_);

        return Contains(allocated_span, block);
    }

    inline void VirtualStackAllocator::Swap(VirtualStackAllocator& rhs) noexcept
    {
        using Algorithm::Swap;

        Swap(virtual_span_, rhs.virtual_span_);
        Swap(unallocated_span_, rhs.unallocated_span_);
        Swap(uncommitted_span_, rhs.uncommitted_span_);
    }

    inline VirtualStackAllocator::TCheckpoint VirtualStackAllocator::Checkpoint() const noexcept
    {
        auto checkpoint = TCheckpoint{};

        checkpoint.unallocated_span_ = unallocated_span_;
        checkpoint.uncommitted_span_ = uncommitted_span_;

        return checkpoint;
    }

    inline void VirtualStackAllocator::Rewind(const TCheckpoint& checkpoint) noexcept
    {
        VirtualMemory::Decommit(DifferenceFront(checkpoint.uncommitted_span_, uncommitted_span_));          // Kernel call.

        unallocated_span_ = checkpoint.unallocated_span_;
        uncommitted_span_ = checkpoint.uncommitted_span_;
    }

}
