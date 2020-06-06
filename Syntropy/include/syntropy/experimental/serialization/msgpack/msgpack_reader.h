
/// \file msgpack_reader.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for Msgpack stream readers.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <ostream>
#include <optional>

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/platform/endianness.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/map.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory.h"
#include "syntropy/math/math.h"
#include "syntropy/language/type_traits.h"

#include "syntropy/serialization/msgpack/msgpack.h"

namespace syntropy
{


    /************************************************************************/
    /* MSGPACK STREAM READER                                                */
    /************************************************************************/

    /// \brief Exposes methods to read from a Msgpack stream.
    /// \author Raffaele D. Facendola - June 2020.
    class MsgpackStreamReader
    {
    public:

        /// \brief Type of the underlying output stream.
        using TStream = BasicStringStream<Byte>;

        /// \brief Type of the underlying string.
        using TString = BasicString<Byte>;

        /// \brief Create an empty reader.
        MsgpackStreamReader() = default;

        /// \brief Create a reader from string.
        MsgpackStreamReader(TString stream);

        /// \brief No copy constructor.
        MsgpackStreamReader(const MsgpackStreamReader&) = delete;

        /// \brief No move constructor.
        MsgpackStreamReader(MsgpackStreamReader&&) = delete;

        /// \brief No copy-assignment constructor.
        MsgpackStreamReader& operator=(const MsgpackStreamReader&) = delete;

        /// \brief No move-assignment constructor.
        MsgpackStreamReader& operator=(MsgpackStreamReader&&) = delete;

        /// \brief Default destructor.
        ~MsgpackStreamReader() = default;

        /// \brief Peek a single byte from the underlying stream and test it against a format using a mask as per (byte & mask == format).
        /// Both format and mask are expected to be convertible to Byte, otherwise the behavior of this method is undefined.
        /// \return If the masked byte matches the provided format returns true, otherwise returns false.
        template <typename TFormat, typename TMask = Byte>
        Bool Test(TFormat format, TMask mask = Byte{ 0b11111111 });

        /// \brief Peek a single byte from the underlying stream and test it against a format, eventually consuming the byte.
        /// Format type is expected to be convertible to Byte, otherwise the behavior of this method is undefined.
        /// \return If the peeked byte matches the provided format, consume the byte and returns true, otherwise returns false.
        template <typename TFormat>
        Bool Consume(TFormat format);

        /// \brief Read a packed value from the underlying stream.
        /// \brief Both format and payload are expected to be convertible to Byte.
        /// \return Returns the unpacked payload.
        template <typename TPayload, typename TFormat>
        TPayload Unpack(TFormat format);

        /// \brief Read a value from the underlying stream.
        template <typename TType>
        TType Read();

        /// \brief Read data from the underlying stream.
        void ReadRaw(const MemoryRange& data);

        /// \brief Set the underlying string and reset the read count.
        void FromString(const TString& stream);

        /// \brief Get the total amount of bytes read from the stream.
        Bytes GetReadCount() const;

    private:

        /// \brief Underlying stream.
        TStream stream_;

        /// \brief Number of written bytes.
        Bytes read_count_;

    };

    /************************************************************************/
    /* MSGPACK READ TRANSACTION                                             */
    /************************************************************************/

    /// \brief Sentry object used to rollback the status of the owning stream upon destruction unless dismissed.
    /// This object is meant to rollback a stream when a read failed after sentry construction.
    /// A sentry constructed with a stream whose fail bit was already set does not clear the stream status and behaves as if it was never constructed.
    /// \author Raffaele D. Facendola - November 2019.
    class MsgpackReaderSentry
    {
    public:

        /// \brief Create the sentry.
        MsgpackReaderSentry(MsgpackReader::TStream& stream_);

        /// \brief Destructor. Causes the rollback of the stream state if not dismissed.
        ~MsgpackReaderSentry();

        /// \brief Dismiss the sentry.
        void Dismiss();

    private:

        /// \brief Underlying stream.
        MsgpackReader::TStream& stream_;

        /// \brief Position of the stream upon sentry construction.
        std::streampos position_;

        /// \brief Whether the sentry was dismissed.
        Bool dismissed_{ false };

        /// \brief Whether the sentry was constructed with a valid stream.
        Bool engaged_{ false };

    };

    /************************************************************************/
    /* MSGPACK READER                                                       */
    /************************************************************************/

