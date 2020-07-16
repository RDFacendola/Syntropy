
/// \file virtual_memory.h
/// \brief This header is part of the Syntropy memory module. It contains functionalities used to manage virtual memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_span.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    namespace VirtualMemory
    {
         /// \brief Get the virtual memory page size.
         /// \return Returns the virtual memory page size, in bytes.
         Bytes GetPageSize();
 
         /// \brief Get the virtual memory page alignment.
         /// \return Returns the virtual memory page alignment, in bytes.
         Alignment GetPageAlignment();
 
         /// \brief Reserve a range of virtual memory addresses.
         /// Reserved memory region must be committed via Commit() before accessing it.
         /// \param size Size of the range to reserve, in bytes.
         /// \return Returns the reserved memory range. If the method fails returns an empty range.
         RWMemorySpan Reserve(Bytes size);
 
         /// \brief Allocate a range of virtual memory addresses.
         /// This method has the same effect as a Reserve() followed by a Commit().
         /// \param size Size of the range to reserve, in bytes.
         /// \return Returns the reserved memory range. If the method fails returns an empty range.
         RWMemorySpan Allocate(Bytes size);
 
         /// \brief Release a range of virtual memory addresses.
         /// \param memory_span Memory span to release. Must match any return value of a previous Reserve() / Allocate(), otherwise the behaviour is unspecified.
         /// \return Returns true if the range could be released, returns false otherwise.
         Bool Release(const RWMemorySpan& memory_span);

         /// \brief Commit a reserved virtual memory block.
         /// This method allocates all the pages containing at least one byte in the provided range and makes them accessible by the application.
         /// \param memory_span Memory range to commit.
         /// \return Returns true if the memory could be committed, returns false otherwise.
         /// \remarks The provided memory range must refer to a memory region that was previously reserved via Reserve().
         Bool Commit(const RWMemorySpan& memory_span);

         /// \brief Decommit a virtual memory block.
         /// This method decommits all the pages containing at least one byte in the provided range.
         /// \param memory_span Memory range to decommit.
         Bool Decommit(const RWMemorySpan& memory_span);

    };

}

