
/// \file msgpack.h
/// \brief This header is part of the Syntropy serialization module. It contains classes for the Msgpack decoding.
///
/// Specification found here: https://github.com/msgpack/msgpack/blob/master/spec.md
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/core/types.h"
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
        Byte DecodePositiveFixUInt(Byte rhs);

        /// \brief Decode a positive fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the positive fix int specification.
        Byte DecodePositiveFixInt(Byte rhs);

        /// \brief Decode the length of a fixed-length map.
        Byte DecodeFixMapLength(Byte rhs);

        /// \brief Decode the length of a fixed-length array.
        Byte DecodeFixArrayLength(Byte rhs);

        /// \brief Decode the length of a fixed-length string.
        Byte DecodeFixStrLength(Byte rhs);

        /// \brief Decode an 8-bit value.
        template <typename TDecoded>
        TDecoded Decode(Byte rhs);

        /// \brief Decode a 16-bit value.
        template <typename TDecoded>
        TDecoded Decode(std::int16_t rhs);

        /// \brief Decode a 32-bit value.
        template <typename TDecoded>
        TDecoded Decode(std::int32_t rhs);

        /// \brief Decode a 64-bit value.
        template <typename TDecoded>
        TDecoded Decode(Int rhs);

        /// \brief Decode a negative fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the negative fix int specification.
        Byte DecodeNegativeFixInt(Byte rhs);
    }

    /************************************************************************/
    /* MSGPACK                                                              */
    /************************************************************************/

    /// \brief Define methods used to check encoded format types during decoding.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Msgpack
    {
        /// \brief Check whether rhs represents a positive fix int type format.
        Bool IsPositiveFixIntFormat(Byte rhs);

        /// \brief Check whether rhs represents a fixed-length map type format.
        Bool IsFixMapFormat(Byte rhs);

        /// \brief Check whether rhs represents a fixed-length array type format.
        Bool IsFixArrayFormat(Byte rhs);

        /// \brief Check whether rhs represents a fixed-length string type format.
        Bool IsFixStrFormat(Byte rhs);

        /// \brief Check whether rhs represents a negative fix int type format.
        Bool IsNegativeFixIntFormat(Byte rhs);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Msgpack.

    inline Byte Msgpack::DecodePositiveFixUInt(Byte rhs)
    {
        return rhs & 0b01111111;
    }

    inline Byte Msgpack::DecodePositiveFixInt(Byte rhs)
    {
        return rhs & 0b01111111;
    }

    inline Byte Msgpack::DecodeFixMapLength(Byte rhs)
    {
        return rhs & 0b00001111;
    }

    inline Byte Msgpack::DecodeFixArrayLength(Byte rhs)
    {
        return rhs & 0b00001111;
    }

    inline Byte Msgpack::DecodeFixStrLength(Byte rhs)
    {
        return rhs & 0b00011111;
    }

    template <>
    inline Byte Msgpack::Decode<Byte>(Byte rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    template <>
    inline std::int16_t Msgpack::Decode<std::int16_t>(std::int16_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    template <>
    inline Float Msgpack::Decode<Float>(std::int32_t rhs)
    {
        auto value = Float{};

        rhs = Endianness::FromBigEndian(rhs);

        std::memcpy(&value, &rhs, sizeof(Float));

        return value;
    }

    template <>
    inline std::int32_t Msgpack::Decode<std::int32_t>(std::int32_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    template <>
    inline double Msgpack::Decode<double>(Int rhs)
    {
        auto value = double{};

        rhs = Endianness::FromBigEndian(rhs);

        std::memcpy(&value, &rhs, sizeof(double));

        return value;
    }

    template <>
    inline Int Msgpack::Decode<Int>(Int rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    inline Byte Msgpack::DecodeNegativeFixInt(Byte rhs)
    {
        return -(rhs & 0b00011111);
    }

    // Msgpack.

    inline Bool Msgpack::IsPositiveFixIntFormat(Byte rhs)
    {
        return Byte(MsgpackFormat::kPositiveFixInt) == Byte(rhs & 0b10000000);
    }

    inline Bool Msgpack::IsFixMapFormat(Byte rhs)
    {
        return Byte(MsgpackFormat::kFixMap) == Byte(rhs & 0b11110000);
    }

    inline Bool Msgpack::IsFixArrayFormat(Byte rhs)
    {
        return Byte(MsgpackFormat::kFixArray) == Byte(rhs & 0b11110000);
    }

    inline Bool Msgpack::IsFixStrFormat(Byte rhs)
    {
        return Byte(MsgpackFormat::kFixStr) == Byte(rhs & 0b11100000);
    }

    inline Bool Msgpack::IsNegativeFixIntFormat(Byte rhs)
    {
        return Byte(MsgpackFormat::kNegativeFixInt) == Byte(rhs & 0b11100000);
    }

}