    /// \brief Reader for data streams encoded via Msgpack specification.
    /// \author Raffaele D. Facendola - May 2020.
    /// \see https://github.com/msgpack/msgpack/blob/master/spec.md
    class MsgpackReader
    {
    public:

        /// \brief Type of the underlying string.
        using TString = MsgpackStreamReader::TString;

        /// \brief Create a reader from a string.
        MsgpackReader(TString stream);

        /// \brief Extract a null value.
        MsgpackReader& operator>>(Null& rhs);

        /// \brief Extract a boolean value.
        MsgpackReader& operator>>(Bool& rhs);

        /// \brief Extract an integer value.
        MsgpackReader& operator>>(Int& rhs);

        /// \brief Extract a floating point value.
        MsgpackReader& operator>>(Float& rhs);

        /// \brief Extract a string.
        MsgpackReader& operator>>(String& rhs);

        /// \brief Extract a byte-array.
        MsgpackReader& operator>>(MemoryRange& rhs);

        /// \brief Extract an array.
        template <typename TElement>
        MsgpackReader& operator>>(Vector<TElement>& rhs);

        /// \brief Extract a map.
        template <typename TKey, typename TValue>
        MsgpackReader& operator>>(Map<TKey, TValue>& rhs);

        /// \brief Extract an extension-type value.
        template <typename TExtension, typename = EnableIfValidExpressionT<IsMsgpackExtension, TExtension>>
        MsgpackReader& operator>>(TExtension& rhs);

        /// \brief Get the index of the next byte to read.
        Int GetReadPosition() const;

    private:

        /// \brief Underlying stream reader.
        MsgpackStreamReader stream_reader_;

    };



    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackStreamReader.

    inline MsgpackStreamReader::MsgpackStreamReader(TString stream)
        : stream_(std::move(stream))
    {

    }

    template <typename TFormat, typename TMask>
    inline Bool MsgpackStreamReader::Test(TFormat format, TMask mask)
    {
        return (ToByte(stream_.peek()) & ToByte(mask)) == ToByte(format);
    }

    template <typename TFormat>
    inline Bool MsgpackStreamReader::Consume(TFormat format)
    {
        if (Test(format))
        {
            Read<Byte>();
            return true;
        }

        return false;
    }

    template <typename TPayload, typename TFormat>
    inline TPayload MsgpackStreamReader::Unpack(TFormat format)
    {
        static_assert(sizeof(TPayload) == sizeof(Byte), "Payload type must be 8-bit wide.");
        static_assert(sizeof(TFormat) == sizeof(Byte), "Format type must be 8-bit wide.");

        auto unpack = Read<Byte>() & ~ToByte(format);

        static_assert(sizeof(unpack) == sizeof(Byte));

        return Memory::BitCast<TPayload>(unpack);
    }

    template <typename TType>
    inline TType MsgpackStreamReader::Read()
    {
        auto buffer = TType{};

        ReadRaw(MakeMemoryRange(buffer));

        return buffer;
    }

    inline void MsgpackStreamReader::ReadRaw(const MemoryRange& data)
    {
        stream_.read(data.Begin().As<Byte>(), *data.GetSize());

        read_count_ += data.GetSize();
    }

    inline void MsgpackStreamReader::FromString(const TString& stream)
    {
        stream_.str(stream);

        read_count_ = Bytes{ 0 };
    }

    inline Bytes MsgpackStreamReader::GetReadCount() const
    {
        return read_count_;
    }

    // MsgpackReader.

    inline MsgpackReader::MsgpackReader(TString stream)
        : stream_reader_(std::move(stream))
    {

    }

    template <typename TElement>
    MsgpackReader& MsgpackReader::operator>>(Vector<TElement>& rhs)
    {
        auto sentry = MsgpackReaderSentry(stream_);

        // Type format and count.

        auto count = Int{};

        if (Test(MsgpackFormat::kFixArray, MsgpackFormatMask::kFixArray))
        {
            count = ToInt(Endianness::FromBigEndian(Unpack<Fix8>(MsgpackFormat::kFixArray)));
        }
        else if (Test(MsgpackFormat::kArray16))
        {
            count = ToInt(Endianness::FromBigEndian(Read<Fix16>()));
        }
        else if (Test(MsgpackFormat::kArray32))
        {
            count = ToInt(Endianness::FromBigEndian(Read<Fix32>()));
        }
        else
        {
            return *this;
        }

        // Payload.

        rhs.clear();
        rhs.reserve(*count);

        for (; count > 0; --count)
        {
            auto element = TElement{};

            operator>>(element);

            rhs.emplace_back(std::move(element));
        }

        sentry.Dismiss();

        return *this;
    }

