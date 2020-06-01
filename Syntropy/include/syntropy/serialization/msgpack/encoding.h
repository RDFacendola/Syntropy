
/// \file encoding.h
/// \brief This header is part of the Syntropy serialization module. It contains classes for the Msgpack encoding.
///
/// Specification found here: https://github.com/msgpack/msgpack/blob/master/spec.md
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <limits>

#include "syntropy/core/types.h"
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
        Byte EncodePositiveFixInt(Byte rhs);

        /// \brief Encode the length of a fixed-length map.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length map specification.
        Byte EncodeFixMapLength(Byte rhs);

        /// \brief Encode the length of a fixed-length array.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length array specification.
        Byte EncodeFixArrayLength(Byte rhs);

        /// \brief Encode the length of a fixed-length string.
        /// The behavior of this method is undefined if rhs is incompatible with the fixed-length string specification.
        Byte EncodeFixStrLength(Byte rhs);

        /// \brief Encode a null value.
        Byte Encode(std::nullptr_t rhs);

        /// \brief Encode a boolean value.
        Byte Encode(Bool rhs);

        /// \brief Encode a 32-bit floating point number.
        std::int32_t Encode(Float rhs);

        /// \brief Encode a 64-bit floating point number.
        Int Encode(double rhs);

        /// \brief Encode a 8-bit signed int.
        Byte Encode(Byte rhs);

        /// \brief Encode a 16-bit signed int.
        std::int16_t Encode(std::int16_t rhs);

        /// \brief Encode a 32-bit signed int.
        std::int32_t Encode(std::int32_t rhs);

        /// \brief Encode a 64-bit signed int.
        Int Encode(Int rhs);

        /// \brief Encode a negative fix int.
        /// The behavior of this method is undefined if rhs is incompatible with the negative fix int specification.
        Byte EncodeNegativeFixInt(Byte rhs);
    }

    /************************************************************************/
    /* MSGPACK                                                              */
    /************************************************************************/

    /// \brief Define methods used to check the encoded format of values during encoding.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Msgpack
    {
        /// \brief Check whether rhs can be encoded using a positive fix int.
        Bool IsPositiveFixInt(Byte rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length map.
        template <typename TMap>
        Bool IsFixMap(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length array.
        template <typename TArray>
        Bool IsFixArray(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a fixed-length string.
        Bool IsFixStr(const String& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit long byte-array.
        Bool IsBin8(const ConstMemoryRange& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long byte-array.
        Bool IsBin16(const ConstMemoryRange& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long byte-array.
        Bool IsBin32(const ConstMemoryRange& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^8)-1 bytes.
        template <typename TExtension>
        Bool IsExt8(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^16)-1 bytes.
        template <typename TExtension>
        Bool IsExt16(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using an extension type whose size is up to (2^32)-1 bytes.
        template <typename TExtension>
        Bool IsExt32(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit signed int.
        Bool IsInt8(Int rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit signed int.
        Bool IsInt16(Int rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit signed int.
        Bool IsInt32(Int rhs);

        /// \brief Check whether rhs can be encoded using a 1-byte fixed extension type.
        template <typename TExtension>
        Bool IsFixExt1(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 2-bytes fixed extension type.
        template <typename TExtension>
        Bool IsFixExt2(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 4-bytes fixed extension type.
        template <typename TExtension>
        Bool IsFixExt4(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bytes fixed extension type.
        template <typename TExtension>
        Bool IsFixExt8(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bytes fixed extension type.
        template <typename TExtension>
        Bool IsFixExt16(const TExtension& rhs);

        /// \brief Check whether rhs can be encoded using a 8-bit long string.
        Bool IsStr8(const String& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long string.
        Bool IsStr16(const String& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long string.
        Bool IsStr32(const String& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long array.
        template <typename TArray>
        Bool IsArray16(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long array.
        template <typename TArray>
        Bool IsArray32(const TArray& rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit long map.
        template <typename TMap>
        Bool IsMap16(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit long map.
        template <typename TMap>
        Bool IsMap32(const TMap& rhs);

        /// \brief Check whether rhs can be encoded using a negative fix int.
        Bool IsNegativeFixInt(Byte rhs);
    }

    /************************************************************************/
    /* MSGPACK SELECT ENCODED TYPE                                          */
    /************************************************************************/

    /// \brief Trait used to associate each type with an encoded data type.
    /// \author Raffaele D. Facendola - December 2019.
    template <typename TType>
    using MsgpackSelectEncodedTypeT = ConditionalT<sizeof(TType) == sizeof(Byte), Byte,
        ConditionalT<sizeof(TType) == sizeof(std::int16_t), std::int16_t,
        ConditionalT<sizeof(TType) == sizeof(std::int32_t), std::int32_t,
        ConditionalT<sizeof(TType) == sizeof(Int), Int,
        void>>>>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

     // Msgpack

    inline Byte Msgpack::EncodePositiveFixInt(Byte rhs)
    {
        return Byte(MsgpackFormat::kPositiveFixInt) | (rhs & 0b01111111);
    }

    inline Byte Msgpack::EncodeFixMapLength(Byte rhs)
    {
        return Byte(MsgpackFormat::kFixMap) | (rhs & 0b00001111);
    }

    inline Byte Msgpack::EncodeFixArrayLength(Byte rhs)
    {
        return Byte(MsgpackFormat::kFixArray) | (rhs & 0b00001111);
    }

    inline Byte Msgpack::EncodeFixStrLength(Byte rhs)
    {
        return Byte(MsgpackFormat::kFixStr) | (rhs & 0b00011111);
    }

    inline Byte Msgpack::Encode(std::nullptr_t rhs)
    {
        return Byte(MsgpackFormat::kNil);
    }

    inline Byte Msgpack::Encode(Bool rhs)
    {
        return Byte(rhs ? MsgpackFormat::kTrue : MsgpackFormat::kFalse);
    }

    inline std::int32_t Msgpack::Encode(Float rhs)
    {
        auto bytes = std::int32_t{};

        std::memcpy(&bytes, &rhs, sizeof(Float));

        return Endianness::ToBigEndian(bytes);
    }

    inline Int Msgpack::Encode(double rhs)
    {
        auto bytes = Int{};

        std::memcpy(&bytes, &rhs, sizeof(double));

        return Endianness::ToBigEndian(bytes);
    }

    inline Byte Msgpack::Encode(Byte rhs)
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

    inline Int Msgpack::Encode(Int rhs)
    {
        return Endianness::ToBigEndian(rhs);
    }

    inline Byte Msgpack::EncodeNegativeFixInt(Byte rhs)
    {
        return Byte(MsgpackFormat::kNegativeFixInt) | ((-rhs) & 0b00011111);
    }

    // Msgpack.

    inline Bool Msgpack::IsPositiveFixInt(Byte rhs)
    {
        return rhs >= 0 && rhs <= 127;
    }

    template <typename TMap>
    inline Bool Msgpack::IsFixMap(const TMap& rhs)
    {
        return rhs.size() <= 0xF;
    }

    template <typename TArray>
    inline Bool Msgpack::IsFixArray(const TArray& rhs)
    {
        return rhs.size() <= 0xF;
    }

    inline Bool Msgpack::IsFixStr(const String& rhs)
    {
        return rhs.size() <= 0b11111;
    }

    inline Bool Msgpack::IsBin8(const ConstMemoryRange& rhs)
    {
        return *rhs.GetSize() <= 0xF;
    }

    inline Bool Msgpack::IsBin16(const ConstMemoryRange& rhs)
    {
        return *rhs.GetSize() <= 0xFFFF;
    }

    inline Bool Msgpack::IsBin32(const ConstMemoryRange& rhs)
    {
        return *rhs.GetSize() <= 0xFFFFFFFF;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsExt8(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFF_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsExt16(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFFFF_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsExt32(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFFFFFFFF_Bytes;
    }

    inline Bool Msgpack::IsInt8(Int rhs)
    {
        return rhs >= std::numeric_limits<Byte>::min() && rhs <= std::numeric_limits<Byte>::max();
    }

    inline Bool Msgpack::IsInt16(Int rhs)
    {
        return rhs >= std::numeric_limits<std::int16_t>::min() && rhs <= std::numeric_limits<std::int16_t>::max();
    }

    inline Bool Msgpack::IsInt32(Int rhs)
    {
        return rhs >= std::numeric_limits<std::int32_t>::min() && rhs <= std::numeric_limits<std::int32_t>::max();
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt1(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 1_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt2(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 2_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt4(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 4_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt8(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 8_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt16(const TExtension& rhs)
    {
        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 16_Bytes;
    }

    inline Bool Msgpack::IsStr8(const String& rhs)
    {
        return rhs.size() <= 0xFF;
    }

    inline Bool Msgpack::IsStr16(const String& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    inline Bool Msgpack::IsStr32(const String& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
    }

    template <typename TArray>
    inline Bool Msgpack::IsArray16(const TArray& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    template <typename TArray>
    inline Bool Msgpack::IsArray32(const TArray& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
    }

    template <typename TMap>
    inline Bool Msgpack::IsMap16(const TMap& rhs)
    {
        return rhs.size() <= 0xFFFF;
    }

    template <typename TMap>
    inline Bool Msgpack::IsMap32(const TMap& rhs)
    {
        return rhs.size() <= 0xFFFFFFFF;
    }

    inline Bool Msgpack::IsNegativeFixInt(Byte rhs)
    {
        return rhs >= -63 && rhs <= 0;
    }

}