
/// \file virtual_memory.h
/// \brief This header is part of the Syntropy virtual memory module. It contains functionalities used to manage virtual memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/math/math.h"

namespace Syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    namespace VirtualMemory
    {
        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size.
        Memory::Bytes PageSize() noexcept;
 
        /// \brief Get the virtual memory page alignment.
        /// \return Returns the virtual memory page alignment.
        Memory::Alignment PageAlignment() noexcept;
 
        /// \brief Reserve a range of virtual memory addresses.
        /// Reserved memory region must be committed via Commit() before accessing it.
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved memory range. If the method fails returns an empty range.
        Memory::RWByteSpan Reserve(Memory::Bytes size) noexcept;
 
        /// \brief Allocate a range of virtual memory addresses.
        /// This method has the same effect as a Reserve() followed by a Commit().
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved memory range. If the method fails returns an empty range.
        Memory::RWByteSpan Allocate(Memory::Bytes size) noexcept;
 
        /// \brief Release a range of virtual memory addresses.
        /// \param byte_span Memory span to release. Must match any return value of a previous Reserve() / Allocate(), otherwise the behaviour is unspecified.
        /// \return Returns true if the range could be released, returns false otherwise.
        Bool Release(const Memory::RWByteSpan& byte_span) noexcept;

        /// \brief Commit a reserved virtual memory block.
        /// This method allocates all the pages containing at least one byte in the provided range and makes them accessible by the application.
        /// \param byte_span Memory range to commit.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        /// \remarks The provided memory range must refer to a memory region that was previously reserved via Reserve().
        Bool Commit(const Memory::RWByteSpan& byte_span) noexcept;

        /// \brief Decommit a virtual memory block.
        /// This method decommits all the pages containing at least one byte in the provided range.
        /// \param byte_span Memory range to decommit.
        Bool Decommit(const Memory::RWByteSpan& byte_span) noexcept;

        /// \brief Return the greatest size equal-to or smaller-than the provided value which is also a multiple of virtual memory's page size.
        Memory::Bytes Floor(Memory::Bytes size) noexcept;

        /// \brief Return the greatest alignment equal-to or smaller-than the provided value which also satisfies virtual memory's page alignment.
        Memory::Alignment Floor(Memory::Alignment alignment) noexcept;

        /// \brief Return the smallest size equal-to or greater-than the provided value which is also a multiple of virtual memory's page size.
        Memory::Bytes Ceil(Memory::Bytes size) noexcept;

        /// \brief Return the smallest alignment equal-to or greater-than the provided value which also satisfies virtual memory's page alignment.
        Memory::Alignment Ceil(Memory::Alignment alignment) noexcept;

        /// \brief Consume lhs from both sides until its first byte is aligned to virtual memory's page alignment and its size is a multiple of virtual memory's page size.
        Memory::ByteSpan Align(const Memory::ByteSpan& lhs) noexcept;

        /// \brief Consume lhs from both sides until its first byte is aligned to virtual memory's page alignment and its size is a multiple of virtual memory's page size.
        Memory::RWByteSpan Align(const Memory::RWByteSpan& lhs) noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualMemory
    // =============

    inline Memory::Bytes VirtualMemory::Floor(Memory::Bytes size) noexcept
    {
        return Math::Floor(size, PageSize());
    }

    inline Memory::Alignment VirtualMemory::Floor(Memory::Alignment alignment) noexcept
    {
        return Math::Min(alignment, PageAlignment());
    }

    inline Memory::Bytes VirtualMemory::Ceil(Memory::Bytes size) noexcept
    {
        return Math::Ceil(size, PageSize());
    }

    inline Memory::Alignment VirtualMemory::Ceil(Memory::Alignment alignment) noexcept
    {
        return Math::Max(alignment, PageAlignment());
    }

    inline Memory::ByteSpan VirtualMemory::Align(const Memory::ByteSpan& lhs) noexcept
    {
        return Memory::Align(lhs, PageSize(), PageAlignment());
    }

    inline Memory::RWByteSpan VirtualMemory::Align(const Memory::RWByteSpan& lhs) noexcept
    {
        return ReadWrite(Align(ReadOnly(lhs)));
    }
}