    template <typename TKey, typename TValue>
    MsgpackReader& MsgpackReader::operator>>(Map<TKey, TValue>& rhs)
    {
        auto sentry = MsgpackReaderSentry(stream_);

        // Type format and count.

        auto count = Int{};

        if (Test(MsgpackFormat::kFixMap, MsgpackFormatMask::kFixMap))
        {
            count = ToInt(Endianness::FromBigEndian(Unpack<Fix8>(MsgpackFormat::kFixMap)));
        }
        else if (Test(MsgpackFormat::kMap16))
        {
            count = ToInt(Endianness::FromBigEndian(Read<Fix16>()));
        }
        else if (Test(MsgpackFormat::kMap32))
        {
            count = ToInt(Endianness::FromBigEndian(Read<Fix32>()));
        }
        else
        {
            return *this;
        }

        // Payload.

        rhs.clear();
        rhs.reserve(count);

        for (; count > 0; --count)
        {
            auto key = TKey{};
            auto value = TValue{};

            operator>>(key);
            operator>>(value);

            rhs.emplace(std::make_pair(std::move(key), std::move(value)));
        }

        sentry.Dismiss();

        return *this;
    }

    template <typename TExtension, typename>
    MsgpackReader& MsgpackReader::operator>>(TExtension& rhs)
    {
        using namespace Literals;

        using TMsgpackExtension = MsgpackExtension<TExtension>;

        auto sentry = MsgpackReaderSentry(stream_);

        auto size = Bytes{};

        if (Test(MsgpackFormat::kFixExt1))
        {
            size = 1_Bytes;
        }
        else if (Test(MsgpackFormat::kFixExt2))
        {
            size = 2_Bytes;
        }
        else if (Test(MsgpackFormat::kFixExt4))
        {
            size = 4_Bytes;
        }
        else if (Test(MsgpackFormat::kFixExt8))
        {
            size = 8_Bytes;
        }
        else if (Test(MsgpackFormat::kFixExt16))
        {
            size = 16_Bytes;
        }
        else if (Test(MsgpackFormat::kExt8))
        {
            size = Bytes(ToInt(Endianness::FromBigEndian(Read<Fix8>())));
        }
        else if (Test(MsgpackFormat::kExt16))
        {
            size = Bytes(ToInt(Endianness::FromBigEndian(Read<Fix16>())));
        }
        else if (Test(MsgpackFormat::kExt32))
        {
            size = Bytes(ToInt(Endianness::FromBigEndian(Read<Fix32>())));
        }
        else
        {
            return *this;
        }

        // Payload.

        if (Test(TMsgpackExtension::GetFormat()))
        {
            TMsgpackExtension::Decode(static_cast<TInputStream&>(stream_), size, rhs);

            sentry.Dismiss();
        }

        return *this;
    }

    inline Bool MsgpackReader::IsFail() const
    {
        return stream_.fail();
    }

    inline Bool MsgpackReader::IsEndOfStream() const
    {
        return stream_.eof();
    }

    inline void MsgpackReader::Recover()
    {
        stream_.clear();
    }

    inline Int MsgpackReader::GetReadPosition() const
    {
        return static_cast<Int>(stream_.rdbuf()->pubseekoff(0, std::ios::cur, std::ios::in));
    }

    // MsgpackReadSentry.

    inline MsgpackReaderSentry::MsgpackReaderSentry(MsgpackReader::TStream& stream)
        : stream_(stream)
    {
        if (!stream_.fail())
        {
            engaged_ = true;
            position_ = stream.tellg();
        }
    }

    inline MsgpackReaderSentry::~MsgpackReaderSentry()
    {
        // If the failbit is set a read after sentry construction failed and wrong data were decoded: rollback!

        if (engaged_ && (!dismissed_ || stream_.fail()))
        {
            auto state = stream_.rdstate() | std::ios::failbit;

            stream_.clear();
            stream_.seekg(position_);

            stream_.setstate(state);
        }
    }

    inline void MsgpackReaderSentry::Dismiss()
    {
        dismissed_ = true;
    }

}

