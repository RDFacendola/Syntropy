
/// \file hal_virtual_memory.h
/// \brief This header is part of the Syntropy hardware abstraction layer module. It exposes APIs needed to handle virtual memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

namespace syntropy
{
    /************************************************************************/
    /* HAL MEMORY                                                           */
    /************************************************************************/

    /// \brief Abstracts low level primitives used to handle virtual memory.
    /// \author Raffaele D. Facendola - 2017
    namespace HALMemory
    {
        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size, in bytes.
        Bytes GetPageSize() noexcept;

        /// \brief Get the virtual memory page alignment.
        /// \return Returns the virtual memory page alignment, in bytes.
        Alignment GetPageAlignment() noexcept;

        /// \brief Reserve a range of virtual memory addresses.
        /// Reserved memory region must be committed via Commit() before accessing it.
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved memory range. If the method fails returns an empty range.
        RWByteSpan Reserve(Bytes size) noexcept;

        /// \brief Allocate a range of virtual memory addresses.
        /// This method has the same effect as a Reserve() followed by a Commit().
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the reserved virtual memory range. If the method fails returns an empty range.
        RWByteSpan Allocate(Bytes size) noexcept;

        /// \brief Release a range of virtual memory addresses.
        /// \param byte_span Memory range to release. Must match any return value of a previous Reserve() / Allocate(), otherwise the behaviour is unspecified.
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

    };

}