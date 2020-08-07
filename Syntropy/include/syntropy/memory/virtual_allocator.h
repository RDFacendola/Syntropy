
/// \file virtual_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains definitions for allocators allocating fixed-size blocks on system virtual memory.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_buffer.h"
#include "syntropy/math/math.h"
#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL ALLOCATOR                                                    */
    /************************************************************************/

    /// \brief Tier 0 allocator used to allocate fixed-sized blocks on system virtual memory.
    /// Allocation sizes are rounded up and aligned to virtual page boundaries.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Virtual memory capacity to reserve. This amount is not committed initially, therefore it can be much higher than system physical memory size.
        /// \param page_size Size of each allocation.
        VirtualAllocator(Bytes capacity, Bytes page_size) noexcept;

        /// \brief No copy constructor.
        VirtualAllocator(const VirtualAllocator&) = delete;

        /// \brief Move constructor.
        VirtualAllocator(VirtualAllocator&& rhs) noexcept;

        /// \brief Default destructor.
        ~VirtualAllocator() = default;

        /// \brief Unified assignment operator.
        VirtualAllocator& operator=(VirtualAllocator rhs) noexcept;

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

        /// \brief Check whether this allocator owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(VirtualAllocator& rhs) noexcept;

    private:

        /// \brief Type of a linked list used to track free pages.
        struct FreeList;

        /// \brief Reserve a block and return its range.
        RWByteSpan Reserve() noexcept;

        /// \brief Virtual memory range reserved for this resource.
        VirtualBuffer virtual_storage_;

        /// \brief Range of memory yet to allocate.
        RWByteSpan virtual_unallocated_;

        /// \brief Size of each allocation. This value is a multiple of the system virtual memory page size.
        Bytes page_size_;

        ///< \brief Maximum alignment for each allocated page.
        Alignment page_alignment_;

        /// \brief Head of the free list.
        Pointer<FreeList> free_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two VirtualMemoryResources.
    void swap(VirtualAllocator& lhs, VirtualAllocator& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualAllocator.
    // =================

    inline VirtualAllocator::VirtualAllocator(Bytes capacity, Bytes page_size) noexcept
        : virtual_storage_(capacity)
        , virtual_unallocated_(virtual_storage_.GetData())
        , page_size_(Math::Ceil(page_size, Memory::GetPageSize()))
        , page_alignment_(ToAlignment(Memory::GetPageSize()))
    {

    }

    inline VirtualAllocator::VirtualAllocator(VirtualAllocator&& rhs) noexcept
        : virtual_storage_(std::move(rhs.virtual_storage_))
        , virtual_unallocated_(rhs.virtual_unallocated_)
        , page_size_(rhs.page_size_)
        , page_alignment_(rhs.page_alignment_)
        , free_(rhs.free_)
    {

    }

    inline VirtualAllocator& VirtualAllocator::operator=(VirtualAllocator rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    inline Bool VirtualAllocator::Owns(const ByteSpan& block) const noexcept
    {
        return Contains(virtual_storage_.GetData(), block);
    }

    inline void swap(VirtualAllocator& lhs, VirtualAllocator& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}

