
/// \file msgpack_format.h
/// \brief This header is part of the Syntropy serialization module. It contains Msgpack supported types.
///
/// Specification found here: https://github.com/msgpack/msgpack/blob/master/spec.md
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <limits>

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK FORMAT                                                       */
    /************************************************************************/

    /// \brief Type formats supported by Msgpack.
    /// \author Raffaele D. Facendola - November 2019.
    enum class MsgpackFormat : Int
    {
        /// \brief 7-bit positive integer value.
        kPositiveFixInt = 0x00,

        /// \brief Map whose length is up to 15 elements.
        kFixMap = 0x80,

        /// \brief Array whose length is up to 15 elements.
        kFixArray = 0x90,

        /// \brief Byte array whose length is up to 31 bytes.
        kFixStr = 0xA0,

        /// \brief Null value.
        kNil = 0xC0,

        /// \brief This value is never used.
        kUnused = 0xC1,

        /// \brief False boolean value.
        kFalse = 0xC2,

        /// \brief True boolean value.
        kTrue = 0xC3,

        /// \brief Byte array whose length is up to (0x2^8, - 1 Bytes.
        kBin8 = 0xC4,

        /// \brief Byte array whose length is up to (0x2^16, - 1 Bytes.
        kBin16 = 0xC5,

        /// \brief Byte array whose length is up to (0x2^32, - 1 Bytes.
        kBin32 = 0xC6,

        /// \brief Integer and a byte array whose length is up to (0x2^8, - 1 Bytes.
        kExt8 = 0xC7,

        /// \brief Integer and a byte array whose length is up to (0x2^16, - 1 Bytes.
        kExt16 = 0xC8,

        /// \brief Integer and a byte array whose length is up to (0x2^32, - 1 Bytes.
        kExt32 = 0xC9,

        /// \brief Big-endian floating point number in IEEE 754 single precision floating point value.
        kFloat32 = 0xCA,

        /// \brief Big-endian floating point number in IEEE 754 double precision floating point value.
        kFloat64 = 0xCB,

        /// \brief 8-bit unsigned integer value.
        kUInt8 = 0xCC,

        /// \brief 16-bit big-endian unsigned integer value.
        kUInt16 = 0xCD,

        /// \brief 32-bit big-endian unsigned integer value.
        kUInt32 = 0xCE,

        /// \brief 64-bit big-endian unsigned integer value.
        kUInt64 = 0xCF,

        /// \brief 8-bit signed integer value.
        kInt8 = 0xD0,

        /// \brief 16-bit big-endian signed integer value.
        kInt16 = 0xD1,

        /// \brief 32-bit big-endian signed integer value.
        kInt32 = 0xD2,

        /// \brief 64-bit big-endian signed integer value.
        kInt64 = 0xD3,

        /// \brief Integer and a byte array whose length is 1 Byte.
        kFixExt1 = 0xD4,

        /// \brief Integer and a byte array whose length is 2 Bytes.
        kFixExt2 = 0xD5,

        /// \brief Integer and a byte array whose length is 4 Bytes.
        kFixExt4 = 0xD6,

        /// \brief Integer and a byte array whose length is 8 Bytes.
        kFixExt8 = 0xD7,

        /// \brief Integer and a byte array whose length is 16 Bytes.
        kFixExt16 = 0xD8,

        /// \brief Byte array whose length is up to (0x2^8, - 1 Bytes.
        kStr8 = 0xD9,

        /// \brief Byte array whose length is up to (0x2^16, - 1 Bytes.
        kStr16 = 0xDA,

        /// \brief Byte array whose length is up to (0x2^32, - 1 Bytes.
        kStr32 = 0xDB,

        /// \brief Array whose length is up to (0x2^16, - 1 elements.
        kArray16 = 0xDC,

        /// \brief Array whose length is up to (0x2^32, - 1 elements.
        kArray32 = 0xDD,

        /// \brief Map whose length is up to (0x2^16, - 1 elements.
        kMap16 = 0xDE,

        /// \brief Map whose length is up to (0x2^32, - 1 elements.
        kMap32 = 0xDF,

        /// \brief 5-bit negative integer value.
        kNegativeFixInt = 0xE0,
    };

    /************************************************************************/
    /* MSGPACK FORMAT MASK                                                  */
    /************************************************************************/

    /// \brief Masks for packed type formats supported by Msgpack.
    /// Some formats pack together both the type format and the size. This enumeration provides the bit-masks used to unpack the two.
    /// \author Raffaele D. Facendola - May 2020.
    enum class MsgpackFormatMask : Int
    {
        /// \brief 7-bit positive integer value.
        kPositiveFixInt = 0b10000000,

        /// \brief Map whose length is up to 15 elements.
        kFixMap = 0b11110000,

        /// \brief Array whose length is up to 15 elements.
        kFixArray = 0b11110000,

        /// \brief Byte array whose length is up to 31 bytes.
        kFixStr = 0b11100000,

        /// \brief 5-bit negative integer value.
        kNegativeFixInt = 0b11100000
    };

    /************************************************************************/
    /* MSGPACK EXTENSION TYPE                                               */
    /************************************************************************/

    /// \brief Exposes methods used to handle extension types encoded using Msgpack specification.
    /// \author Raffaele D. Facendola - November 2019.
    template <typename TType>
    struct MsgpackExtensionType {};

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

    template <typename TExtension>
    inline Bool Msgpack::IsExt8(const TExtension& rhs)
    {
        using namespace Literals;

        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFF_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsExt16(const TExtension& rhs)
    {
        using namespace Literals;

        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFFFF_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsExt32(const TExtension& rhs)
    {
        using namespace Literals;

        return MsgpackExtensionType<TExtension>::GetSize(rhs) <= 0xFFFFFFFF_Bytes;
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

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt1(const TExtension& rhs)
    {
        using namespace Literals;

        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 1_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt2(const TExtension& rhs)
    {
        using namespace Literals;

        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 2_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt4(const TExtension& rhs)
    {
        using namespace Literals;

        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 4_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt8(const TExtension& rhs)
    {
        using namespace Literals;

        return MsgpackExtensionType<TExtension>::GetSize(rhs) == 8_Bytes;
    }

    template <typename TExtension>
    inline Bool Msgpack::IsFixExt16(const TExtension& rhs)
    {
        using namespace Literals;

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

}
