
/// \file msgpack.h
/// \brief This header is part of the Syntropy serialization module. It contains classes for the Msgpack decoding.
///
/// Specification found here: https://github.com/msgpack/msgpack/blob/master/spec.md
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>

#include "syntropy/serialization/msgpack/format.h"
#include "syntropy/platform/endianness.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK                                                              */
    /************************************************************************/

    /// \brief Defines methods used to decode values using the Msgpack specification.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Msgpack
    {
        /// \brief Decode a positive fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the positive fix int specification.
        std::int8_t DecodePositiveFixUInt(std::int8_t rhs);

        /// \brief Decode a positive fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the positive fix int specification.
        std::int8_t DecodePositiveFixInt(std::int8_t rhs);

        /// \brief Decode the length of a fixed-length map.
        std::int8_t DecodeFixMapLength(std::int8_t rhs);

        /// \brief Decode the length of a fixed-length array.
        std::int8_t DecodeFixArrayLength(std::int8_t rhs);

        /// \brief Decode the length of a fixed-length string.
        std::int8_t DecodeFixStrLength(std::int8_t rhs);

        /// \brief Decode an 8-bit value.
        template <typename TDecoded>
        TDecoded Decode(std::int8_t rhs);

        /// \brief Decode a 16-bit value.
        template <typename TDecoded>
        TDecoded Decode(std::int16_t rhs);

        /// \brief Decode a 32-bit value.
        template <typename TDecoded>
        TDecoded Decode(std::int32_t rhs);

        /// \brief Decode a 64-bit value.
        template <typename TDecoded>
        TDecoded Decode(std::int64_t rhs);

        /// \brief Decode a negative fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the negative fix int specification.
        std::int8_t DecodeNegativeFixInt(std::int8_t rhs);
    }

    /************************************************************************/
    /* MSGPACK                                                              */
    /************************************************************************/

    /// \brief Define methods used to check encoded format types during decoding.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Msgpack
    {
        /// \brief Check whether rhs represents a positive fix int type format.
        bool IsPositiveFixIntFormat(std::int8_t rhs);

        /// \brief Check whether rhs represents a fixed-length map type format.
        bool IsFixMapFormat(std::int8_t rhs);

        /// \brief Check whether rhs represents a fixed-length array type format.
        bool IsFixArrayFormat(std::int8_t rhs);

        /// \brief Check whether rhs represents a fixed-length string type format.
        bool IsFixStrFormat(std::int8_t rhs);

        /// \brief Check whether rhs represents a negative fix int type format.
        bool IsNegativeFixIntFormat(std::int8_t rhs);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Msgpack.

    inline std::int8_t Msgpack::DecodePositiveFixUInt(std::int8_t rhs)
    {
        return rhs & 0b01111111;
    }

    inline std::int8_t Msgpack::DecodePositiveFixInt(std::int8_t rhs)
    {
        return rhs & 0b01111111;
    }

    inline std::int8_t Msgpack::DecodeFixMapLength(std::int8_t rhs)
    {
        return rhs & 0b00001111;
    }

    inline std::int8_t Msgpack::DecodeFixArrayLength(std::int8_t rhs)
    {
        return rhs & 0b00001111;
    }

    inline std::int8_t Msgpack::DecodeFixStrLength(std::int8_t rhs)
    {
        return rhs & 0b00011111;
    }

    template <>
    inline std::int8_t Msgpack::Decode<std::int8_t>(std::int8_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    template <>
    inline std::int16_t Msgpack::Decode<std::int16_t>(std::int16_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    template <>
    inline float Msgpack::Decode<float>(std::int32_t rhs)
    {
        auto value = float{};

        rhs = Endianness::FromBigEndian(rhs);

        std::memcpy(&value, &rhs, sizeof(float));

        return value;
    }

    template <>
    inline std::int32_t Msgpack::Decode<std::int32_t>(std::int32_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    template <>
    inline double Msgpack::Decode<double>(std::int64_t rhs)
    {
        auto value = double{};

        rhs = Endianness::FromBigEndian(rhs);

        std::memcpy(&value, &rhs, sizeof(double));

        return value;
    }

    template <>
    inline std::int64_t Msgpack::Decode<std::int64_t>(std::int64_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    inline std::int8_t Msgpack::DecodeNegativeFixInt(std::int8_t rhs)
    {
        return -(rhs & 0b00011111);
    }

    // Msgpack.

    inline bool Msgpack::IsPositiveFixIntFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kPositiveFixInt) == std::int8_t(rhs & 0b10000000);
    }

    inline bool Msgpack::IsFixMapFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixMap) == std::int8_t(rhs & 0b11110000);
    }

    inline bool Msgpack::IsFixArrayFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixArray) == std::int8_t(rhs & 0b11110000);
    }

    inline bool Msgpack::IsFixStrFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixStr) == std::int8_t(rhs & 0b11100000);
    }

    inline bool Msgpack::IsNegativeFixIntFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kNegativeFixInt) == std::int8_t(rhs & 0b11100000);
    }

}

