
/// \file hash.h
/// \brief This header is part of the syntropy math system. It contains generic hash functions and definitions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <string>
#include <numeric>

namespace syntropy
{
    /// \brief Exposes hashing functions.
    /// \author Raffaele D. Facendola - 2016.
    struct Hash
    {
        /// \brief Get the non-cryptographic 64-bit hash of a string.
        /// \param string String to hash.
        /// \return Returns the 64-bit hash of the provided string.
        template <typename TString>
        static uint64_t Hash64(const TString& string)
        {
            return Hash64(string.c_str(), string.length() * sizeof(typename TString::value_type));
        }

        /// \brief Get the non-cryptographic 32-bit hash of a string.
        /// \param string String to hash.
        /// \return Returns the 32-bit hash of the provided string.
        template <typename TString>
        static uint32_t Hash32(const TString& string)
        {
            return Hash32(string.c_str(), string.length() * sizeof(typename TString::value_type));
        }

        /// \brief Get the non-cryptographic 64-bit hash of a buffer.
        /// \param buffer Buffer to hash.
        /// \param length Length of the buffer, in bytes.
        /// \return Returns the 64-bit hash of the provided string.
        static uint64_t Hash64(const void* buffer, size_t length);

        /// \brief Get the non-cryptographic 32-bit hash of a buffer.
        /// \param buffer Buffer to hash.
        /// \param length Length of the buffer, in bytes.
        /// \return Returns the 32-bit hash of the provided string.
        static uint32_t Hash32(const void* buffer, size_t length);
    };

    /// \brief 64-bit non-cryptographic string hasher.
    template <typename TString>
    struct StringHasher64
    {
        auto operator()(const TString& string) const
        {
            return Hash::Hash64(string);
        }
    };

    /// \brief 32-bit non-cryptographic string hasher.
    template <typename TString>
    struct StringHasher32
    {
        auto operator()(const TString& string) const
        {
            return Hash::Hash32(string);
        }
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Hash64.

    inline uint64_t Hash::Hash64(const void* buffer, size_t length)
    {
        // Uses FNV1a 64-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        auto begin = reinterpret_cast<const uint8_t*>(buffer);

        return std::accumulate(begin, begin + length, uint64_t(0xcbf29ce484222325U), [](uint64_t hash, uint8_t character)
        {
            hash ^= static_cast<uint64_t>(character);

            return (hash << 0) + (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40);
        });
    }

    // Hash32.
    inline uint32_t Hash::Hash32(const void* buffer, size_t length)
    {
        // Uses FNV1a 32-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        auto begin = reinterpret_cast<const uint8_t*>(buffer);

        return std::accumulate(begin, begin + length, uint32_t(0x811c9dc5U), [](uint32_t hash, uint8_t character)
        {
            hash ^= static_cast<uint32_t>(character);

            return (hash << 0) + (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
        });
    }

}