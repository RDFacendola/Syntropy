
/// \file virtual_memory.h
/// \brief This header is part of the Syntropy virtual memory module. It contains functionalities used to manage virtual memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/fundamentals.h"
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
        Bytes PageSize() noexcept;
 
        /// \brief Get the virtual memory page alignment.
        /// \return Returns the virtual memory page alignment.
        Alignment PageAlignment() noexcept;
 
        /// \brief Reserve a range of virtual memory addresses.
        /// Reserved memory region must be committed via Commit() before accessing it.
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved memory range. If the method fails returns an empty range.
        RWByteSpan Reserve(Bytes size) noexcept;
 
        /// \brief Allocate a range of virtual memory addresses.
        /// This method has the same effect as a Reserve() followed by a Commit().
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved memory range. If the method fails returns an empty range.
        RWByteSpan Allocate(Bytes size) noexcept;
 
        /// \brief Release a range of virtual memory addresses.
        /// \param byte_span Memory span to release. Must match any return value of a previous Reserve() / Allocate(), otherwise the behaviour is unspecified.
        /// \return Returns true if the range could be released, returns false otherwise.
        Bool Release(const RWByteSpan& byte_span) noexcept;

        /// \brief Commit a reserved virtual memory block.
        /// This method allocates all the pages containing at least one byte in the provided range and makes them accessible by the application.
        /// \param byte_span Memory range to commit.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        /// \remarks The provided memory range must refer to a memory region that was previously reserved via Reserve().
        Bool Commit(const RWByteSpan& byte_span) noexcept;

        /// \brief Decommit a virtual memory block.
        /// This method decommits all the pages containing at least one byte in the provided range.
        /// \param byte_span Memory range to decommit.
        Bool Decommit(const RWByteSpan& byte_span) noexcept;

        /// \brief Return the greatest size equal-to or smaller-than the provided value which is also a multiple of virtual memory's page size.
        Bytes Floor(Bytes size) noexcept;

        /// \brief Return the greatest alignment equal-to or smaller-than the provided value which also satisfies virtual memory's page alignment.
        Alignment Floor(Alignment alignment) noexcept;

        /// \brief Return the smallest size equal-to or greater-than the provided value which is also a multiple of virtual memory's page size.
        Bytes Ceil(Bytes size) noexcept;

        /// \brief Return the smallest alignment equal-to or greater-than the provided value which also satisfies virtual memory's page alignment.
        Alignment Ceil(Alignment alignment) noexcept;

        /// \brief Consume lhs from both sides until its first byte is aligned to virtual memory's page alignment and its size is a multiple of virtual memory's page size.
        ByteSpan Align(const ByteSpan& lhs) noexcept;

        /// \brief Consume lhs from both sides until its first byte is aligned to virtual memory's page alignment and its size is a multiple of virtual memory's page size.
        RWByteSpan Align(const RWByteSpan& lhs) noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualMemory
    // =============

    inline Bytes VirtualMemory::Floor(Bytes size) noexcept
    {
        return Math::Floor(size, PageSize());
    }

    inline Alignment VirtualMemory::Floor(Alignment alignment) noexcept
    {
        return Math::Min(alignment, PageAlignment());
    }

    inline Bytes VirtualMemory::Ceil(Bytes size) noexcept
    {
        return Math::Ceil(size, PageSize());
    }

    inline Alignment VirtualMemory::Ceil(Alignment alignment) noexcept
    {
        return Math::Max(alignment, PageAlignment());
    }

    inline ByteSpan VirtualMemory::Align(const ByteSpan& lhs) noexcept
    {
        return Memory::Align(lhs, PageSize(), PageAlignment());
    }

    inline RWByteSpan VirtualMemory::Align(const RWByteSpan& lhs) noexcept
    {
        return ReadWrite(Align(ReadOnly(lhs)));
    }
}

