
/// \file msgpack_reader.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for Msgpack stream readers.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <ostream>

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

#include "syntropy/serialization/msgpack/msgpack_format.h"

namespace syntropy
{
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
        using TString = BasicString<Byte>;

        /// \brief Type of the underlying stream.
        using TStream = BasicStringStream<Byte>;

        /// \brief Type of the underlying input stream.
        using TInputStream = BasicIStringStream<Byte>;

        /// \brief Create an empty reader.
        MsgpackReader() = default;

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
        template <typename TExtension>
        MsgpackReader& operator>>(TExtension& rhs);

        /// \brief Check whether the fail bit of the underlying stream is set.
        /// \return Returns true if the fail bit is set, returns false otherwise
        Bool IsFail() const;

        /// \brief Check whether the last byte in the stream was read.
        /// \return Returns true if the last byte in the stream was read, returns false otherwise.
        Bool IsEndOfStream() const;

        /// \brief Clear the status of the stream after a reading error.
        void Recover();

        /// \brief Get the index of the next byte to read.
        Int GetReadPosition() const;

    private:

        /// \brief Peek a single byte from the underlying stream and test it with the provided type.
        /// \return If the peeked byte matches the provided type, consume the byte and returns true, otherwise returns false.
        Bool Test(MsgpackFormat format);

        /// \brief Peek a single byte from the underlying stream and test it with the provided type using a mask.
        /// \return If the peeked byte matches the provided type, consume the byte and returns true, otherwise returns false.
        Bool Test(MsgpackFormat format, MsgpackFormatMask mask);

        /// \brief Read a packed value from the underlying stream.
        template <typename TType>
        TType Unpack(MsgpackFormat format);

        /// \brief Read a value from the underlying stream.
        template <typename TType>
        TType Read();

        /// \brief Read a value from the underlying stream.
        void Read(const MemoryRange& memory_range);

        /// \brief Underlying stream.
        TStream stream_;
    };

    /************************************************************************/
    /* MSGPACK READER SENTRY                                                */
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
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackReader.

    inline MsgpackReader::MsgpackReader(TString stream)
        : stream_(std::move(stream))
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

    template <typename TExtension>
    MsgpackReader& MsgpackReader::operator>>(TExtension& rhs)
    {
        using namespace Literals;

        using TMsgpackExtensionType = MsgpackExtensionType<TExtension>;

        auto sentry = MsgpackReaderSentry(stream_);

        // Type format, count and type-id.

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

        if (Test(TMsgpackExtensionType::GetType()))
        {
            TMsgpackExtensionType::Decode(static_cast<TInputStream&>(stream_), size, rhs);

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

    inline Bool MsgpackReader::Test(MsgpackFormat format)
    {
        if (ToByte(stream_.peek()) == ToByte(format))
        {
            Read<Fix8>();
            return true;
        }
        else
        {
            return false;
        }
    }

    inline Bool MsgpackReader::Test(MsgpackFormat format, MsgpackFormatMask mask)
    {
        if ((ToByte(stream_.peek()) & ToByte(mask)) == ToByte(format))
        {
            Read<Fix8>();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename TType>
    inline TType MsgpackReader::Unpack(MsgpackFormat format)
    {
        static_assert(sizeof(TType) == sizeof(Fix8), "Packed value require an 8-bit type");

        auto byte = Read<Fix8>();

        byte &= ~ToFix8(format);

        return static_cast<TType>(byte);
    }

    template <typename TType>
    inline TType MsgpackReader::Read()
    {
        auto buffer = TType{};

        stream_.read(reinterpret_cast<Byte*>(&buffer), sizeof(TType));

        return buffer;
    }

    inline void MsgpackReader::Read(const MemoryRange& rhs)
    {
        stream_.read(rhs.Begin().As<Byte>(), *rhs.GetSize());
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

