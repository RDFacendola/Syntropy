
/// \file virtual_memory.h
/// \brief This header is part of the syntropy memory management system. It contains functionalities used to manage virtual memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/virtual_memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class VirtualMemory
    {
    public:

         /// \brief Get the virtual memory page size.
         /// \return Returns the virtual memory page size, in bytes.
         static Bytes GetPageSize();
 
         /// \brief Get the virtual memory page alignment.
         /// \return Returns the virtual memory page alignment, in bytes.
         static Alignment GetPageAlignment();
 
         /// \brief Reserve a range of virtual memory addresses.
         /// Reserved memory pages must be committed via Commit() before accessing them.
         /// \param size Size of the range to reserve, in bytes.
         /// \return Returns the reserved memory range. If the method fails returns an empty range.
         static VirtualMemoryRange Reserve(Bytes size);
 
         /// \brief Allocate a range of virtual memory addresses.
         /// This method has the same effect as a Reserve() followed by a Commit().
         /// \param size Size of the range to reserve, in bytes.
         /// \return Returns the reserved memory range. If the method fails returns an empty range.
         static VirtualMemoryRange Allocate(Bytes size);
 
         /// \brief Release a range of virtual memory addresses.
         /// \param address First address of the range to release. Must match any return value of a previous Reserve() / Allocate(), otherwise the behaviour is unspecified.
         /// \return Returns true if the range could be released, returns false otherwise.
         static bool Release(const VirtualMemoryRange& memory_range);

         /// \brief Commit a reserved virtual memory block.
         /// This method allocates all the pages containing at least one byte in the provided range and makes them accessible by the application.
         /// \param memory_range Memory range to commit.
         /// \return Returns true if the memory could be committed, returns false otherwise.
         /// \remarks The provided memory range must refer to a memory region that was previously reserved via Reserve().
         static bool Commit(const VirtualMemoryRange& memory_range);

         /// \brief Decommit a virtual memory block.
         /// This method decommits all the pages containing at least one byte in the provided range.
         /// \param memory_range Memory range to decommit.
         static bool Decommit(const VirtualMemoryRange& memory_range);

    };

}

