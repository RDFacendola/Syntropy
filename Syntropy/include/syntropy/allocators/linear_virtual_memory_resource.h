
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
#include "syntropy/memory/virtual_memory_buffer.h"

#include "syntropy/math/math.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* LINEAR VIRTUAL MEMORY RESOURCE                                       */
    /************************************************************************/

    /// \brief Tier 0 memory resource that grows in a contiguous virtual memory space.
    /// Memory is allocated sequentially. Pointer-level deallocation is not supported.
    /// \author Raffaele D. Facendola - May 2020
    class LinearVirtualMemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        /// \param capacity Maximum capacity for the allocator.
        /// \param granularity Granularity size when committing new virtual memory pages.
        LinearVirtualMemoryResource(Bytes capacity, Bytes granularity) noexcept;

        /// \brief No copy constructor.
        LinearVirtualMemoryResource(const LinearVirtualMemoryResource&) = delete;

        /// \brief Move constructor.
        LinearVirtualMemoryResource(LinearVirtualMemoryResource&& rhs) noexcept;

        /// \brief Destructor.
        ~LinearVirtualMemoryResource() = default;

        /// \brief Unified assignment operator.
        LinearVirtualMemoryResource& operator=(LinearVirtualMemoryResource rhs) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a span representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// Pointer-level deallocations are not supported, therefore this method does nothing.
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Deallocate every allocation performed so far.
        void DeallocateAll() noexcept;

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Swap this memory resource with the provided instance.
        void Swap(LinearVirtualMemoryResource& rhs) noexcept;

        /// \brief Get the current state of the allocator.
        RWByteSpan SaveState() const;

        /// \brief Restore the allocator to a previous state.
        /// If the provided state wasn't obtained by means of ::SaveState(), the behavior of this method is undefined.
        /// RestoreState invalidates all states obtained after the state being provided. Restoring an invalid state results in undefined behavior.
        void RestoreState(RWByteSpan state);

    private:

        /// \brief Virtual memory range reserved for this resource.
        VirtualMemoryBuffer virtual_memory_;

        /// \brief Span of unallocated memory.
        RWByteSpan free_;

        /// \brief Commit granularity, reduces the number of kernel calls when committing new pages.
        Alignment granularity_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two SequentialMemoryResource.
    void swap(LinearVirtualMemoryResource& lhs, LinearVirtualMemoryResource& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LinearVirtualMemoryResource.

    inline LinearVirtualMemoryResource::LinearVirtualMemoryResource(Bytes capacity, Bytes granularity) noexcept
        : virtual_memory_(capacity)
        , free_(virtual_memory_.GetData())
        , granularity_(ToAlignment(Math::Ceil(granularity, VirtualMemory::GetPageSize())))
    {
        auto commit_head = free_.GetData();
        auto commit_tail = Memory::Align(free_, ToAlignment(granularity)).GetData();

        VirtualMemory::Commit({ commit_head, commit_tail });
    }

    inline LinearVirtualMemoryResource::LinearVirtualMemoryResource(LinearVirtualMemoryResource&& rhs) noexcept
        : virtual_memory_(std::move(rhs.virtual_memory_))
        , free_(rhs.free_)
        , granularity_(rhs.granularity_)
    {

    }

    inline LinearVirtualMemoryResource& LinearVirtualMemoryResource::operator=(LinearVirtualMemoryResource rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    inline void LinearVirtualMemoryResource::Deallocate(const RWByteSpan& block, Alignment /*alignment*/) noexcept
    {
        SYNTROPY_ASSERT(Owns(block));
    }

    inline void LinearVirtualMemoryResource::DeallocateAll() noexcept
    {
        auto decommit_head = virtual_memory_.GetData().GetData();
        auto decommit_tail = Memory::Align(free_, granularity_).GetData();

        VirtualMemory::Decommit({ decommit_head, decommit_tail });

        free_ = virtual_memory_.GetData();
    }

    inline Bool LinearVirtualMemoryResource::Owns(const ByteSpan& block) const noexcept
    {
        return Contains(virtual_memory_.GetData(), block);
    }

    inline void LinearVirtualMemoryResource::Swap(LinearVirtualMemoryResource& rhs) noexcept
    {
        using std::swap;

        swap(virtual_memory_, rhs.virtual_memory_);
        swap(free_, rhs.free_);
        swap(granularity_, rhs.granularity_);
    }

    inline RWByteSpan LinearVirtualMemoryResource::SaveState() const
    {
        return free_;
    }

    inline void LinearVirtualMemoryResource::RestoreState(RWByteSpan state)
    {
        state = Memory::Align(state, granularity_);

        VirtualMemory::Decommit(state);

        free_ = state;
    }

    // Non-member functions.

    inline void swap(LinearVirtualMemoryResource& lhs, LinearVirtualMemoryResource& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}
