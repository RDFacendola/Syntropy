
/// \file virtual_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for allocators allocating fixed-size blocks on system virtual memory.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/allocators/virtual_stack_allocator.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/virtual_memory/virtual_memory.h"
#include "syntropy/virtual_memory/virtual_buffer.h"
#include "syntropy/math/math.h"
#include "syntropy/diagnostics/assert.h"

namespace Syntropy
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

        /// \brief Deallocate every allocation performed on this allocator so far.
        void DeallocateAll() noexcept;

        /// \brief Check whether this allocator owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(VirtualAllocator& rhs) noexcept;

    private:

        /// \brief A chunk in the index used to keep unmapped free pages.
        struct FreePageIndex;

        /// \brief Reserve a chunk and return its range.
        RWByteSpan Reserve() noexcept;

        /// \brief Size of each allocation. This value is a multiple of the system virtual memory page size.
        Bytes page_size_;

        /// \brief Head of the free list.
        RWPointer<FreePageIndex> free_page_index_{ nullptr };

        /// \brief Underlying virtual allocator.
        VirtualStackAllocator allocator_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualAllocator.
    // =================

    inline VirtualAllocator::VirtualAllocator(Bytes capacity, Bytes page_size) noexcept
        : page_size_(VirtualMemory::Ceil(page_size))
        , allocator_(capacity, page_size)
    {

    }

    inline VirtualAllocator::VirtualAllocator(VirtualAllocator&& rhs) noexcept
        : page_size_(rhs.page_size_)
        , free_page_index_(rhs.free_page_index_)
        , allocator_(Move(rhs.allocator_))
    {
        rhs.free_page_index_ = nullptr;
    }

    inline VirtualAllocator& VirtualAllocator::operator=(VirtualAllocator rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    inline void VirtualAllocator::DeallocateAll() noexcept
    {
        allocator_.DeallocateAll();

        free_page_index_ = nullptr;
    }

    inline Bool VirtualAllocator::Owns(const ByteSpan& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    inline void VirtualAllocator::Swap(VirtualAllocator& rhs) noexcept
    {
        using Algorithm::Swap;

        Swap(page_size_, rhs.page_size_);
        Swap(free_page_index_, rhs.free_page_index_);
        Swap(allocator_, rhs.allocator_);
    }
}

