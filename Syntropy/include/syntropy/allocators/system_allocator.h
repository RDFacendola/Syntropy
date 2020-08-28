
/// \file system_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for allocators wrapping system heap memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

namespace Syntropy
{
    /************************************************************************/
    /* SYSTEM ALLOCATOR                                                     */
    /************************************************************************/

    /// \brief Tier 0 allocator used to allocate memory on system heap via new\delete calls.
    /// \author Raffaele D. Facendola - February 2017
    class SystemAllocator
    {
    public:

        /// \brief Default constructor.
        SystemAllocator() noexcept = default;

        /// \brief Default copy constructor.
        SystemAllocator(const SystemAllocator&) noexcept = default;

        /// \brief Default move constructor.
        SystemAllocator(SystemAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~SystemAllocator() noexcept = default;

        /// \brief Default assignment operator.
        SystemAllocator& operator=(const SystemAllocator&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // SystemAllocator.
    // ================

    inline RWByteSpan SystemAllocator::Allocate(Bytes size, Alignment alignment) noexcept
    {
        auto size_value = static_cast<std::size_t>(ToInt(size));
        auto alignment_value = static_cast<std::align_val_t>(ToInt(alignment));

        if (auto block = ::operator new(size_value, alignment_value, std::nothrow))
        {
            return { ToRWBytePtr(block), ToInt(size) };
        }

        return {};
    }

    inline void SystemAllocator::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        auto alignment_value = static_cast<std::align_val_t>(ToInt(alignment));

        ::operator delete(block.GetData(), alignment_value, std::nothrow);
    }

}


