
/// \file null_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains memory resources that use no memory at all.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
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

        /// \brief Allocate a new memory block.
        /// Allocations performed on this allocator are rejected, hence this method returns empty spans only.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was empty.
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Check whether the memory resource owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/
 
    // NullMemoryResource.
    // ===================

    inline RWByteSpan NullMemoryResource::Allocate(Bytes /*size*/, Alignment /*alignment*/) noexcept
    {
        return {};
    }

    inline void NullMemoryResource::Deallocate(const RWByteSpan& block, Alignment /*alignment*/)
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(Owns(block));       // Empty memory blocks only.
    }

    inline Bool NullMemoryResource::Owns(const ByteSpan& block) const noexcept
    {
        return !block;                                  // This memory resource owns only empty ranges.
    }

}

