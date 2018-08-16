
/// \file standard_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains an allocator around the standard malloc.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <new>

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* STANDARD ALLOCATOR                                                   */
    /************************************************************************/

    /// \brief Allocator used to wrap new\delete calls.
    /// \author Raffaele D. Facendola - February 2017
    class StandardAllocator
    {
    public:

        /// \brief Default constructor.
        StandardAllocator() noexcept = default;

        /// \brief Default copy constructor.
        StandardAllocator(const StandardAllocator&) noexcept = default;

        /// \brief Default move constructor.
        StandardAllocator(StandardAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~StandardAllocator() noexcept = default;

        /// \brief Default assignment operator.
        StandardAllocator& operator=(const StandardAllocator&) noexcept = default;

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

}

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/

namespace syntropy
{
    inline MemoryRange StandardAllocator::Allocate(Bytes size) noexcept
    {
        if (auto block = MemoryAddress{ ::operator new(std::size_t{size}, std::nothrow) })
        {
            return { block, block + size };
        }

        return {};
    }

    inline MemoryRange StandardAllocator::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = MemoryAddress{ ::operator new(std::size_t{size}, alignment, std::nothrow) })
        {
            return { block, block + size };
        }

        return {};
    }

    inline void StandardAllocator::Deallocate(const MemoryRange& block) noexcept
    {
        ::operator delete(block.Begin(), std::nothrow);
    }

    inline void StandardAllocator::Deallocate(const MemoryRange& block, Alignment alignment) noexcept
    {
        ::operator delete(block.Begin(), alignment, std::nothrow);
    }

}


