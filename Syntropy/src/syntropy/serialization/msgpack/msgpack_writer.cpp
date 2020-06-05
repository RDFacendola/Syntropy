#include "syntropy/serialization/msgpack/msgpack_writer.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK WRITER                                                       */
    /************************************************************************/

    MsgpackWriter& MsgpackWriter::operator<<(Integer rhs)
    {
        if (Msgpack::IsPositiveFixInt(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(rhs));

            Pack(MsgpackFormat::kPositiveFixInt, bytes);
        }
        else if (Msgpack::IsNegativeFixInt(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(-rhs));

            Pack(MsgpackFormat::kNegativeFixInt, bytes);
        }
        else if (Msgpack::IsInt8(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(rhs));

            Write(MsgpackFormat::kInt8, bytes);
        }
        else if (Msgpack::IsInt16(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix16(rhs));

            Write(MsgpackFormat::kInt16, bytes);
        }
        else if (Msgpack::IsInt32(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix32(rhs));

            Write(MsgpackFormat::kInt32, bytes);
        }
        else
        {
            auto bytes = Endianness::ToBigEndian(ToFix64(rhs));

            Write(MsgpackFormat::kInt64, bytes);
        }

        return *this;
    }

    MsgpackWriter& MsgpackWriter::operator<<(const String& rhs)
    {
        // Type format and size.

        if (Msgpack::IsFixStr(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            Pack(MsgpackFormat::kFixStr, size);
        }
        else if (Msgpack::IsStr8(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            Write(MsgpackFormat::kStr8, size);
        }
        else if (Msgpack::IsStr16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            Write(MsgpackFormat::kStr16, size);
        }
        else if (Msgpack::IsStr32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            Write(MsgpackFormat::kStr32, size);
        }

        // Payload.

        auto data = MakeConstMemoryRange(rhs.data(), Bytes(rhs.size()));

        Write(data);

        return *this;
    }

    MsgpackWriter& MsgpackWriter::operator<<(const ConstMemoryRange& rhs)
    {
        // Type format and size.

        if (Msgpack::IsBin8(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(*rhs.GetSize()));

            Write(MsgpackFormat::kBin8, size);
        }
        else if (Msgpack::IsBin16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(*rhs.GetSize()));

            Write(MsgpackFormat::kBin16, size);
        }
        else if (Msgpack::IsBin32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(*rhs.GetSize()));

            Write(MsgpackFormat::kBin32, size);
        }

        // Payload.

        return Write(rhs);
    }

}
