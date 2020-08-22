
/// \file null_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for allocators which don't use any memory.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/language/language.h"
#include "syntropy/diagnostics/assert.h"

namespace Syntropy
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
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Deallocate each allocation performed so far.
        void DeallocateAll() noexcept;

        /// \brief Check whether the allocator owns a memory block.
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
        SYNTROPY_UNDEFINED_BEHAVIOR(Owns(block));
    }

    inline void NullAllocator::DeallocateAll() noexcept
    {

    }

    inline Bool NullAllocator::Owns(const ByteSpan& block) const noexcept
    {
        return !block;
    }

}

