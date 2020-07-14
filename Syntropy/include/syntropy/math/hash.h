
/// \file hash.h
/// \brief This header is part of the Syntropy math module. It contains generic hash functions and definitions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <numeric>

#include "syntropy/core/types.h"
#include "syntropy/memory/memory_span.h"

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
        Int FNV1a64(const ReadOnlyMemorySpan& buffer);

        /// \brief Get the non-cryptographic 32-bit FNV1a hash of a buffer.
        Int FNV1a32(const ReadOnlyMemorySpan& buffer);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Hash.

    inline Int Hash::FNV1a64(const ReadOnlyMemorySpan& buffer)
    {
        // Uses FNV1a 64-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        auto hash = ToInt(0xcbf29ce484222325);

        for (auto&& byte : buffer)
        {
            hash ^= ToInt(byte);

            hash = (hash << 0) + (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40);
        }

        return hash;
    }

    inline Int Hash::FNV1a32(const ReadOnlyMemorySpan& buffer)
    {
        // Uses FNV1a 32-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        auto hash = ToInt(0x811c9dc5);

        for (auto&& byte : buffer)
        {
            hash ^= ToInt(byte);

            hash = (hash << 0) + (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
        }

        return hash & 0xFFFFFFFF;
    }

}