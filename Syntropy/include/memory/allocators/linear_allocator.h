
/// \file frame_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains sequential and linear allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <algorithm>

#include "memory/memory.h"
#include "memory/virtual_memory.h"

namespace syntropy
{

    /// \brief Allocator used to allocate memory blocks over a contiguous range of virtual memory addresses. Blocks are allocated sequentially.
    /// Memory is committed and decommited on demand. Memory is always allocated on the head. Pointer-level deallocations are not supported.
    /// \author Raffaele D. Facendola - January 2017
    class LinearAllocator
    {
    public:

        /// \brief Default constructor.
        LinearAllocator();

        /// \brief Create a new allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param alignment Memory alignment.
        LinearAllocator(size_t capacity, size_t alignment);

        /// \brief Create a new allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param alignment Memory alignment.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        LinearAllocator(const MemoryRange& memory_range, size_t alignment);

        /// \brief No copy constructor.
        LinearAllocator(const LinearAllocator&) = delete;

        /// \brief Move constructor.
        LinearAllocator(LinearAllocator&& other);

        /// \brief Default destructor.
        ~LinearAllocator() = default;

        /// \brief No assignment operator.
        LinearAllocator& operator=(const LinearAllocator&) = delete;

        /// \brief Allocate a new memory block on the allocator's head.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \param alignment Alignment of the block.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size, size_t alignment = 1);

        /// \brief Reserve a new memory block on the allocator's head.
        /// \param size Size of the memory block to reserve, in bytes.
        /// \param alignment Alignment of the block.
        /// \return Returns a pointer to the reserved memory block.
        void* Reserve(size_t size, size_t alignment = 1);

        /// \brief Free all the allocations performed so far.
        void Free();

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        MemoryPool memory_pool_;        ///< \brief Virtual memory range owned by this allocator. Empty if the allocator owns no virtual memory.

        MemoryRange memory_range_;      ///< \brief Memory range managed by the allocator. May refer to memory_pool_ or to a range owned by someone else.

        void* head_;                    ///< \brief Points to the first unallocated memory address.

        void* page_head_;               ///< \brief Points to the first unmapped memory page.

    };

}