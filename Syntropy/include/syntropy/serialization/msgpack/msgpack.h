
/// \file msgpack.h
/// \brief This header is part of the syntropy serialization system. It contains base definition for the msgpack specification.
///
/// Specification found here: https://github.com/msgpack/msgpack/blob/master/spec.md
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>
#include <limits>
#include "syntropy/memory/bytes.h"

#include "syntropy/platform/endianness.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK FORMAT                                                       */
    /************************************************************************/

    /// \brief Type formats supported by msgpack.
    /// \author Raffaele D. Facendola - November 2019.
    enum class MsgpackFormat : std::uint8_t
    {
        /// \brief 7-bit positive integer value.
        kPositiveFixInt = 0x00u,

        /// \brief Map whose length is up to 15 elements.
        kFixMap = 0x80u,

        /// \brief Array whose length is up to 15 elements.
        kFixArray = 0x90u,

        /// \brief Byte array whose length is up to 31 bytes.
        kFixStr = 0xA0u,

        /// \brief Null value.
        kNil = 0xC0u,

        /// \brief This value is never used.
        kUnused = 0xC1u,

        /// \brief False boolean value.
        kFalse = 0xC2u,

        /// \brief True boolean value.
        kTrue = 0xC3u,

        /// \brief Byte array whose length is up to (2^8) - 1 Bytes.
        kBin8 = 0xC4u,

        /// \brief Byte array whose length is up to (2^16) - 1 Bytes.
        kBin16 = 0xC5u,

        /// \brief Byte array whose length is up to (2^32) - 1 Bytes.
        kBin32 = 0xC6u,

        /// \brief Integer and a byte array whose length is up to (2^8) - 1 Bytes.
        kExt8 = 0xC7u,

        /// \brief Integer and a byte array whose length is up to (2^16) - 1 Bytes.
        kExt16 = 0xC8u,

        /// \brief Integer and a byte array whose length is up to (2^32) - 1 Bytes.
        kExt32 = 0xC9u,

        /// \brief Big-endian floating point number in IEEE 754 single precision floating point value.
        kFloat32 = 0xCAu,

        /// \brief Big-endian floating point number in IEEE 754 double precision floating point value.
        kFloat64 = 0xCBu,

        /// \brief 8-bit unsigned integer value.
        kUInt8 = 0xCCu,

        /// \brief 16-bit big-endian unsigned integer value.
        kUInt16 = 0xCDu,

        /// \brief 32-bit big-endian unsigned integer value.
        kUInt32 = 0xCEu,

        /// \brief 64-bit big-endian unsigned integer value.
        kUInt64 = 0xCFu,

        /// \brief 8-bit signed integer value.
        kInt8 = 0xD0u,

        /// \brief 16-bit big-endian signed integer value.
        kInt16 = 0xD1u,

        /// \brief 32-bit big-endian signed integer value.
        kInt32 = 0xD2u,

        /// \brief 64-bit big-endian signed integer value.
        kInt64 = 0xD3u,

        /// \brief Integer and a byte array whose length is 1 Byte.
        kFixExt1 = 0xD4u,

        /// \brief Integer and a byte array whose length is 2 Bytes.
        kFixExt2 = 0xD5u,

        /// \brief Integer and a byte array whose length is 4 Bytes.
        kFixExt4 = 0xD6u,

        /// \brief Integer and a byte array whose length is 8 Bytes.
        kFixExt8 = 0xD7u,

        /// \brief Integer and a byte array whose length is 16 Bytes.
        kFixExt16 = 0xD8u,

        /// \brief Byte array whose length is up to (2^8) - 1 Bytes.
        kStr8 = 0xD9u,

        /// \brief Byte array whose length is up to (2^16) - 1 Bytes.
        kStr16 = 0xDAu,

        /// \brief Byte array whose length is up to (2^32) - 1 Bytes.
        kStr32 = 0xDBu,

        /// \brief Array whose length is up to (2^16) - 1 elements.
        kArray16 = 0xDCu,

        /// \brief Array whose length is up to (2^32) - 1 elements.
        kArray32 = 0xDDu,

        /// \brief Map whose length is up to (2^16) - 1 elements.
        kMap16 = 0xDEu,

        /// \brief Map whose length is up to (2^32) - 1 elements.
        kMap32 = 0xDFu,

        /// \brief 5-bit negative integer value.
        kNegativeFixInt = 0xE0u
    };

    /************************************************************************/
    /* MSGPACK EXTENSION TYPE                                               */
    /************************************************************************/

    /// \brief Exposes methods used to handle extension types encoded using msgpack specification.
    /// \author Raffaele D. Facendola - November 2019.
    template <typename TType>
    struct MsgpackExtensionType {};

    /************************************************************************/
    /* MSGPACK                                                              */
    /************************************************************************/

    /// \brief Defines methods used to encode and decode data using the msgpack specification.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Msgpack
    {
        /// \brief Check whether rhs can be encoded using a positive fix int.
        bool IsPositiveFixInt(std::int8_t rhs);

        /// \brief Check whether rhs can be encoded using a negative fix int.
        bool IsNegativeFixInt(std::int8_t rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit signed int.
        bool IsInt8(std::int64_t rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit signed int.
        bool IsInt16(std::int64_t rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit signed int.
        bool IsInt32(std::int64_t rhs);

        /// \brief Check whether rhs can be encoded using a positive fix int.
        bool IsPositiveFixInt(std::uint8_t rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit unsigned int.
        bool IsUInt8(std::uint64_t rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit unsigned int.
        bool IsUInt16(std::uint64_t rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit unsigned int.
        bool IsUInt32(std::uint64_t rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length string.
        bool IsFixStr(const std::string& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit long string.
        bool IsStr8(const std::string& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long string.
        bool IsStr16(const std::string& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long string.
        bool IsStr32(const std::string& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit long byte-array.
        bool IsBin8(const std::vector<std::int8_t>& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long byte-array.
        bool IsBin16(const std::vector<std::int8_t>& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long byte-array.
        bool IsBin32(const std::vector<std::int8_t>& rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length array.
        template <typename TArray>
        bool IsFixArray(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long array.
        template <typename TArray>
        bool IsArray16(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long array.
        template <typename TArray>
        bool IsArray32(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length map.
        template <typename TMap>
        bool IsFixMap(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long map.
        template <typename TMap>
        bool IsMap16(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long map.
        template <typename TMap>
        bool IsMap32(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a 1-byte fixed extension type.
        template <typename TExtension>
        bool IsFixExt1(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 2-bytes fixed extension type.
        template <typename TExtension>
        bool IsFixExt2(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 4-bytes fixed extension type.
        template <typename TExtension>
        bool IsFixExt4(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bytes fixed extension type.
        template <typename TExtension>
        bool IsFixExt8(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bytes fixed extension type.
        template <typename TExtension>
        bool IsFixExt16(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^8)-1 bytes.
        template <typename TExtension>
        bool IsExt8(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^16)-1 bytes.
        template <typename TExtension>
        bool IsExt16(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^32)-1 bytes.
        template <typename TExtension>
        bool IsExt32(const TExtension& rhs);

        /// \brief Encode a null value.
        std::int8_t Encode(std::nullptr_t rhs);

        /// \brief Encode a boolean value.
        std::int8_t Encode(bool rhs);

        /// \brief Encode a positive fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the positive fix int specification.
        std::int8_t EncodePositiveFixInt(std::int8_t rhs);

        /// \brief Encode a negative fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the negative fix int specification.
        std::int8_t EncodeNegativeFixInt(std::int8_t rhs);

        /// \brief Encode a 8-bit signed int.
        std::int8_t Encode(std::int8_t rhs);

        /// \brief Encode a 16-bit signed int.
        std::int16_t Encode(std::int16_t rhs);

        /// \brief Encode a 32-bit signed int.
        std::int32_t Encode(std::int32_t rhs);

        /// \brief Encode a 64-bit signed int.
        std::int64_t Encode(std::int64_t rhs);

        /// \brief Encode a positive fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the positive fix int specification.
        std::int8_t EncodePositiveFixInt(std::uint8_t rhs);

        /// \brief Encode a 8-bit unsigned int.
        std::int8_t Encode(std::uint8_t rhs);

        /// \brief Encode a 16-bit unsigned int.
        std::int16_t Encode(std::uint16_t rhs);

        /// \brief Encode a 32-bit unsigned int.
        std::int32_t Encode(std::uint32_t rhs);

        /// \brief Encode a 64-bit unsigned int.
        std::int64_t Encode(std::uint64_t rhs);

        /// \brief Encode a 32-bit floating point number.
        std::int32_t Encode(float rhs);

        /// \brief Encode a 64-bit floating point number.
        std::int64_t Encode(double rhs);

        /// \brief Encode the length of a fixed-length string.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length string specification.
        std::int8_t EncodeFixStrLength(std::uint8_t rhs);

        /// \brief Encode the length of a fixed-length array.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length array specification.
        std::int8_t EncodeFixArrayLength(std::uint8_t rhs);

        /// \brief Encode the length of a fixed-length map.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length map specification.
        std::int8_t EncodeFixMapLength(std::uint8_t rhs);

        /// \brief Check whether rhs represents a positive fix int type format.
        bool IsPositiveFixIntFormat(std::int8_t rhs);

        /// \brief Check whether rhs represents a negative fix int type format.
        bool IsNegativeFixIntFormat(std::int8_t rhs);

        /// \brief Check whether rhs represents a fixed-length string type format.
        bool IsFixStrFormat(std::int8_t rhs);

        /// \brief Check whether rhs represents a fixed-length array type format.
        bool IsFixArrayFormat(std::int8_t rhs);

        /// \brief Check whether rhs represents a fixed-length map type format.
        bool IsFixMapFormat(std::int8_t rhs);

        /// \brief Decode a positive fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the positive fix int specification.
        std::int8_t DecodePositiveFixInt(std::int8_t rhs);

        /// \brief Decode a negative fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the negative fix int specification.
        std::int8_t DecodeNegativeFixInt(std::int8_t rhs);

        /// \brief Decode a 8-bit signed int.
        std::int8_t DecodeInt8(std::int8_t rhs);

        /// \brief Decode a 16-bit signed int.
        std::int16_t DecodeInt16(std::int16_t rhs);

        /// \brief Decode a 32-bit signed int.
        std::int32_t DecodeInt32(std::int32_t rhs);

        /// \brief Decode a 64-bit signed int.
        std::int64_t DecodeInt64(std::int64_t rhs);

        /// \brief Decode a positive fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the positive fix int specification.
        std::uint8_t DecodePositiveFixUInt(std::int8_t rhs);

        /// \brief Decode a 8-bit unsigned int.
        std::uint8_t DecodeUInt8(std::int8_t rhs);

        /// \brief Decode a 16-bit unsigned int.
        std::uint16_t DecodeUInt16(std::int16_t rhs);

        /// \brief Decode a 32-bit unsigned int.
        std::uint32_t DecodeUInt32(std::int32_t rhs);

        /// \brief Decode a 64-bit unsigned int.
        std::uint64_t DecodeUInt64(std::int64_t rhs);

        /// \brief Decode a 32-bit floating point number.
        float DecodeFloat(std::int32_t rhs);

        /// \brief Decode a 64-bit floating point number.
        double DecodeDouble(std::int64_t rhs);

        /// \brief Decode the length of a fixed-length string.
        std::uint8_t DecodeFixStrLength(std::int8_t rhs);

        /// \brief Decode the length of a fixed-length array.
        std::uint8_t DecodeFixArrayLength(std::int8_t rhs);

        /// \brief Decode the length of a fixed-length map.
        std::uint8_t DecodeFixMapLength(std::int8_t rhs);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Msgpack.

    inline bool Msgpack::IsPositiveFixInt(std::int8_t rhs)
    {
        return rhs >= 0 && rhs <= 127;
    }

    inline bool Msgpack::IsNegativeFixInt(std::int8_t rhs)
    {
        return rhs >= -63 && rhs <= 0;
    }

    inline bool Msgpack::IsInt8(std::int64_t rhs)
    {
        return rhs >= std::numeric_limits<std::int8_t>::min() && rhs <= std::numeric_limits<std::int8_t>::max();
    }

    inline bool Msgpack::IsInt16(std::int64_t rhs)
    {
        return rhs >= std::numeric_limits<std::int16_t>::min() && rhs <= std::numeric_limits<std::int16_t>::max();
    }

    inline bool Msgpack::IsInt32(std::int64_t rhs)
    {
        return rhs >= std::numeric_limits<std::int32_t>::min() && rhs <= std::numeric_limits<std::int32_t>::max();
    }

    inline bool Msgpack::IsPositiveFixInt(std::uint8_t rhs)
    {
        return rhs >= 0u && rhs <= 127u;
    }

    inline bool Msgpack::IsUInt8(std::uint64_t rhs)
    {
        return rhs >= std::numeric_limits<std::uint8_t>::min() && rhs <= std::numeric_limits<std::uint8_t>::max();
    }

    inline bool Msgpack::IsUInt16(std::uint64_t rhs)
    {
        return rhs >= std::numeric_limits<std::uint16_t>::min() && rhs <= std::numeric_limits<std::uint16_t>::max();
    }

    inline bool Msgpack::IsUInt32(std::uint64_t rhs)
    {
        return rhs >= std::numeric_limits<std::uint32_t>::min() && rhs <= std::numeric_limits<std::uint32_t>::max();
    }

    inline bool Msgpack::IsFixStr(const std::string& rhs)
    {
        return rhs.size() <= 0b11111;
    }

    inline bool Msgpack::IsStr8(const std::string& rhs)
    {
        return rhs.size() <= 0xFF;
    }

    inline bool Msgpack::IsStr16(const std::string& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    inline bool Msgpack::IsStr32(const std::string& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
    }

    inline bool Msgpack::IsBin8(const std::vector<std::int8_t>& rhs)
    {
        return rhs.size() <= 0xF;
    }

    inline bool Msgpack::IsBin16(const std::vector<std::int8_t>& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    inline bool Msgpack::IsBin32(const std::vector<std::int8_t>& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
    }

    template <typename TArray>
    inline bool Msgpack::IsFixArray(const TArray& rhs)
    {
        return rhs.size() <= 0xF;
    }

    template <typename TArray>
    inline bool Msgpack::IsArray16(const TArray& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    template <typename TArray>
    inline bool Msgpack::IsArray32(const TArray& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
    }

    template <typename TMap>
    inline bool Msgpack::IsFixMap(const TMap& rhs)
    {
        return rhs.size() <= 0xF;
    }

    template <typename TMap>
    inline bool Msgpack::IsMap16(const TMap& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    template <typename TMap>
    inline bool Msgpack::IsMap32(const TMap& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
    }

    template <typename TExtension>
    inline bool Msgpack::IsFixExt1(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 1_Bytes;
    }

    template <typename TExtension>
    inline bool Msgpack::IsFixExt2(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 2_Bytes;
    }

    template <typename TExtension>
    inline bool Msgpack::IsFixExt4(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 4_Bytes;
    }

    template <typename TExtension>
    inline bool Msgpack::IsFixExt8(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 8_Bytes;
    }

    template <typename TExtension>
    inline bool Msgpack::IsFixExt16(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 16_Bytes;
    }

    template <typename TExtension>
    inline bool Msgpack::IsExt8(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFF_Bytes;
    }

    template <typename TExtension>
    inline bool Msgpack::IsExt16(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFFFF_Bytes;
    }

    template <typename TExtension>
    inline bool Msgpack::IsExt32(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFFFFFFFF_Bytes;
    }

    inline std::int8_t Msgpack::Encode(std::nullptr_t rhs)
    {
        return std::int8_t(MsgpackFormat::kNil);
    }

    inline std::int8_t Msgpack::Encode(bool rhs)
    {
        return std::int8_t(rhs ? MsgpackFormat::kTrue : MsgpackFormat::kFalse);
    }

    inline std::int8_t Msgpack::EncodePositiveFixInt(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kPositiveFixInt) | ((+rhs) & 0b01111111);
    }

    inline std::int8_t Msgpack::EncodeNegativeFixInt(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kNegativeFixInt) | ((-rhs) & 0b00011111);
    }

    inline std::int8_t Msgpack::Encode(std::int8_t rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline std::int16_t Msgpack::Encode(std::int16_t rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline std::int32_t Msgpack::Encode(std::int32_t rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline std::int64_t Msgpack::Encode(std::int64_t rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline std::int8_t Msgpack::EncodePositiveFixInt(std::uint8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kPositiveFixInt) | (rhs & 0b01111111);

    }

    inline std::int8_t Msgpack::Encode(std::uint8_t rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline std::int16_t Msgpack::Encode(std::uint16_t rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline std::int32_t Msgpack::Encode(std::uint32_t rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline std::int64_t Msgpack::Encode(std::uint64_t rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline std::int32_t Msgpack::Encode(float rhs)
    {
        auto bytes = std::int32_t{};

        std::memcpy(&bytes, &rhs, sizeof(float));

        return Endianness::ToBigEndian(bytes);
    }

    inline std::int64_t Msgpack::Encode(double rhs)
    {
        auto bytes = std::int64_t{};

        std::memcpy(&bytes, &rhs, sizeof(double));

        return Endianness::ToBigEndian(bytes);
    }

    inline std::int8_t Msgpack::EncodeFixStrLength(std::uint8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixStr) | (std::int8_t(rhs) & 0b00011111);
    }

    inline std::int8_t Msgpack::EncodeFixArrayLength(std::uint8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixArray) | (std::int8_t(rhs) & 0b00001111);
    }

    inline std::int8_t Msgpack::EncodeFixMapLength(std::uint8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixMap) | (std::int8_t(rhs) & 0b00001111);
    }

    inline bool Msgpack::IsPositiveFixIntFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kPositiveFixInt) == std::int8_t(rhs & 0b10000000);
    }

    inline bool Msgpack::IsNegativeFixIntFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kNegativeFixInt) == std::int8_t(rhs & 0b11100000);
    }

    inline bool Msgpack::IsFixStrFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixStr) == std::int8_t(rhs & 0b11100000);
    }

    inline bool Msgpack::IsFixArrayFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixArray) == std::int8_t(rhs & 0b11110000);
    }

    inline bool Msgpack::IsFixMapFormat(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixMap) == std::int8_t(rhs & 0b11110000);
    }

    inline std::int8_t Msgpack::DecodePositiveFixInt(std::int8_t rhs)
    {
        return rhs & 0b01111111;
    }

    inline std::int8_t Msgpack::DecodeNegativeFixInt(std::int8_t rhs)
    {
        return -(rhs & 0b00011111);
    }

    inline std::int8_t Msgpack::DecodeInt8(std::int8_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    inline std::int16_t Msgpack::DecodeInt16(std::int16_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    inline std::int32_t Msgpack::DecodeInt32(std::int32_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    inline std::int64_t Msgpack::DecodeInt64(std::int64_t rhs)
    {
        return Endianness::FromBigEndian(rhs);
    }

    inline std::uint8_t Msgpack::DecodePositiveFixUInt(std::int8_t rhs)
    {
        return std::uint8_t(rhs & 0b01111111);
    }

    inline std::uint8_t Msgpack::DecodeUInt8(std::int8_t rhs)
    {
        return Endianness::FromBigEndian(std::uint8_t(rhs));
    }

    inline std::uint16_t Msgpack::DecodeUInt16(std::int16_t rhs)
    {
        return Endianness::FromBigEndian(std::uint16_t(rhs));
    }

    inline std::uint32_t Msgpack::DecodeUInt32(std::int32_t rhs)
    {
        return Endianness::FromBigEndian(std::uint32_t(rhs));
    }

    inline std::uint64_t Msgpack::DecodeUInt64(std::int64_t rhs)
    {
        return Endianness::FromBigEndian(std::uint64_t(rhs));
    }

    inline float Msgpack::DecodeFloat(std::int32_t rhs)
    {
        auto value = float{};

        rhs = Endianness::FromBigEndian(rhs);

        std::memcpy(&value, &rhs, sizeof(float));

        return value;
    }

    inline double Msgpack::DecodeDouble(std::int64_t rhs)
    {
        auto value = double{};

        rhs = Endianness::FromBigEndian(rhs);

        std::memcpy(&value, &rhs, sizeof(double));

        return value;
    }

    inline std::uint8_t Msgpack::DecodeFixStrLength(std::int8_t rhs)
    {
        return std::uint8_t(rhs & 0b00011111);
    }

    inline std::uint8_t Msgpack::DecodeFixArrayLength(std::int8_t rhs)
    {
        return std::uint8_t(rhs & 0b00001111);
    }

    inline std::uint8_t Msgpack::DecodeFixMapLength(std::int8_t rhs)
    {
        return std::uint8_t(rhs & 0b00001111);
    }
}

