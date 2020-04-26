
/// \file null_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains memory resources that use no system memory at all.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* NULL MEMORY RESOURCE                                                 */
    /************************************************************************/

    /// \brief Tier 0 memory resource that rejects any form of allocation.
    /// \author Raffaele D. Facendola - August 2018
    class NullMemoryResource
    {
    public:

        /// \brief Default constructor.
        NullMemoryResource() noexcept = default;

        /// \brief Default copy constructor.
        NullMemoryResource(const NullMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        NullMemoryResource(NullMemoryResource&&) noexcept = default;

        /// \brief Default destructor.
        ~NullMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        NullMemoryResource& operator=(const NullMemoryResource&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate. Expects an empty range.
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Expects an empty range.
        /// \param alignment Block alignment.
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this memory resource owns the provided memory block.
        /// The null memory resource only contains empty ranges.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/
 
    // NullMemoryResource.

    inline MemoryRange NullMemoryResource::Allocate(Bytes /*size*/) noexcept
    {
        return {};
    }

    inline MemoryRange NullMemoryResource::Allocate(Bytes /*size*/, Alignment /*alignment*/) noexcept
    {
        return {};
    }

    inline void NullMemoryResource::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(!block);        // Only empty ranges can be deallocated.
    }

    inline void NullMemoryResource::Deallocate(const MemoryRange& block, Alignment /*alignment*/)
    {
        SYNTROPY_ASSERT(!block);        // Only empty ranges can be deallocated.
    }

    inline bool NullMemoryResource::Owns(const MemoryRange& block) const noexcept
    {
        return !block;                  // This memory resource owns only empty ranges.
    }

}

