
/// \file heap_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains memory resources using system heap memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <new>
#include <limits>

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

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

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

    inline bool HeapMemoryResource::Owns(const MemoryRange& block) const noexcept
    {
        // The heap memory resource is expected to be used as the single application memory resource or as a last resort fallback
        // for other memory resources; assumes the system heap may contain any block.

        return true;
    }

    inline Bytes HeapMemoryResource::GetMaxAllocationSize() const noexcept
    {
        // The heap memory resource is expected to be used as the single application memory resource or as a last resort fallback
        // for other memory resources; assumes the system heap is arbitrary large.

        return Bytes(std::numeric_limits<std::size_t>::infinity());
    }

}


