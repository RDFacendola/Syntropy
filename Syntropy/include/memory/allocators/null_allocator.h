
/// \file null_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains empty allocators.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

#include "diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* NULL ALLOCATOR                                                       */
    /************************************************************************/

    /// \brief Basic allocator that rejects any form of allocation.
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

        /// \brief Check whether this allocator owns the provided memory block.
        /// The null allocator only contains empty ranges.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;
    };

}

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/
 
namespace syntropy
{
    inline MemoryRange NullAllocator::Allocate(Bytes /*size*/) noexcept
    {
        return {};
    }

    inline MemoryRange NullAllocator::Allocate(Bytes /*size*/, Alignment /*alignment*/) noexcept
    {
        return {};
    }

    inline void NullAllocator::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(!block);        // Only empty ranges can be "deallocated" by this allocator.
    }

    inline void NullAllocator::Deallocate(const MemoryRange& block, Alignment /*alignment*/)
    {
        SYNTROPY_ASSERT(!block);        // Only empty ranges can be "deallocated" by this allocator.
    }

    inline bool NullAllocator::Owns(const MemoryRange& block) const noexcept
    {
        return !block;                  // This allocator "owns" only empty ranges.
    }

    inline Bytes NullAllocator::GetMaxAllocationSize() const noexcept
    {
        return 0_Bytes;
    }
}
