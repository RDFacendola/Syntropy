
/// \file virtual_memory.h
/// \brief This header is part of the Syntropy memory module. It contains functionalities used to manage virtual memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    namespace Memory
    {
        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size.
        Bytes VirtualPageSize() noexcept;
 
        /// \brief Get the virtual memory page alignment.
        /// \return Returns the virtual memory page alignment.
        Alignment VirtualPageAlignment() noexcept;
 
        /// \brief Reserve a range of virtual memory addresses.
        /// Reserved memory region must be committed via VirtualCommit() before accessing it.
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved memory range. If the method fails returns an empty range.
        RWByteSpan VirtualReserve(Bytes size) noexcept;
 
        /// \brief Allocate a range of virtual memory addresses.
        /// This method has the same effect as a VirtualReserve() followed by a VirtualCommit().
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved memory range. If the method fails returns an empty range.
        RWByteSpan VirtualAllocate(Bytes size) noexcept;
 
        /// \brief Release a range of virtual memory addresses.
        /// \param byte_span Memory span to release. Must match any return value of a previous VirtualReserve() / VirtualAllocate(), otherwise the behaviour is unspecified.
        /// \return Returns true if the range could be released, returns false otherwise.
        Bool VirtualRelease(const RWByteSpan& byte_span) noexcept;

        /// \brief Commit a reserved virtual memory block.
        /// This method allocates all the pages containing at least one byte in the provided range and makes them accessible by the application.
        /// \param byte_span Memory range to commit.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        /// \remarks The provided memory range must refer to a memory region that was previously reserved via VirtualReserve().
        Bool VirtualCommit(const RWByteSpan& byte_span) noexcept;

        /// \brief Decommit a virtual memory block.
        /// This method decommits all the pages containing at least one byte in the provided range.
        /// \param byte_span Memory range to decommit.
        Bool VirtualDecommit(const RWByteSpan& byte_span) noexcept;

        /// \brief Return the greatest size equal-to or smaller-than the provided value which is also a multiple of virtual memory's page size.
        Bytes VirtualFloor(Bytes size) noexcept;

        /// \brief Return the greatest alignment equal-to or smaller-than the provided value which also satisfies virtual memory's page alignment.
        Alignment VirtualFloor(Alignment alignment) noexcept;

        /// \brief Return the smallest size equal-to or greater-than the provided value which is also a multiple of virtual memory's page size.
        Bytes VirtualCeil(Bytes size) noexcept;

        /// \brief Return the smallest alignment equal-to or greater-than the provided value which also satisfies virtual memory's page alignment.
        Alignment VirtualCeil(Alignment alignment) noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Memory.
    // =======

    inline Bytes Memory::VirtualFloor(Bytes size) noexcept
    {
        return Math::Floor(size, VirtualPageSize());
    }

    inline Alignment Memory::VirtualFloor(Alignment alignment) noexcept
    {
        return Math::Min(alignment, VirtualPageAlignment());
    }

    inline Bytes Memory::VirtualCeil(Bytes size) noexcept
    {
        return Math::Ceil(size, VirtualPageSize());
    }

    inline Alignment Memory::VirtualCeil(Alignment alignment) noexcept
    {
        return Math::Max(alignment, VirtualPageAlignment());
    }

}

