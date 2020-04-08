
/// \file virtul_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains virtual memory resources.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <algorithm>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_memory_buffer.h"
#include "syntropy/memory/virtual_memory_page.h"

#include "syntropy/allocators/sequential_memory_resource.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY RESOURCE                                              */
    /************************************************************************/

    /// \brief Memory resource used to allocate blocks using system virtual memory.
    /// Allocation sizes are rounded up and aligned to page boundaries.
    /// Memory pages are committed and decommitted automatically.
    /// \tparam TPolicy Policy to be used to free and recycle previous memory pages.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        /// \param capacity Virtual memory capacity to reserve. This amount is not committed initially, therefore it can be much higher than system physical memory size.
        /// \param max_allocation_size Maximum size of each allocation performed on this memory resource.
        VirtualMemoryResource(Bytes capacity, Bytes max_allocation_size) noexcept;

        /// \brief No copy constructor.
        VirtualMemoryResource(const VirtualMemoryResource&) = delete;

        /// \brief Move constructor.
        VirtualMemoryResource(VirtualMemoryResource&& rhs) noexcept;

        /// \brief Default destructor.
        ~VirtualMemoryResource() = default;

        /// \brief Unified assignment operator.
        VirtualMemoryResource& operator=(VirtualMemoryResource rhs) noexcept;

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

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(VirtualMemoryResource& rhs) noexcept;

    private:

        /// \brief Allocate a block and return its range.
        MemoryRange Allocate();

        /// \brief Represents a list used to track free pages.
        struct FreeList;

        /// \brief Virtual memory buffer reserved by this allocator.
        VirtualMemoryBuffer virtual_memory_;

        /// \brief Underlying memory resource.
        SequentialMemoryResource memory_resource_;

        /// \brief Size of each allocation. This value is a multiple of system's virtual memory pages.
        Bytes allocation_size_;

        ///< \brief Maximum alignment for each allocated block.
        Alignment allocation_alignment_;

        /// \brief Current free list.
        FreeList* free_{ nullptr };

    };

    /// \brief Swaps two VirtualMemoryResources.
    void swap(syntropy::VirtualMemoryResource& lhs, syntropy::VirtualMemoryResource& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualMemoryResource.

    inline VirtualMemoryResource::VirtualMemoryResource(Bytes capacity, Bytes max_allocation_size) noexcept
        : virtual_memory_(capacity)
        , memory_resource_(MemoryRange{ virtual_memory_ })
        , allocation_size_(Ceil(max_allocation_size, VirtualMemory::GetPageSize()))
        , allocation_alignment_(VirtualMemory::GetPageSize())
    {

    }

    inline VirtualMemoryResource::VirtualMemoryResource(VirtualMemoryResource&& rhs) noexcept
        : virtual_memory_(std::move(rhs.virtual_memory_))
        , memory_resource_(std::move(rhs.memory_resource_))
        , allocation_size_(rhs.allocation_size_)
        , allocation_alignment_(rhs.allocation_alignment_)
        , free_(rhs.free_)
    {

    }

    inline VirtualMemoryResource& VirtualMemoryResource::operator=(VirtualMemoryResource rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    inline MemoryRange VirtualMemoryResource::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (alignment <= allocation_alignment_)
        {
            return Allocate(size);
        }

        return {};
    }

    inline void VirtualMemoryResource::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(Owns(block));

        SYNTROPY_ASSERT(alignment <= allocation_alignment_);

        Deallocate(block);
    }

    inline bool VirtualMemoryResource::Owns(const MemoryRange& block) const noexcept
    {
        return virtual_memory_.Contains(block);
    }

    inline Bytes VirtualMemoryResource::GetMaxAllocationSize() const noexcept
    {
        return allocation_size_;
    }

    inline void VirtualMemoryResource::Swap(VirtualMemoryResource& rhs) noexcept
    {
        using std::swap;

        swap(virtual_memory_, rhs.virtual_memory_);
        swap(memory_resource_, rhs.memory_resource_);
        swap(allocation_size_, rhs.allocation_size_);
        swap(allocation_alignment_, rhs.allocation_alignment_);
        swap(free_, rhs.free_);
    }

    inline void swap(syntropy::VirtualMemoryResource& lhs, syntropy::VirtualMemoryResource& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}

