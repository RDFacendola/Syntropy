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

            stream_writer_.Pack(MsgpackFormat::kPositiveFixInt, bytes);
        }
        else if (Msgpack::IsNegativeFixInt(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(-rhs));

            stream_writer_.Pack(MsgpackFormat::kNegativeFixInt, bytes);
        }
        else if (Msgpack::IsInt8(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(rhs));

            stream_writer_.Write(MsgpackFormat::kInt8, bytes);
        }
        else if (Msgpack::IsInt16(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix16(rhs));

            stream_writer_.Write(MsgpackFormat::kInt16, bytes);
        }
        else if (Msgpack::IsInt32(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix32(rhs));

            stream_writer_.Write(MsgpackFormat::kInt32, bytes);
        }
        else
        {
            auto bytes = Endianness::ToBigEndian(ToFix64(rhs));

            stream_writer_.Write(MsgpackFormat::kInt64, bytes);
        }

        return *this;
    }

    MsgpackWriter& MsgpackWriter::operator<<(const String& rhs)
    {
        // Type format and size.

        if (Msgpack::IsFixStr(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            stream_writer_.Pack(MsgpackFormat::kFixStr, size);
        }
        else if (Msgpack::IsStr8(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            stream_writer_.Write(MsgpackFormat::kStr8, size);
        }
        else if (Msgpack::IsStr16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            stream_writer_.Write(MsgpackFormat::kStr16, size);
        }
        else if (Msgpack::IsStr32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            stream_writer_.Write(MsgpackFormat::kStr32, size);
        }

        // Payload.

        auto data = MakeConstMemoryRange(rhs.data(), Bytes(rhs.size()));

        stream_writer_.Write(data);

        return *this;
    }

    MsgpackWriter& MsgpackWriter::operator<<(const ConstMemoryRange& rhs)
    {
        // Type format and size.

        if (Msgpack::IsBin8(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(*rhs.GetSize()));

            stream_writer_.Write(MsgpackFormat::kBin8, size);
        }
        else if (Msgpack::IsBin16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(*rhs.GetSize()));

            stream_writer_.Write(MsgpackFormat::kBin16, size);
        }
        else if (Msgpack::IsBin32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(*rhs.GetSize()));

            stream_writer_.Write(MsgpackFormat::kBin32, size);
        }

        // Payload.

        stream_writer_.WriteRaw(rhs);

        return *this;
    }

}
