
/// \file system_allocator.h
///
/// \brief This header is part of the Syntropy allocators module.
///        It contains definitions for allocators wrapping system heap memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/alignment.h"
#include "syntropy/memory/foundation/byte_span.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* SYSTEM ALLOCATOR                                                     */
    /************************************************************************/

    /// \brief Tier 0 allocator used to allocate memory on system heap via
    ///        new\delete calls.
    /// \author Raffaele D. Facendola - February 2017
    class SystemAllocator
    {
    public:

        /// \brief Default constructor.
        SystemAllocator() noexcept = default;

        /// \brief Default copy constructor.
        SystemAllocator(Immutable<SystemAllocator>) noexcept = default;

        /// \brief Default move constructor.
        SystemAllocator(Movable<SystemAllocator>) noexcept = default;

        /// \brief Default destructor.
        ~SystemAllocator() noexcept = default;

        /// \brief Default assignment operator.
        SystemAllocator&
        operator=(Immutable<SystemAllocator>) noexcept = default;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless
        ///          the provided block was returned by a previous call to
        ///          ::Allocate(size, alignment).
        void
        Deallocate(Immutable<RWByteSpan> block, Alignment alignment) noexcept;

    };

}

// ===========================================================================

#include "details/system_allocator.inl"

// ===========================================================================
