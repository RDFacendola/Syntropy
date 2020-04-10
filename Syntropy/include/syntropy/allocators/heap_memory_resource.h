
/// \file new_delete_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains an allocator that wraps system heap allocation calls.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <new>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* HEAP MEMORY RESOURCE                                                 */
    /************************************************************************/

    /// \brief Tier 0 memory resource used to allocate memory on system heap via new\delete calls.
    /// \author Raffaele D. Facendola - February 2017
    class HeapMemoryResource
    {
    public:

        /// \brief Default constructor.
        HeapMemoryResource() noexcept = default;

        /// \brief Default copy constructor.
        HeapMemoryResource(const HeapMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        HeapMemoryResource(HeapMemoryResource&&) noexcept = default;

        /// \brief Default destructor.
        ~HeapMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        HeapMemoryResource& operator=(const HeapMemoryResource&) noexcept = default;

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
        void Deallocate(const MemoryRange& block) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment) noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // HeapMemoryResource.

    inline MemoryRange HeapMemoryResource::Allocate(Bytes size) noexcept
    {
        if (auto block = MemoryAddress{ ::operator new(std::size_t{size}, std::nothrow) })
        {
            return { block, block + size };
        }

        return {};
    }

    inline MemoryRange HeapMemoryResource::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = MemoryAddress{ ::operator new(std::size_t{size}, alignment, std::nothrow) })
        {
            return { block, block + size };
        }

        return {};
    }

    inline void HeapMemoryResource::Deallocate(const MemoryRange& block) noexcept
    {
        ::operator delete(block.Begin(), std::nothrow);
    }

    inline void HeapMemoryResource::Deallocate(const MemoryRange& block, Alignment alignment) noexcept
    {
        ::operator delete(block.Begin(), alignment, std::nothrow);
    }

}


