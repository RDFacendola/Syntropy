
/// \file linear_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains sequential memory resources.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <algorithm>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* LINEAR MEMORY RESOURCE                                               */
    /************************************************************************/

    /// \brief Memory resource used to allocate memory over a contiguous range of memory addresses.
    /// Memory is allocated sequentially on demand. Pointer-level deallocations are not supported.
    /// The memory resource can be rewound to a previous state, undoing all the allocations that were performed from that point on.
    /// \author Raffaele D. Facendola - January 2017, August 2018
    class LinearMemoryResource
    {
    public:

        /// \brief Default constructor.
        LinearMemoryResource() noexcept = default;

        /// \brief Create a new memory resource.
        /// \param memory_range Memory range the memory resource will operate on.
        LinearMemoryResource(const MemoryRange& memory_range) noexcept;

        /// \brief No copy constructor.
        LinearMemoryResource(const LinearMemoryResource&) = delete;

        /// \brief Move constructor.
        LinearMemoryResource(LinearMemoryResource&& rhs) noexcept;

        /// \brief Default destructor.
        ~LinearMemoryResource() = default;

        /// \brief Unified assignment operator.
        LinearMemoryResource& operator=(LinearMemoryResource rhs) noexcept;

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
        /// Pointer-level deallocations are not supported, therefore this method does nothing.
        void Deallocate(const MemoryRange& block) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// Pointer-level deallocations are not supported, therefore this method does nothing.
        void Deallocate(const MemoryRange& block, Alignment alignment) noexcept;

        /// \brief Deallocate every allocation performed so far.
        void DeallocateAll() noexcept;

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this memory resource.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this memory resource.
        Bytes GetMaxAllocationSize() const noexcept;

        /// \brief Restore the memory resource to a previous state.
        /// \param state State to restore the memory resource to. Must match any value returned by SaveState() otherwise the behaviour is undefined.
        void RestoreState(MemoryAddress state);

        /// \brief Get the current state of the memory resource.
        /// The returned value can be used to restore the memory resource to a previous state via the method RestoreState(state);
        /// \return Returns the current state of the memory resource.
        MemoryAddress SaveState() const noexcept;

        /// \brief Swap this memory resource with the provided instance.
        void Swap(LinearMemoryResource& rhs) noexcept;

    private:

        /// \brief Memory range manager by this memory resource.
        MemoryRange memory_range_;

        /// \brief Pointer past the last allocated address.
        MemoryAddress head_;

    };

    /// \brief Swaps two SequentialMemoryResource.
    void swap(syntropy::LinearMemoryResource& lhs, syntropy::LinearMemoryResource& rhs) noexcept;
 
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LinearMemoryResource.

    inline LinearMemoryResource::LinearMemoryResource(const MemoryRange& memory_range) noexcept
        : memory_range_(memory_range)
        , head_(memory_range_.Begin())
    {

    }

    inline LinearMemoryResource::LinearMemoryResource(LinearMemoryResource&& rhs) noexcept
        : memory_range_(rhs.memory_range_)
        , head_(rhs.head_)
    {

    }

    inline LinearMemoryResource& LinearMemoryResource::operator=(LinearMemoryResource rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    inline MemoryRange LinearMemoryResource::Allocate(Bytes size) noexcept
    {
        return Allocate(size, Alignment());
    }

    inline MemoryRange LinearMemoryResource::Allocate(Bytes size, Alignment alignment) noexcept
    {
        auto block = head_.GetAligned(alignment);

        auto head = block + size;

        if (head <= memory_range_.End())
        {
            head_ = head;

            return { block, head_ };
        }

        return {};      // Out of memory.
    }

    inline void LinearMemoryResource::Deallocate(const MemoryRange& block) noexcept
    {
        SYNTROPY_ASSERT(memory_range_.Contains(block));
    }

    inline void LinearMemoryResource::Deallocate(const MemoryRange& block, Alignment /*alignment*/) noexcept
    {
        SYNTROPY_ASSERT(memory_range_.Contains(block));
    }

    inline void LinearMemoryResource::DeallocateAll() noexcept
    {
        head_ = memory_range_.Begin();      // Unwind the head pointer.
    }

    inline bool LinearMemoryResource::Owns(const MemoryRange& block) const noexcept
    {
        return block.Begin() >= memory_range_.Begin() && block.End() <= head_;
    }

    inline Bytes LinearMemoryResource::GetMaxAllocationSize() const noexcept
    {
        return Bytes(memory_range_.End() - head_);
    }

    inline void LinearMemoryResource::RestoreState(MemoryAddress head)
    {
        SYNTROPY_ASSERT(head >= memory_range_.Begin() && head <= memory_range_.End());

        head_ = head;
    }

    inline MemoryAddress LinearMemoryResource::SaveState() const noexcept
    {
        return head_;
    }

    inline void LinearMemoryResource::Swap(LinearMemoryResource& rhs) noexcept
    {
        using std::swap;

        swap(memory_range_, rhs.memory_range_);
        swap(head_, rhs.head_);
    }

    inline void swap(syntropy::LinearMemoryResource& lhs, syntropy::LinearMemoryResource& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}
