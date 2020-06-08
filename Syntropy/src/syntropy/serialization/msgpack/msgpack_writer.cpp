#include "syntropy/serialization/msgpack/msgpack_writer.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK ENCODER                                                      */
    /************************************************************************/

    void MsgpackStreamEncoder::operator()(OutputStream& lhs, Integer rhs) const
    {
        if (Msgpack::IsPositiveFixInt(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(rhs));

            lhs << Pack(MsgpackFormat::kPositiveFixInt, bytes, MsgpackFormatMask::kPositiveFixInt);
        }
        else if (Msgpack::IsNegativeFixInt(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(-rhs));

            lhs << Pack(MsgpackFormat::kNegativeFixInt, bytes, MsgpackFormatMask::kNegativeFixInt);
        }
        else if (Msgpack::IsInt8(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(rhs));

            lhs << MsgpackFormat::kInt8 << bytes;
        }
        else if (Msgpack::IsInt16(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix16(rhs));

            lhs << MsgpackFormat::kInt16 << bytes;
        }
        else if (Msgpack::IsInt32(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix32(rhs));

            lhs << MsgpackFormat::kInt32 << bytes;
        }
        else
        {
            auto bytes = Endianness::ToBigEndian(ToFix64(rhs));

            lhs << MsgpackFormat::kInt64 << bytes;
        }
    }

    inline void MsgpackStreamEncoder::operator()(OutputStream& lhs, const String& rhs) const
    {
        // Type format and size.

        if (Msgpack::IsFixStr(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            lhs << Pack(MsgpackFormat::kFixStr, encoded_size, MsgpackFormatMask::kFixStr);
        }
        else if (Msgpack::IsStr8(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            lhs << MsgpackFormat::kStr8 << encoded_size;
        }
        else if (Msgpack::IsStr16(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            lhs << MsgpackFormat::kStr16 << encoded_size;
        }
        else if (Msgpack::IsStr32(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            lhs << MsgpackFormat::kStr32 << encoded_size;
        }

        // Payload.

        lhs << MakeConstMemoryRange(rhs.data(), ToBytes(rhs.size()));

    }

    inline void MsgpackStreamEncoder::operator()(OutputStream& lhs, const ConstMemoryRange& rhs) const
    {
        // Type format and size.

        if (Msgpack::IsBin8(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(*rhs.GetSize()));

            lhs << MsgpackFormat::kBin8 << encoded_size;
        }
        else if (Msgpack::IsBin16(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix16(*rhs.GetSize()));

            lhs << MsgpackFormat::kBin16 << encoded_size;
        }
        else if (Msgpack::IsBin32(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix32(*rhs.GetSize()));

            lhs << MsgpackFormat::kBin32 << encoded_size;
        }

        // Payload.

        lhs << rhs;
    }

}