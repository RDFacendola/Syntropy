/// \file encoding.h
/// \brief This header is part of the syntropy math system. It contains encoding methods for numbers.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>
#include <cstdlib>

#include "syntropy/math/vector.h"

namespace syntropy
{

    /// \brief Encode a binary number according to the Gray code.
    /// \param rhs Number to encode.
    /// \return Returns rhs encoded according to the Gray code.
    template <typename TNumber>
    constexpr TNumber ToGrayCode(TNumber rhs);

    /// \brief Decode a Gray code-encoded number to binary.
    /// \param rhs Number to decode.
    /// \return Returns rhs decoded according to the Gray code.
    template <typename TNumber>
    constexpr TNumber FromGrayCode(TNumber rhs);

    /// \brief Encode a binary number according to the Morton code 2D.
    /// Ths method interleaves each bit of rhs with 0s.
    /// \param rhs Number to encode.
    /// \return Returns rhs encoded according to the Morton code 2D.
    constexpr std::uint64_t ToMortonCode2(std::uint32_t rhs);

    /// \brief Decode a 2D Morton-encoded number to binary.
    /// \param rhs Number to decode.
    /// \return Returns rhs decoded according to the Morton code 2D.
    constexpr std::uint32_t FromMortonCode2(std::uint64_t rhs);

    /// \brief Encode a binary number according to the Morton code 2D.
    /// This method interleaves each bit of rhs with 00s.
    /// \param rhs Number to encode.
    /// \return Returns rhs encoded according to the Morton code 2D.
    constexpr std::uint64_t ToMortonCode3(std::uint32_t rhs);

    /// \brief Decode a 3D Morton-encoded number to binary.
    /// \param rhs Number to decode.
    /// \return Returns rhs decoded according to the Morton code 3D.
    constexpr std::uint32_t FromMortonCode3(std::uint64_t rhs);

    /// \brief Encode a 2-vector using the Morton code.
    /// \rhs Vector to encode.
    /// \return Returns rhs encoded using the Morton code.
    template <typename T>
    constexpr std::uint64_t ToMortonCode(const VectorN<T, 2>& rhs);

    /// \brief Encode a 3-vector using the Morton code.
    /// \rhs Vector to encode.
    /// \return Returns rhs encoded using the Morton code.
    template <typename T>
    constexpr std::uint64_t ToMortonCode(const VectorN<T, 3>& rhs);

    /// \brief Decode a 2-vector from a Morton code.
    /// \param lhs Morton code.
    /// \param rhs Vector to decode.
    template <typename T>
    constexpr void FromMortonCode(std::uint64_t lhs, VectorN<T, 2>& rhs);

    /// \brief Decode a 3-vector from a Morton code.
    /// \param lhs Morton code.
    /// \param rhs Vector to decode.
    template <typename T>
    constexpr void FromMortonCode(std::uint64_t lhs, VectorN<T, 3>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TNumber>
    constexpr TNumber ToGrayCode(TNumber rhs)
    {
        static_assert(std::is_integral_v<TNumber>, "TNumber must be integral.");

        return rhs ^ (rhs >> TNumber(1));
    }

    template <typename TNumber>
    constexpr TNumber FromGrayCode(TNumber rhs)
    {
        static_assert(std::is_integral_v<TNumber>, "TNumber must be integral.");

        for (auto mask = rhs >> TNumber(1); mask != 0; mask >>= TNumber(1))
        {
            rhs = rhs ^ mask;
        }

        return rhs;
    }

    constexpr std::uint64_t ToMortonCode2(std::uint32_t rhs)
    {
        auto result = std::uint64_t(rhs);

        result = (result | (result << 0x10)) & 0b0000000000000000111111111111111100000000000000001111111111111111;
        result = (result | (result << 0x08)) & 0b0000000011111111000000001111111100000000111111110000000011111111;
        result = (result | (result << 0x04)) & 0b0000111100001111000011110000111100001111000011110000111100001111;
        result = (result | (result << 0x02)) & 0b0011001100110011001100110011001100110011001100110011001100110011;
        result = (result | (result << 0x01)) & 0b0101010101010101010101010101010101010101010101010101010101010101;

        return result;
    }

    constexpr std::uint32_t FromMortonCode2(std::uint64_t rhs)
    {
        rhs = (rhs) & 0b0101010101010101010101010101010101010101010101010101010101010101;
        rhs = (rhs | (rhs >> 0x01)) & 0b0011001100110011001100110011001100110011001100110011001100110011;
        rhs = (rhs | (rhs >> 0x02)) & 0b0000111100001111000011110000111100001111000011110000111100001111;
        rhs = (rhs | (rhs >> 0x04)) & 0b0000000011111111000000001111111100000000111111110000000011111111;
        rhs = (rhs | (rhs >> 0x08)) & 0b0000000000000000111111111111111100000000000000001111111111111111;
        rhs = (rhs | (rhs >> 0x10)) & 0b0000000000000000000000000000000011111111111111111111111111111111;

        return std::uint32_t(rhs);
    }

    constexpr std::uint64_t ToMortonCode3(std::uint32_t rhs)
    {
        auto result = std::uint64_t(rhs);

        result = (result | (result << 0x10)) & 0b0000000011111111000000000000000011111111000000000000000011111111;
        result = (result | (result << 0x08)) & 0b1111000000001111000000001111000000001111000000001111000000001111;
        result = (result | (result << 0x04)) & 0b0011000011000011000011000011000011000011000011000011000011000011;
        result = (result | (result << 0x02)) & 0b1001001001001001001001001001001001001001001001001001001001001001;

        return result;
    }

    constexpr std::uint32_t FromMortonCode3(std::uint64_t rhs)
    {
        rhs = (rhs) & 0b1001001001001001001001001001001001001001001001001001001001001001;
        rhs = (rhs | (rhs >> 0x02)) & 0b0011000011000011000011000011000011000011000011000011000011000011;
        rhs = (rhs | (rhs >> 0x04)) & 0b1111000000001111000000001111000000001111000000001111000000001111;
        rhs = (rhs | (rhs >> 0x08)) & 0b0000000011111111000000000000000011111111000000000000000011111111;
        rhs = (rhs | (rhs >> 0x10)) & 0b0000000000000000000000000000000000000000111111111111111111111111;

        return std::uint32_t(rhs);
    }

    template <typename T>
    constexpr std::uint64_t ToMortonCode(const VectorN<T, 2>& rhs)
    {
        auto x = ToMortonCode2(T(rhs.x_)) << 0;
        auto y = ToMortonCode2(T(rhs.y_)) << 1;

        return x | y;
    }

    template <typename T>
    constexpr std::uint64_t ToMortonCode(const VectorN<T, 3>& rhs)
    {
        auto x = ToMortonCode3(T(rhs.x_)) << 0;
        auto y = ToMortonCode3(T(rhs.y_)) << 1;
        auto z = ToMortonCode3(T(rhs.z_)) << 2;

        return x | y | z;
    }

    template <typename T>
    constexpr void FromMortonCode(std::uint64_t lhs, VectorN<T, 2>& rhs)
    {
        rhs.x_ = T(FromMortonCode2(lhs >> 0));
        rhs.y_ = T(FromMortonCode2(lhs >> 1));
    }

    template <typename T>
    constexpr void FromMortonCode(std::uint64_t lhs, VectorN<T, 3>& rhs)
    {
        rhs.x_ = T(FromMortonCode2(lhs >> 0));
        rhs.y_ = T(FromMortonCode2(lhs >> 1));
        rhs.z_ = T(FromMortonCode2(lhs >> 2));
    }

}
