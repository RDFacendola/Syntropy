
/// \file encoding.h
/// \brief This header is part of the Syntropy serialization module. It contains classes for the Msgpack encoding.
///
/// Specification found here: https://github.com/msgpack/msgpack/blob/master/spec.md
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>
#include <limits>

#include "syntropy/serialization/msgpack/format.h"
#include "syntropy/platform/endianness.h"
#include "syntropy/language/type_traits.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK                                                              */
    /************************************************************************/

    /// \brief Define methods used to encode values using the Msgpack specification.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Msgpack
    {
        /// \brief Encode a positive fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the positive fix int specification.
        std::int8_t EncodePositiveFixInt(std::int8_t rhs);

        /// \brief Encode the length of a fixed-length map.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length map specification.
        std::int8_t EncodeFixMapLength(std::int8_t rhs);

        /// \brief Encode the length of a fixed-length array.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length array specification.
        std::int8_t EncodeFixArrayLength(std::int8_t rhs);

        /// \brief Encode the length of a fixed-length string.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length string specification.
        std::int8_t EncodeFixStrLength(std::int8_t rhs);

        /// \brief Encode a null value.
        std::int8_t Encode(std::nullptr_t rhs);

        /// \brief Encode a boolean value.
        std::int8_t Encode(bool rhs);

        /// \brief Encode a 32-bit floating point number.
        std::int32_t Encode(float rhs);

        /// \brief Encode a 64-bit floating point number.
        std::int64_t Encode(double rhs);

        /// \brief Encode a 8-bit signed int.
        std::int8_t Encode(std::int8_t rhs);

        /// \brief Encode a 16-bit signed int.
        std::int16_t Encode(std::int16_t rhs);

        /// \brief Encode a 32-bit signed int.
        std::int32_t Encode(std::int32_t rhs);

        /// \brief Encode a 64-bit signed int.
        std::int64_t Encode(std::int64_t rhs);

        /// \brief Encode a negative fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the negative fix int specification.
        std::int8_t EncodeNegativeFixInt(std::int8_t rhs);
    }

    /************************************************************************/
    /* MSGPACK                                                              */
    /************************************************************************/

    /// \brief Define methods used to check the encoded format of values during encoding.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Msgpack
    {
        /// \brief Check whether rhs can be encoded using a positive fix int.
        bool IsPositiveFixInt(std::int8_t rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length map.
        template <typename TMap>
        bool IsFixMap(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length array.
        template <typename TArray>
        bool IsFixArray(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length string.
        bool IsFixStr(const String& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit long byte-array.
        bool IsBin8(const ConstMemoryRange& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long byte-array.
        bool IsBin16(const ConstMemoryRange& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long byte-array.
        bool IsBin32(const ConstMemoryRange& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^8)-1 bytes.
        template <typename TExtension>
        bool IsExt8(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^16)-1 bytes.
        template <typename TExtension>
        bool IsExt16(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^32)-1 bytes.
        template <typename TExtension>
        bool IsExt32(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit signed int.
        bool IsInt8(std::int64_t rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit signed int.
        bool IsInt16(std::int64_t rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit signed int.
        bool IsInt32(std::int64_t rhs);

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

        /// \brief Check whether rhs can be encoded using a 8-bit long string.
        bool IsStr8(const String& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long string.
        bool IsStr16(const String& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long string.
        bool IsStr32(const String& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long array.
        template <typename TArray>
        bool IsArray16(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long array.
        template <typename TArray>
        bool IsArray32(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long map.
        template <typename TMap>
        bool IsMap16(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long map.
        template <typename TMap>
        bool IsMap32(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a negative fix int.
        bool IsNegativeFixInt(std::int8_t rhs);
    }

    /************************************************************************/
    /* MSGPACK SELECT ENCODED TYPE                                          */
    /************************************************************************/

    /// \brief Trait used to associate each type with an encoded data type.
    /// \author Raffaele D. Facendola - December 2019.
    template <typename TType>
    using MsgpackSelectEncodedTypeT = ConditionalT<sizeof(TType) == sizeof(std::int8_t), std::int8_t,
        ConditionalT<sizeof(TType) == sizeof(std::int16_t), std::int16_t,
        ConditionalT<sizeof(TType) == sizeof(std::int32_t), std::int32_t,
        ConditionalT<sizeof(TType) == sizeof(std::int64_t), std::int64_t,
        void>>>>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

     // Msgpack

    inline std::int8_t Msgpack::EncodePositiveFixInt(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kPositiveFixInt) | (rhs & 0b01111111);
    }

    inline std::int8_t Msgpack::EncodeFixMapLength(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixMap) | (rhs & 0b00001111);
    }

    inline std::int8_t Msgpack::EncodeFixArrayLength(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixArray) | (rhs & 0b00001111);
    }

    inline std::int8_t Msgpack::EncodeFixStrLength(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kFixStr) | (rhs & 0b00011111);
    }

    inline std::int8_t Msgpack::Encode(std::nullptr_t rhs)
    {
        return std::int8_t(MsgpackFormat::kNil);
    }

    inline std::int8_t Msgpack::Encode(bool rhs)
    {
        return std::int8_t(rhs ? MsgpackFormat::kTrue : MsgpackFormat::kFalse);
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

    inline std::int8_t Msgpack::EncodeNegativeFixInt(std::int8_t rhs)
    {
        return std::int8_t(MsgpackFormat::kNegativeFixInt) | ((-rhs) & 0b00011111);
    }

    // Msgpack.

    inline bool Msgpack::IsPositiveFixInt(std::int8_t rhs)
    {
        return rhs >= 0 && rhs <= 127;
    }

    template <typename TMap>
    inline bool Msgpack::IsFixMap(const TMap& rhs)
    {
        return rhs.size() <= 0xF;
    }

    template <typename TArray>
    inline bool Msgpack::IsFixArray(const TArray& rhs)
    {
        return rhs.size() <= 0xF;
    }

    inline bool Msgpack::IsFixStr(const String& rhs)
    {
        return rhs.size() <= 0b11111;
    }

    inline bool Msgpack::IsBin8(const ConstMemoryRange& rhs)
    {
        return *rhs.GetSize() <= 0xF;
    }

    inline bool Msgpack::IsBin16(const ConstMemoryRange& rhs)
    {
        return *rhs.GetSize() <= 0xFFFF;
    }

    inline bool Msgpack::IsBin32(const ConstMemoryRange& rhs)
    {
        return *rhs.GetSize() <= 0xFFFFFFFF;
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

    inline bool Msgpack::IsStr8(const String& rhs)
    {
        return rhs.size() <= 0xFF;
    }

    inline bool Msgpack::IsStr16(const String& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    inline bool Msgpack::IsStr32(const String& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
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
    inline bool Msgpack::IsMap16(const TMap& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    template <typename TMap>
    inline bool Msgpack::IsMap32(const TMap& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
    }

    inline bool Msgpack::IsNegativeFixInt(std::int8_t rhs)
    {
        return rhs >= -63 && rhs <= 0;
    }

}