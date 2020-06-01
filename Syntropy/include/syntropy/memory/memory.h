
/// \file memory.h
/// \brief This header is part of the Syntropy memory module. It contains generic functionalities used to manipulate memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstring>
#include <algorithm>

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Exposes utility methods to manipulate memory.
    namespace Memory
    {
        /// \brief Copy a source memory region to a destination memory region. Neither range is exceed during the process.
        /// If source and destination overlap, the behavior of this function is undefined.
        /// \return Returns the bytes copied as a result of this call.
        Bytes Copy(const MemoryRange& destination, const ConstMemoryRange& source);

        /// \brief Copy a source memory region to a destination memory region. Neither range is exceed during the process.
        /// Source and destination may overlap, in this case the function behaves as if source was copied to a temporary buffer and then copied from there to the destination.
        /// \return Returns the bytes copied as a result of this call.
        Bytes Move(const MemoryRange& destination, const ConstMemoryRange& source);

        /// \brief Set a value to each byte in a destination range.
        void Set(const MemoryRange& destination, Byte value);

        /// \brief Zero-out a memory region.
        void Zero(const MemoryRange& destination);
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Memory.

    inline Bytes Memory::Copy(const MemoryRange& destination, const ConstMemoryRange& source)
    {
        SYNTROPY_ASSERT(!source.Overlaps(destination));

        auto bytes = std::min(source.GetSize(), destination.GetSize());

        if (bytes > Bytes{ 0 })
        {
            std::memcpy(*destination.Begin(), *source.Begin(), bytes.GetCount());
        }

        return bytes;
    }

    inline Bytes Memory::Move(const MemoryRange& destination, const ConstMemoryRange& source)
    {
        auto bytes = std::min(source.GetSize(), destination.GetSize());

        if (bytes > Bytes{ 0 })
        {
            std::memmove(*destination.Begin(), *source.Begin(), bytes.GetCount());
        }

        return bytes;
    }

    inline void Memory::Set(const MemoryRange& destination, Byte value)
    {
        std::memset(*destination.Begin(), value, destination.GetSize().GetCount());
    }

    inline void Memory::Zero(const MemoryRange& destination)
    {
        Set(destination, 0);
    }

}

