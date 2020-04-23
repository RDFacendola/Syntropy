
/// \file hash.h
/// \brief This header is part of the syntropy math system. It contains generic hash functions and definitions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <numeric>

#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* HASH                                                                 */
    /************************************************************************/

    /// \brief Exposes generic hash functions.
    /// \author Raffaele D. Facendola - April 2020.
    namespace Hash
    {

        /// \brief Get the non-cryptographic 64-bit hash of a buffer.
        int64_t FastHash64(const ConstMemoryRange& buffer);

        /// \brief Get the non-cryptographic 32-bit hash of a buffer.
        int32_t FastHash32(const ConstMemoryRange& buffer);

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Hash.

    inline int64_t Hash::FastHash64(const ConstMemoryRange& buffer)
    {
        // Uses FNV1a 64-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        return std::accumulate(buffer.Begin(), buffer.End(), std::int64_t(0xcbf29ce484222325), [](std::int64_t hash, const ConstMemoryAddress& address)
        {
            hash ^= *address.As<int8_t>();

            return (hash << 0) + (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40);
        });
    }

    inline int32_t Hash::FastHash32(const ConstMemoryRange& buffer)
    {
        // Uses FNV1a 32-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        return std::accumulate(buffer.Begin(), buffer.End(), std::int32_t(0x811c9dc5), [](int32_t hash, const ConstMemoryAddress& address)
        {
            hash ^= *address.As<int8_t>();

            return (hash << 0) + (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
        });
    }

}