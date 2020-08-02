
/// \file null_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains allocators that use no memory.
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
    /* NULL ALLOCATOR                                                       */
    /************************************************************************/

    /// \brief Tier 0 memory resource that rejects any form of allocation.
    /// \author Raffaele D. Facendola - August 2018
    class NullAllocator
    {
    public:

        /// \brief Default constructor.
        NullAllocator() noexcept = default;

        /// \brief Default copy constructor.
        NullAllocator(const NullAllocator&) noexcept = default;

        /// \brief Default move constructor.
        NullAllocator(NullAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~NullAllocator() noexcept = default;

        /// \brief Default assignment operator.
        NullAllocator& operator=(const NullAllocator&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// \return Returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks If the provided block is not empty the behavior of this function is undefined.
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Check whether the memory resource owns a memory block.
        /// \remarks Only empty blocks are owned by this allocator, even empty blocks allocated on another allocator.
        Bool Owns(const ByteSpan& block) const noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/
 
    // NullAllocator.
    // ==============

    inline RWByteSpan NullAllocator::Allocate(Bytes /*size*/, Alignment /*alignment*/) noexcept
    {
        return {};
    }

    inline void NullAllocator::Deallocate(const RWByteSpan& block, Alignment /*alignment*/)
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(Owns(block));       // Empty memory blocks only.
    }

    inline Bool NullAllocator::Owns(const ByteSpan& block) const noexcept
    {
        return !block;                                  // This memory resource owns only empty ranges.
    }

}

