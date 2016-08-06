
/// \file fnv1.h
/// \brief Provide a fast algorithm for hash computation.
///
/// \author Copyright 2008-2009 Daniel James, refactoring by Raffaele D. Facendola

// Copyright 2008-2009 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This code is also released into the public domain.

// Algorithm from: http://www.isthe.com/chongo/tech/comp/fnv/

#include <cstdint>
#include <string>
#include <numeric>

#pragma once

namespace syntropy {

    /// \brief Functor object used to compute the string hashes.
    template <uint64_t kPrime, uint64_t kOffsetBasis>
    struct FNV1aT {

        /// \brief Computes the hash of the provided string.
        /// \param string Source string.
        /// \return Returns the hash of the provided string.
        uint64_t operator()(const std::string& string) const;

    };


    const uint64_t kFNVPrime = 1099511628211u;
    const uint64_t kFNVOffsetBasis = 14695981039346656037u;

    /// \brief Default FNV1a definition.
    using FNV1a = FNV1aT<kFNVPrime, kFNVOffsetBasis>;
    
    //////////////// FNV 1 ////////////////

    template <uint64_t kPrime, uint64_t kOffsetBasis>
    inline uint64_t FNV1aT<kPrime, kOffsetBasis>::operator()(const std::string& string) const {

        return std::accumulate(std::begin(string),
                               std::end(string),
                               kFNVOffsetBasis,
                               [](uint64_t hash, std::string::value_type character) {

                                    return (hash ^ character) * kPrime;

                               });

    }

}