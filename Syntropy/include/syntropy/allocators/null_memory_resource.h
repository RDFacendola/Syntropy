
/// \file null_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains memory resources that use no system memory at all.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_span.h"
#include "syntropy/core/types.h"
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

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns an empty range.
        MemorySpan Allocate(Bytes size, Alignment alignment = MaxAlignmentOf()) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Expects an empty range.
        /// \param alignment Block alignment.
        void Deallocate(const MemorySpan& block, Alignment alignment = MaxAlignmentOf());

        /// \brief Check whether this memory resource owns the provided memory block.
        /// The null memory resource only contains empty ranges.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        Bool Owns(const MemorySpan& block) const noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/
 
    // NullMemoryResource.

    inline MemorySpan NullMemoryResource::Allocate(Bytes /*size*/, Alignment /*alignment*/) noexcept
    {
        return {};
    }

    inline void NullMemoryResource::Deallocate(const MemorySpan& block, Alignment /*alignment*/)
    {
        SYNTROPY_ASSERT(!block);        // Only empty ranges can be deallocated.
    }

    inline Bool NullMemoryResource::Owns(const MemorySpan& block) const noexcept
    {
        return !block;                  // This memory resource owns only empty ranges.
    }

}

