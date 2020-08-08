
/// \file linear_virtual_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains linear virtual-memory resources.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/memory.h"
#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_buffer.h"

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

        /// \brief Swap this memory resource with the provided instance.
        void Swap(VirtualStackAllocator& rhs) noexcept;

        /// \brief Get the current state of the allocator.
        TCheckpoint Checkpoint() const;

        /// \brief Restore the allocator to a previous state.
        /// If the provided checkpoint wasn't obtained by means of ::Checkpoint(), the behavior of this method is undefined.
        /// This method invalidates all checkpoints obtained after the provided checkpoint. Rewinding to an invalid checkpoint results in undefined behavior.
        void Rewind(const TCheckpoint& checkpoint);

    private:

        /// \brief Virtual memory range reserved for this resource.
        VirtualBuffer virtual_storage_;

        /// \brief Span of unallocated memory.
        RWByteSpan virtual_unallocated_;

        /// \brief Commit granularity, reduces the number of kernel calls when committing new pages.
        Alignment granularity_;

    };

    /************************************************************************/
    /* VIRTUAL STACK ALLOCATOR :: CHECKPOINT                                */
    /************************************************************************/

    /// \brief Represents a checkpoint used to rewind a virtual stack allocator back to a previous state.
    class VirtualStackAllocator::TCheckpoint
    {
        friend class VirtualStackAllocator;

        /// \brief Unallocated storage when the checkpoint was initially created.
        RWByteSpan checkpoint_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two SequentialMemoryResource.
    void swap(VirtualStackAllocator& lhs, VirtualStackAllocator& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualStackAllocator.
    // ======================

    inline VirtualStackAllocator::VirtualStackAllocator(Bytes capacity, Bytes granularity) noexcept
        : virtual_storage_(capacity)
        , virtual_unallocated_(virtual_storage_.GetData())
        , granularity_(ToAlignment(Math::Ceil(granularity, Memory::GetPageSize())))
    {

    }

    inline VirtualStackAllocator::VirtualStackAllocator(VirtualStackAllocator&& rhs) noexcept
        : virtual_storage_(std::move(rhs.virtual_storage_))
        , virtual_unallocated_(rhs.virtual_unallocated_)
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
        SYNTROPY_ASSERT(Owns(block));
    }

    inline void VirtualStackAllocator::DeallocateAll() noexcept
    {
        Memory::Decommit(DifferenceBack(virtual_storage_.GetData(), virtual_unallocated_));

        virtual_unallocated_ = virtual_storage_.GetData();
    }

    inline Bool VirtualStackAllocator::Owns(const ByteSpan& block) const noexcept
    {
        return Contains(virtual_storage_.GetData(), block);
    }

    inline void VirtualStackAllocator::Swap(VirtualStackAllocator& rhs) noexcept
    {
        using std::swap;

        swap(virtual_storage_, rhs.virtual_storage_);
        swap(virtual_unallocated_, rhs.virtual_unallocated_);
        swap(granularity_, rhs.granularity_);
    }

    inline VirtualStackAllocator::TCheckpoint VirtualStackAllocator::Checkpoint() const
    {
        auto checkpoint = TCheckpoint{};

        checkpoint.checkpoint_ = virtual_unallocated_;

        return checkpoint;
    }

    inline void VirtualStackAllocator::Rewind(const TCheckpoint& checkpoint)
    {
        auto unallocated = Memory::Align(checkpoint.checkpoint_, granularity_);

        Memory::Decommit(unallocated);

        virtual_unallocated_ = unallocated;
    }

    // Non-member functions.
    // =====================

    inline void swap(VirtualStackAllocator& lhs, VirtualStackAllocator& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}
