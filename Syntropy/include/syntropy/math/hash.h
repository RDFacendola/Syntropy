
/// \file hash.h
/// \brief This header is part of the Syntropy math module. It contains generic hash functions and definitions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <numeric>

#include "syntropy/core/types.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* HASH                                                                 */
    /************************************************************************/

    /// \brief Exposes generic hashing functionalities.
    /// \author Raffaele D. Facendola - May 2020.
    namespace Hash
    {
        /// \brief Get the non-cryptographic 64-bit FNV1a hash of a buffer.
        Int FNV1a64(const ConstMemoryRange& buffer);

        /// \brief Get the non-cryptographic 32-bit FNV1a hash of a buffer.
        Int FNV1a32(const ConstMemoryRange& buffer);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Hash.

    inline Int Hash::FNV1a64(const ConstMemoryRange& buffer)
    {
        // Uses FNV1a 64-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        return std::accumulate(buffer.Begin(), buffer.End(), Int(0xcbf29ce484222325), [](Int hash, Byte byte)
        {
            hash ^= ToInt(byte);

            return (hash << 0) + (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40);
        });
    }

    inline Int Hash::FNV1a32(const ConstMemoryRange& buffer)
    {
        // Uses FNV1a 32-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        return std::accumulate(buffer.Begin(), buffer.End(), Int(0x811c9dc5), [](Int hash, Byte byte)
        {
            hash ^= ToInt(byte);

            return (hash << 0) + (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
        }) & 0xFFFFFFFF;
    }

}