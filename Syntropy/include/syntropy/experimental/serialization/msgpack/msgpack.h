
/// \file msgpack.h
/// \brief This header is part of the Syntropy serialization module. It contains Msgpack type, formats and utility methods.
///
/// Specification found here: https://github.com/msgpack/msgpack/blob/master/spec.md
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <limits>

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_span.h"
#include "syntropy/experimental/serialization/append_stream_buffer.h"
#include "syntropy/experimental/serialization/consume_stream_buffer.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK FORMAT                                                       */
    /************************************************************************/

    /// \brief Type formats supported by Msgpack.
    /// \author Raffaele D. Facendola - November 2019.
    enum class MsgpackFormat : Fix8
    {
        /// \brief 7-bit positive integer value.
        kPositiveFixInt = ToFix8(0x00),

        /// \brief Map whose length is up to 15 elements.
        kFixMap = ToFix8(0x80),

        /// \brief Array whose length is up to 15 elements.
        kFixArray = ToFix8(0x90),

        /// \brief Byte array whose length is up to 31 bytes.
        kFixStr = ToFix8(0xA0),

        /// \brief Null value.
        kNil = ToFix8(0xC0),

        /// \brief This value is never used.
        kUnused = ToFix8(0xC1),

        /// \brief False boolean value.
        kFalse = ToFix8(0xC2),

        /// \brief True boolean value.
        kTrue = ToFix8(0xC3),

        /// \brief Byte array whose length is up to (ToFix8(0x2^8), - 1 Bytes.
        kBin8 = ToFix8(0xC4),

        /// \brief Byte array whose length is up to (ToFix8(0x2^16), - 1 Bytes.
        kBin16 = ToFix8(0xC5),

        /// \brief Byte array whose length is up to (ToFix8(0x2^32), - 1 Bytes.
        kBin32 = ToFix8(0xC6),

        /// \brief Integer and a byte array whose length is up to (ToFix8(0x2^8), - 1 Bytes.
        kExt8 = ToFix8(0xC7),

        /// \brief Integer and a byte array whose length is up to (ToFix8(0x2^16), - 1 Bytes.
        kExt16 = ToFix8(0xC8),

        /// \brief Integer and a byte array whose length is up to (ToFix8(0x2^32), - 1 Bytes.
        kExt32 = ToFix8(0xC9),

        /// \brief Big-endian floating point number in IEEE 754 single precision floating point value.
        kFloat32 = ToFix8(0xCA),

        /// \brief Big-endian floating point number in IEEE 754 double precision floating point value.
        kFloat64 = ToFix8(0xCB),

        /// \brief 8-bit unsigned integer value.
        kUInt8 = ToFix8(0xCC),

        /// \brief 16-bit big-endian unsigned integer value.
        kUInt16 = ToFix8(0xCD),

        /// \brief 32-bit big-endian unsigned integer value.
        kUInt32 = ToFix8(0xCE),

        /// \brief 64-bit big-endian unsigned integer value.
        kUInt64 = ToFix8(0xCF),

        /// \brief 8-bit signed integer value.
        kInt8 = ToFix8(0xD0),

        /// \brief 16-bit big-endian signed integer value.
        kInt16 = ToFix8(0xD1),

        /// \brief 32-bit big-endian signed integer value.
        kInt32 = ToFix8(0xD2),

        /// \brief 64-bit big-endian signed integer value.
        kInt64 = ToFix8(0xD3),

        /// \brief Integer and a byte array whose length is 1 Byte.
        kFixExt1 = ToFix8(0xD4),

        /// \brief Integer and a byte array whose length is 2 Bytes.
        kFixExt2 = ToFix8(0xD5),

        /// \brief Integer and a byte array whose length is 4 Bytes.
        kFixExt4 = ToFix8(0xD6),

        /// \brief Integer and a byte array whose length is 8 Bytes.
        kFixExt8 = ToFix8(0xD7),

        /// \brief Integer and a byte array whose length is 16 Bytes.
        kFixExt16 = ToFix8(0xD8),

        /// \brief Byte array whose length is up to 2^8 - 1 Bytes.
        kStr8 = ToFix8(0xD9),

        /// \brief Byte array whose length is up to 2^16 - 1 Bytes.
        kStr16 = ToFix8(0xDA),

        /// \brief Byte array whose length is up to 2^32 - 1 Bytes.
        kStr32 = ToFix8(0xDB),

        /// \brief Array whose length is up to 2^16 - 1 elements.
        kArray16 = ToFix8(0xDC),

        /// \brief Array whose length is up to 2^32 - 1 elements.
        kArray32 = ToFix8(0xDD),

        /// \brief Map whose length is up to 2^16 - 1 elements.
        kMap16 = ToFix8(0xDE),

        /// \brief Map whose length is up to 2^32 - 1 elements.
        kMap32 = ToFix8(0xDF),

        /// \brief 5-bit negative integer value.
        kNegativeFixInt = ToFix8(0xE0)
    };

    /************************************************************************/
    /* MSGPACK FORMAT MASK                                                  */
    /************************************************************************/

    /// \brief Masks for packed type formats supported by Msgpack.
    /// Some formats pack together both the type format and the size. This enumeration provides the bit-masks used to unpack the two apart.
    /// \author Raffaele D. Facendola - May 2020.
    enum class MsgpackFormatMask : Fix8
    {
        /// \brief No bit set.
        kNone = ToFix8(0b00000000),

        /// \brief 7-bit positive integer value.
        kPositiveFixInt = ToFix8(0b10000000),

        /// \brief Map whose length is up to 15 elements.
        kFixMap = ToFix8(0b11110000),

        /// \brief Array whose length is up to 15 elements.
        kFixArray = ToFix8(0b11110000),

        /// \brief Byte array whose length is up to 31 bytes.
        kFixStr = ToFix8(0b11100000),

        /// \brief 5-bit negative integer value.
        kNegativeFixInt = ToFix8(0b11100000),

        /// \brief All bit set.
        kAll = ToFix8(0b11111111)
    };

    /************************************************************************/
    /* MSGPACK EXTENSION TYPE                                               */
    /************************************************************************/

    /// \brief Type of a Msgpack extension.
    /// \author Raffaele D. Facendola - June 2020.
    enum class MsgpackExtensionType : Fix8 {};

    /// \brief Encoder for a Msgoack extension type.
    /// \author Raffaele D. Facendola - November 2019.
    template <typename TType>
    struct MsgpackExtensionEncoder {};

    /// \brief Decoder for a Msgoack extension type.
    /// \author Raffaele D. Facendola - November 2019.
    template <typename TType>
    struct MsgpackExtensionDecoder {};

    /// \brief Predicate used to determine whether TType is a valid extension type for Msgpack.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType>
    using HasMsgpackExtensionEncoder = decltype(MsgpackExtensionEncoder<TType>{}(std::declval<AppendStreamBuffer&>(), std::declval<const TType&>()));

    /// \brief Predicate used to determine whether TType is a valid extension type for Msgpack.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType>
    using HasMsgpackExtensionDecoder = decltype(MsgpackExtensionDecoder<TType>{}(std::declval<ConsumeStreamBuffer&>(), std::declval<TType&>()));

    /************************************************************************/
    /* MSGPACK                                                              */
    /************************************************************************/

    /// \brief Define methods used to check the encoded format of values during encoding.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Msgpack
    {
        /// \brief Check whether rhs can be encoded using a positive fix int.
        Bool IsPositiveFixInt(Int rhs);

        /// \brief Check whether rhs can be encoded using a negative fix int.
        Bool IsNegativeFixInt(Int rhs);

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

        /// \brief Check whether rhs can be encoded using a 8-bit signed int.
        Bool IsInt8(Int rhs);

        /// \brief Check whether rhs can be encoded using a 16-bit signed int.
        Bool IsInt16(Int rhs);

        /// \brief Check whether rhs can be encoded using a 32-bit signed int.
        Bool IsInt32(Int rhs);

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

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Msgpack.

    inline Bool Msgpack::IsPositiveFixInt(Int rhs)
    {
        return rhs >= 0 && rhs <= 127;
    }

    inline Bool Msgpack::IsNegativeFixInt(Int rhs)
    {
        return rhs >= -63 && rhs <= 0;
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

    inline Bool Msgpack::IsInt8(Int rhs)
    {
        return rhs >= std::numeric_limits<Fix8>::min() && rhs <= std::numeric_limits<Fix8>::max();
    }

    inline Bool Msgpack::IsInt16(Int rhs)
    {
        return rhs >= std::numeric_limits<Fix16>::min() && rhs <= std::numeric_limits<Fix16>::max();
    }

    inline Bool Msgpack::IsInt32(Int rhs)
    {
        return rhs >= std::numeric_limits<Fix32>::min() && rhs <= std::numeric_limits<Fix32>::max();
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

}
