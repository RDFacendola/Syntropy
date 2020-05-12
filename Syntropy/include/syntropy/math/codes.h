/// \file codes.h
/// \brief This header is part of the Syntropy math module. It contains encoding and decoding methods for numbers.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>
#include <cstdlib>

#include "syntropy/math/vector.h"

namespace syntropy
{
    /************************************************************************/
    /* CODES                                                                */
    /************************************************************************/

    /// \brief Exposes codes-related functionalities.
    /// \author Raffaele D. Facendola - 2018.
    namespace Codes
    {
        /// \brief Encode a binary number according to the Gray code.
        /// \param rhs Number to encode.
        /// \return Returns rhs encoded according to the Gray code.
        constexpr std::int64_t ToGrayCode(std::int64_t rhs);

        /// \brief Decode a Gray code-encoded number to binary.
        /// \param rhs Number to decode.
        /// \return Returns rhs decoded according to the Gray code.
        constexpr std::int64_t FromGrayCode(std::int64_t rhs);

        /// \brief Encode a 2-vector using the Morton code.
        /// \rhs Vector to encode.
        /// \return Returns rhs encoded using the Morton code.
        constexpr std::int64_t ToMortonCode(const Math::Int2& rhs);

        /// \brief Encode a 3-vector using the Morton code.
        /// \rhs Vector to encode.
        /// \return Returns rhs encoded using the Morton code.
        constexpr std::int64_t ToMortonCode(const Math::Int3& rhs);

        /// \brief Decode a 2-vector from a Morton code.
        /// \param lhs Morton code.
        Math::Int2 FromMortonCode2(std::int64_t lhs);

        /// \brief Decode a 3-vector from a Morton code.
        /// \param lhs Morton code.
        /// \param rhs Vector to decode.
        Math::Int3 FromMortonCode3(std::int64_t lhs);

        /// \brief Interleave each bit of rhs with 0.
        /// \param rhs Number to encode.
        /// \return Returns rhs encoded according to the Morton code 2D.
        constexpr std::int64_t InterleaveMortonCode2(std::int64_t rhs);

        /// \brief De-interleaves each pair of bit.
        /// \param rhs Number to decode.
        /// \return Returns rhs decoded according to the Morton code 2D.
        constexpr std::int64_t DeinterleaveMortonCode2(std::int64_t rhs);

        /// \brief Interleave each bit of rhs with 00.
        /// \param rhs Number to encode.
        /// \return Returns rhs encoded according to the Morton code 2D.
        constexpr std::int64_t InterleaveMortonCode3(std::int64_t rhs);

        /// \brief De-interleaves each triplet of bit.
        /// \param rhs Number to decode.
        /// \return Returns rhs decoded according to the Morton code 3D.
        constexpr std::int64_t DeinterleaveMortonCode3(std::int64_t rhs);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Codes.

    constexpr std::int64_t Codes::ToGrayCode(std::int64_t rhs)
    {
        return rhs ^ (rhs >> std::int64_t(1));
    }

    constexpr std::int64_t Codes::FromGrayCode(std::int64_t rhs)
    {
        for (auto mask = rhs >> std::int64_t(1); mask != 0; mask >>= std::int64_t(1))
        {
            rhs = rhs ^ mask;
        }

        return rhs;
    }

    constexpr std::int64_t Codes::ToMortonCode(const Math::Int2& rhs)
    {
        auto x = InterleaveMortonCode2(rhs.x_) << 0;
        auto y = InterleaveMortonCode2(rhs.y_) << 1;

        return x | y;
    }

    constexpr std::int64_t Codes::ToMortonCode(const Math::Int3& rhs)
    {
        auto x = InterleaveMortonCode3(rhs.x_) << 0;
        auto y = InterleaveMortonCode3(rhs.y_) << 1;
        auto z = InterleaveMortonCode3(rhs.z_) << 2;

        return x | y | z;
    }

    inline Math::Int2 Codes::FromMortonCode2(std::int64_t lhs)
    {
        return { DeinterleaveMortonCode2(lhs >> 0), DeinterleaveMortonCode2(lhs >> 1) };
    }

    inline Math::Int3 Codes::FromMortonCode3(std::int64_t lhs)
    {
        return { DeinterleaveMortonCode3(lhs >> 0), DeinterleaveMortonCode3(lhs >> 1), DeinterleaveMortonCode3(lhs >> 2) };
    }

    constexpr std::int64_t Codes::InterleaveMortonCode2(std::int64_t rhs)
    {
        auto result = rhs & 0b0000000000000000000000000000000011111111111111111111111111111111;

        result = (result | (result << 0x10)) & 0b0000000000000000111111111111111100000000000000001111111111111111;
        result = (result | (result << 0x08)) & 0b0000000011111111000000001111111100000000111111110000000011111111;
        result = (result | (result << 0x04)) & 0b0000111100001111000011110000111100001111000011110000111100001111;
        result = (result | (result << 0x02)) & 0b0011001100110011001100110011001100110011001100110011001100110011;
        result = (result | (result << 0x01)) & 0b0101010101010101010101010101010101010101010101010101010101010101;

        return result;
    }

    constexpr std::int64_t Codes::DeinterleaveMortonCode2(std::int64_t rhs)
    {
        rhs = (rhs) & 0b0101010101010101010101010101010101010101010101010101010101010101;

        rhs = (rhs | (rhs >> 0x01)) & 0b0011001100110011001100110011001100110011001100110011001100110011;
        rhs = (rhs | (rhs >> 0x02)) & 0b0000111100001111000011110000111100001111000011110000111100001111;
        rhs = (rhs | (rhs >> 0x04)) & 0b0000000011111111000000001111111100000000111111110000000011111111;
        rhs = (rhs | (rhs >> 0x08)) & 0b0000000000000000111111111111111100000000000000001111111111111111;
        rhs = (rhs | (rhs >> 0x10)) & 0b0000000000000000000000000000000011111111111111111111111111111111;

        return rhs & 0b0000000000000000000000000000000011111111111111111111111111111111;
    }

    constexpr std::int64_t Codes::InterleaveMortonCode3(std::int64_t rhs)
    {
        auto result = rhs & 0b0000000000000000000000000000000000000000111111111111111111111111;

        result = (result | (result << 0x10)) & 0b0000000011111111000000000000000011111111000000000000000011111111;
        result = (result | (result << 0x08)) & 0b1111000000001111000000001111000000001111000000001111000000001111;
        result = (result | (result << 0x04)) & 0b0011000011000011000011000011000011000011000011000011000011000011;
        result = (result | (result << 0x02)) & 0b1001001001001001001001001001001001001001001001001001001001001001;

        return result;
    }

    constexpr std::int64_t Codes::DeinterleaveMortonCode3(std::int64_t rhs)
    {
        rhs = (rhs) & 0b1001001001001001001001001001001001001001001001001001001001001001;

        rhs = (rhs | (rhs >> 0x02)) & 0b0011000011000011000011000011000011000011000011000011000011000011;
        rhs = (rhs | (rhs >> 0x04)) & 0b1111000000001111000000001111000000001111000000001111000000001111;
        rhs = (rhs | (rhs >> 0x08)) & 0b0000000011111111000000000000000011111111000000000000000011111111;
        rhs = (rhs | (rhs >> 0x10)) & 0b0000000000000000000000000000000000000000111111111111111111111111;

        return rhs & 0b0000000000000000000000000000000000000000111111111111111111111111;
    }

}
