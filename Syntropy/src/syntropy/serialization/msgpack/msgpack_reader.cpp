#include "syntropy/serialization/msgpack/msgpack_reader.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK READER                                                       */
    /************************************************************************/

    MsgpackReader& MsgpackReader::operator>>(Null& rhs)
    {
        auto sentry = MsgpackReaderSentry(stream_);

        if (Test(MsgpackFormat::kNil))
        {
            rhs = nullptr;
            sentry.Dismiss();
        }

        return *this;
    }

    MsgpackReader& MsgpackReader::operator>>(Bool& rhs)
    {
        auto sentry = MsgpackReaderSentry(stream_);

        if (Test(MsgpackFormat::kTrue))
        {
            rhs = true;
            sentry.Dismiss();
        }
        else if (Test(MsgpackFormat::kFalse))
        {
            rhs = false;
            sentry.Dismiss();
        }

        return *this;
    }

    MsgpackReader& MsgpackReader::operator>>(Int& rhs)
    {
        auto sentry = MsgpackReaderSentry(stream_);

        // Unsigned types are not supported but read anyway for better external compatibility.

        if (Test(MsgpackFormat::kInt64) || Test(MsgpackFormat::kUInt64))
        {
            rhs = ToInt(Endianness::FromBigEndian(Read<Fix64>()));
            sentry.Dismiss();
        }
        if (Test(MsgpackFormat::kInt32) || Test(MsgpackFormat::kUInt32))
        {
            rhs = ToInt(Endianness::FromBigEndian(Read<Fix32>()));
            sentry.Dismiss();
        }
        else if (Test(MsgpackFormat::kInt16) || Test(MsgpackFormat::kUInt16))
        {
            rhs = ToInt(Endianness::FromBigEndian(Read<Fix16>()));
            sentry.Dismiss();
        }
        else if (Test(MsgpackFormat::kInt8) || Test(MsgpackFormat::kUInt8))
        {
            rhs = ToInt(Endianness::FromBigEndian(Read<Fix8>()));
            sentry.Dismiss();
        }
        else if (Test(MsgpackFormat::kNegativeFixInt, MsgpackFormatMask::kNegativeFixInt))
        {
            rhs = -ToInt(Endianness::FromBigEndian(Unpack<Fix8>(MsgpackFormat::kNegativeFixInt)));
            sentry.Dismiss();
        }
        else if (Test(MsgpackFormat::kPositiveFixInt, MsgpackFormatMask::kPositiveFixInt))
        {
            rhs = +ToInt(Endianness::FromBigEndian(Unpack<Fix8>(MsgpackFormat::kPositiveFixInt)));
            sentry.Dismiss();
        }

        return *this;
    }

    MsgpackReader& MsgpackReader::operator>>(Float& rhs)
    {
        auto sentry = MsgpackReaderSentry(stream_);

        if (Test(MsgpackFormat::kFloat32))
        {
            rhs = Memory::BitCast<Float>(Endianness::FromBigEndian(Read<Fix32>()));
            sentry.Dismiss();
        }
        else if (Test(MsgpackFormat::kFloat64))
        {
            // Double precision is not supported, this branch is only for better external compatibility.
            // The result will lose precision.

            rhs = ToFloat(Memory::BitCast<double>(Endianness::FromBigEndian(Read<Fix64>())));
            sentry.Dismiss();
        }

        return *this;
    }

    MsgpackReader& MsgpackReader::operator>>(String& rhs)
    {
        auto sentry = MsgpackReaderSentry(stream_);

        // Type format and length.

        auto length = Int{};

        if (Test(MsgpackFormat::kFixStr, MsgpackFormatMask::kFixStr))
        {
            length = ToInt(Endianness::FromBigEndian(Unpack<Fix8>(MsgpackFormat::kFixStr)));
        }
        else if (Test(MsgpackFormat::kStr8))
        {
            length = ToInt(Endianness::FromBigEndian(Read<Fix8>()));
        }
        else if (Test(MsgpackFormat::kStr16))
        {
            length = ToInt(Endianness::FromBigEndian(Read<Fix16>()));
        }
        else if (Test(MsgpackFormat::kStr32))
        {
            length = ToInt(Endianness::FromBigEndian(Read<Fix32>()));
        }
        else
        {
            return *this;
        }

        // Payload.

        rhs.resize(length);

        Read(MakeMemoryRange(rhs.data(), Bytes(length)));

        sentry.Dismiss();

        return *this;
    }

    MsgpackReader& MsgpackReader::operator>>(MemoryRange& rhs)
    {
        auto sentry = MsgpackReaderSentry(stream_);

        // Type format and size.

        auto size = Int{};

        if (Test(MsgpackFormat::kBin8))
        {
            size = ToInt(Endianness::FromBigEndian(Read<Fix8>()));
        }
        else if (Test(MsgpackFormat::kBin16))
        {
            size = ToInt(Endianness::FromBigEndian(Read<Fix16>()));
        }
        else if (Test(MsgpackFormat::kBin32))
        {
            size = ToInt(Endianness::FromBigEndian(Read<Fix32>()));
        }
        else
        {
            return *this;
        }

        // Payload.

        if (size <= *rhs.GetSize())
        {
            rhs = MemoryRange(rhs.Begin(), Bytes(size));

            Read(rhs);

            sentry.Dismiss();
        }

        return *this;
    }

}
