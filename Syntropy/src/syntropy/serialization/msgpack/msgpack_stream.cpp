#include "syntropy/serialization/msgpack/msgpack_stream.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK STREAM                                                       */
    /************************************************************************/

    MsgpackStream& MsgpackStream::operator<<(std::int8_t rhs)
    {
        if (rhs >= 0 && rhs <= 127)
        {
            return WriteByte(static_cast<std::uint8_t>(MsgpackFormat::kPositiveFixInt) | std::uint8_t(rhs));
        }
        else if (rhs >= -63 && rhs <= 0)
        {
            return WriteByte(static_cast<std::uint8_t>(MsgpackFormat::kNegativeFixInt) | std::uint8_t(-rhs));
        }
        else
        {
            return WriteByte(MsgpackFormat::kInt8).WriteByte(rhs);
        }
    }

    MsgpackStream& MsgpackStream::operator<<(std::int16_t rhs)
    {
        if (rhs >= std::numeric_limits<std::int8_t>::min() && rhs <= std::numeric_limits<std::int8_t>::max())
        {
            return (*this << std::int8_t(rhs));
        }
        else
        {
            return WriteByte(MsgpackFormat::kInt16).WriteBytes(Endianness::ToBigEndian(rhs));
        }
    }

    MsgpackStream& MsgpackStream::operator<<(std::int32_t rhs)
    {
        if (rhs >= std::numeric_limits<std::int16_t>::min() && rhs <= std::numeric_limits<std::int16_t>::max())
        {
            return (*this << std::int16_t(rhs));
        }
        else
        {
            return WriteByte(MsgpackFormat::kInt32).WriteBytes(Endianness::ToBigEndian(rhs));
        }
    }

    MsgpackStream& MsgpackStream::operator<<(std::int64_t rhs)
    {
        if (rhs >= std::numeric_limits<std::int32_t>::min() && rhs <= std::numeric_limits<std::int32_t>::max())
        {
            return (*this << std::int32_t(rhs));
        }
        else
        {
            return WriteByte(MsgpackFormat::kInt64).WriteBytes(Endianness::ToBigEndian(rhs));
        }
    }

    MsgpackStream& MsgpackStream::operator<<(std::uint8_t rhs)
    {
        if (rhs >= 0 && rhs <= 127)
        {
            return WriteByte(static_cast<std::uint8_t>(MsgpackFormat::kPositiveFixInt) | rhs);
        }
        else
        {
            return WriteByte(MsgpackFormat::kUInt8).WriteByte(rhs);
        }
    }

    MsgpackStream& MsgpackStream::operator<<(std::uint16_t rhs)
    {
        if (rhs >= std::numeric_limits<std::uint8_t>::min() && rhs <= std::numeric_limits<std::uint8_t>::max())
        {
            return (*this << std::uint8_t(rhs));
        }
        else
        {
            return WriteByte(MsgpackFormat::kUInt16).WriteBytes(Endianness::ToBigEndian(rhs));
        }
    }

    MsgpackStream& MsgpackStream::operator<<(std::uint32_t rhs)
    {
        if (rhs >= std::numeric_limits<std::uint16_t>::min() && rhs <= std::numeric_limits<std::uint16_t>::max())
        {
            return (*this << std::uint16_t(rhs));
        }
        else
        {
            return WriteByte(MsgpackFormat::kUInt32).WriteBytes(Endianness::ToBigEndian(rhs));
        }
    }

    MsgpackStream& MsgpackStream::operator<<(std::uint64_t rhs)
    {
        if (rhs >= std::numeric_limits<std::uint32_t>::min() && rhs <= std::numeric_limits<std::uint32_t>::max())
        {
            return (*this << std::uint32_t(rhs));
        }
        else
        {
            return WriteByte(MsgpackFormat::kUInt64).WriteBytes(Endianness::ToBigEndian(rhs));
        }
    }

    MsgpackStream& MsgpackStream::operator<<(float rhs)
    {
        auto bytes = std::uint32_t{};

        std::memcpy(&bytes, &rhs, sizeof(float));

        return WriteByte(MsgpackFormat::kFloat32).WriteBytes(Endianness::ToBigEndian(bytes));
    }

    MsgpackStream& MsgpackStream::operator<<(double rhs)
    {
        auto bytes = std::uint64_t{};

        std::memcpy(&bytes, &rhs, sizeof(double));

        return WriteByte(MsgpackFormat::kFloat64).WriteBytes(Endianness::ToBigEndian(bytes));
    }

    MsgpackStream& MsgpackStream::operator<<(const std::string& rhs)
    {
        auto length = rhs.size();
        auto capacity = buffer_.size() + rhs.size();

        // Header.

        if (length <= 31)
        {
            buffer_.reserve(capacity + 1);

            WriteByte(static_cast<std::uint8_t>(MsgpackFormat::kFixStr) | std::uint8_t(length));
        }
        else if (length <= ((1ull << 8u) - 1u))
        {
            buffer_.reserve(capacity + 2);

            WriteByte(MsgpackFormat::kStr8).WriteBytes(std::uint8_t(length));
        }
        else if (length <= ((1ull << 16u) - 1u))
        {
            buffer_.reserve(capacity + 3);

            WriteByte(MsgpackFormat::kStr16).WriteBytes(Endianness::ToBigEndian(std::uint16_t(length)));
        }
        else if (length <= ((1ull << 32u) - 1u))
        {
            buffer_.reserve(capacity + 5);

            WriteByte(MsgpackFormat::kStr32).WriteBytes(Endianness::ToBigEndian(std::uint32_t(length)));
        }

        // Data.

        return WriteBytes(rhs.data(), sizeof(char) * length);
    }

    MsgpackStream& MsgpackStream::operator<<(const std::vector<std::uint8_t>& rhs)
    {
        auto length = rhs.size();
        auto capacity = buffer_.size() + rhs.size();

        // Header.

        if (length <= ((1ull << 8u) - 1u))
        {
            buffer_.reserve(capacity + 2);

            WriteByte(MsgpackFormat::kBin8).WriteBytes(std::uint8_t(length));
        }
        else if (length <= ((1ull << 16u) - 1u))
        {
            buffer_.reserve(capacity + 3);

            WriteByte(MsgpackFormat::kBin16).WriteBytes(Endianness::ToBigEndian(std::uint16_t(length)));
        }
        else if (length <= ((1ull << 32u) - 1u))
        {
            buffer_.reserve(capacity + 5);

            WriteByte(MsgpackFormat::kBin32).WriteBytes(Endianness::ToBigEndian(std::uint32_t(length)));
        }

        // Data.

        return WriteBytes(rhs.data(), sizeof(std::uint8_t) * length);
    }

    MsgpackStream& MsgpackStream::operator>>(bool& rhs)
    {
        if (TestByte(MsgpackFormat::kTrue))
        {
            rhs = true;
        }
        else if (TestByte(MsgpackFormat::kFalse))
        {
            rhs = false;
        }
        else
        {
            SetReadFail();
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::int8_t& rhs)
    {
        if (auto positive_fix_int = TestByte(MsgpackFormat::kPositiveFixInt, MsgpackFixTypeMask::kPositiveFixInt))
        {
            rhs = +(*positive_fix_int & ~static_cast<std::uint8_t>(MsgpackFixTypeMask::kPositiveFixInt));
        }
        else if (auto negative_fix_int = TestByte(MsgpackFormat::kNegativeFixInt, MsgpackFixTypeMask::kNegativeFixInt))
        {
            rhs = -(*negative_fix_int & ~static_cast<std::uint8_t>(MsgpackFixTypeMask::kNegativeFixInt));
        }
        else if (TestByte(MsgpackFormat::kInt8))
        {
            ReadByte(rhs);
        }
        else
        {
            SetReadFail();
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::int16_t& rhs)
    {
        if (TestByte(MsgpackFormat::kInt16))
        {
            ReadBytes(rhs);

            rhs = Endianness::FromBigEndian(rhs);
        }
        else if (auto rhs_low = std::int8_t{}; !(*this >> rhs_low).IsReadFail())
        {
            rhs = rhs_low;
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::int32_t& rhs)
    {
        if (TestByte(MsgpackFormat::kInt32))
        {
            ReadBytes(rhs);

            rhs = Endianness::FromBigEndian(rhs);
        }
        else if (auto rhs_low = std::int16_t{}; !(*this >> rhs_low).IsReadFail())
        {
            rhs = rhs_low;
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::int64_t& rhs)
    {
        if (TestByte(MsgpackFormat::kInt64))
        {
            ReadBytes(rhs);

            rhs = Endianness::FromBigEndian(rhs);
        }
        else if (auto rhs_low = std::int32_t{}; !(*this >> rhs_low).IsReadFail())
        {
            rhs = rhs_low;
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::uint8_t& rhs)
    {
        if (auto positive_fix_int = TestByte(MsgpackFormat::kPositiveFixInt, MsgpackFixTypeMask::kPositiveFixInt))
        {
            rhs = +(*positive_fix_int & ~static_cast<std::uint8_t>(MsgpackFixTypeMask::kPositiveFixInt));
        }
        else if (TestByte(MsgpackFormat::kUInt8))
        {
            ReadByte(rhs);
        }
        else
        {
            SetReadFail();
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::uint16_t& rhs)
    {
        if (TestByte(MsgpackFormat::kUInt16))
        {
            ReadBytes(rhs);

            rhs = Endianness::FromBigEndian(rhs);
        }
        else if (auto rhs_low = std::uint8_t{}; !(*this >> rhs_low).IsReadFail())
        {
            rhs = rhs_low;
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::uint32_t& rhs)
    {
        if (TestByte(MsgpackFormat::kUInt32))
        {
            ReadBytes(rhs);

            rhs = Endianness::FromBigEndian(rhs);
        }
        else if (auto rhs_low = std::uint16_t{}; !(*this >> rhs_low).IsReadFail())
        {
            rhs = rhs_low;
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::uint64_t& rhs)
    {
        if (TestByte(MsgpackFormat::kUInt64))
        {
            ReadBytes(rhs);

            rhs = Endianness::FromBigEndian(rhs);
        }
        else if (auto rhs_low = std::uint32_t{}; !(*this >> rhs_low).IsReadFail())
        {
            rhs = rhs_low;
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(float& rhs)
    {
        if (TestByte(MsgpackFormat::kFloat32))
        {
            auto buffer = std::uint32_t{};

            ReadBytes(buffer);

            buffer = Endianness::FromBigEndian(buffer);

            std::memcpy(&rhs, &buffer, sizeof(float));
        }
        else
        {
            SetReadFail();
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(double& rhs)
    {
        if (TestByte(MsgpackFormat::kFloat64))
        {
            auto buffer = std::uint64_t{};

            ReadBytes(buffer);

            buffer = Endianness::FromBigEndian(buffer);

            std::memcpy(&rhs, &buffer, sizeof(double));
        }
        else
        {
            SetReadFail();
        }

        return *this;
    }

    MsgpackStream& MsgpackStream::operator>>(std::string& rhs)
    {
        // Header.

        auto length = std::uint32_t{};

        if (auto FixStr = TestByte(MsgpackFormat::kFixStr, MsgpackFixTypeMask::kFixStr))
        {
            length = std::uint32_t((*FixStr) & ~static_cast<std::uint8_t>(MsgpackFixTypeMask::kFixStr));
        }
        else if (TestByte(MsgpackFormat::kStr8))
        {
            auto length_low = std::uint8_t{};

            ReadByte(length_low);

            length = std::uint32_t(length_low);
        }
        else if(TestByte(MsgpackFormat::kStr16))
        {
            auto length_low = std::uint16_t{};

            ReadByte(length_low);

            length = std::uint32_t(Endianness::FromBigEndian(length_low));
        }
        else if (TestByte(MsgpackFormat::kStr32))
        {
            auto length_low = std::uint32_t{};

            ReadByte(length_low);

            length = std::uint32_t(Endianness::FromBigEndian(length_low));
        }
        else
        {
            SetReadFail();
        }

        // Data.

        if (!IsReadFail())
        {
            rhs.resize(length);

            ReadBytes(rhs.data(), sizeof(char) * length);
        }

        return *this;
    }
}
