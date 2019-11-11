
/// \file msgpack.h
/// \brief This header is part of the syntropy serialization system. It contains base definition for the msgpack specification.
///
/// Specification found here: https://github.com/msgpack/msgpack/blob/master/spec.md
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK FORMAT                                                       */
    /************************************************************************/

    /// \brief Defines each type format supported by the msgpack specification.
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
    /* MSGPACK FIX TYPE MASK                                                */
    /************************************************************************/

    /// \brief Mask used for fixed type whose value is embedded within the format specification.
    /// \author Raffaele D. Facendola - November 2019.
    enum class MsgpackFixTypeMask : std::uint8_t
    {
        /// \brief 7-bit positive integer value.
        kPositiveFixInt = 0b10000000,

        /// \brief Mask value for MsgpackFormat::kFixMap.
        kFixMap = 0b11110000,

        /// \brief Mask value for MsgpackFormat::kFixArray.
        kFixArray = 0b11110000,

        /// \brief Mask value for MsgpackFormat::kFixStr.
        kFixStr = 0b11100000,

        /// \brief 5-bit negative integer value.
        kNegativeFixInt = 0b11100000,
    };
}

