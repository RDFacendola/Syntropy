
/// \file virtul_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains memory resources using system virtual memory.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_memory_range.h"

#include "syntropy/math/math.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY RESOURCE                                              */
    /************************************************************************/

    /// \brief Tier 0 memory resource used to allocate blocks on system virtual memory.
    /// Allocation sizes are rounded up and aligned to page boundaries.
    /// Memory pages are committed and decommitted automatically.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        /// \param capacity Virtual memory capacity to reserve. This amount is not committed initially, therefore it can be much higher than system physical memory size.
        /// \param page_size Size of each allocation.
        VirtualMemoryResource(Bytes capacity, Bytes page_size) noexcept;

        /// \brief No copy constructor.
        VirtualMemoryResource(const VirtualMemoryResource&) = delete;

        /// \brief Move constructor.
        VirtualMemoryResource(VirtualMemoryResource&& rhs) noexcept;

        /// \brief Default destructor.
        ~VirtualMemoryResource() = default;

        /// \brief Unified assignment operator.
        VirtualMemoryResource& operator=(VirtualMemoryResource rhs) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment = MaxAlignmentOf()) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment = MaxAlignmentOf());

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(VirtualMemoryResource& rhs) noexcept;

    private:

        /// \brief Allocate a block and return its range.
        MemoryRange Allocate();

        /// \brief Represents a list used to track free pages.
        struct FreeList;

        /// \brief Virtual memory range reserved for this resource.
        VirtualMemoryRange virtual_memory_;

        /// \brief Pointer past the last allocated address.
        MemoryAddress head_;

        /// \brief Size of each allocation. This value is a multiple of the system virtual memory page size.
        Bytes page_size_;

        ///< \brief Maximum alignment for each allocated page.
        Alignment page_alignment_;

        /// \brief Current free list.
        FreeList* free_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two VirtualMemoryResources.
    void swap(VirtualMemoryResource& lhs, VirtualMemoryResource& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualMemoryResource.

    inline VirtualMemoryResource::VirtualMemoryResource(Bytes capacity, Bytes page_size) noexcept
        : virtual_memory_(capacity)
        , head_(virtual_memory_.Begin())
        , page_size_(Math::Ceil(page_size, VirtualMemory::GetPageSize()))
        , page_alignment_(VirtualMemory::GetPageSize())
    {

    }

    inline VirtualMemoryResource::VirtualMemoryResource(VirtualMemoryResource&& rhs) noexcept
        : virtual_memory_(std::move(rhs.virtual_memory_))
        , head_(rhs.head_)
        , page_size_(rhs.page_size_)
        , page_alignment_(rhs.page_alignment_)
        , free_(rhs.free_)
    {

    }

    inline VirtualMemoryResource& VirtualMemoryResource::operator=(VirtualMemoryResource rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    inline bool VirtualMemoryResource::Owns(const MemoryRange& block) const noexcept
    {
        return virtual_memory_.Contains(block);
    }

    inline void swap(VirtualMemoryResource& lhs, VirtualMemoryResource& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}

