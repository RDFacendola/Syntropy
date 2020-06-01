
/// \file msgpack_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for streams encoding data using the Msgpack specification.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <limits>
#include <optional>

#include <cstdint>
#include "syntropy/containers/vector.h"
#include "syntropy/containers/map.h"
#include "syntropy/core/string.h"
#include "syntropy/serialization/msgpack/encoding.h"
#include "syntropy/serialization/msgpack/decoding.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/core/string_stream.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK STREAM                                                       */
    /************************************************************************/

    /// \brief Stream whose data are encoded via Msgpack specification.
    /// \author Raffaele D. Facendola - November 2019.
    /// \see https://github.com/msgpack/msgpack/blob/master/spec.md
    class MsgpackStream
    {
    public:

        /// \brief Type of the underlying string.
        using TString = BasicString<Byte>;

        /// \brief Type of the underlying stream.
        using TStream = BasicStringStream<Byte>;

        /// \brief Type of the underlying input stream.
        using TInputStream = std::basic_ostream<Byte>;

        /// \brief Type of the underlying output stream.
        using TOutputStream = std::basic_ostream<Byte>;

        /// \brief Create an empty stream.
        MsgpackStream() = default;

        /// \brief Create a stream.
        MsgpackStream(TString stream);

        /// \brief Insert a null value.
        MsgpackStream& operator<<(std::nullptr_t rhs);

        /// \brief Insert a boolean value.
        MsgpackStream& operator<<(Bool rhs);

        /// \brief Insert an 8-bit integer value.
        MsgpackStream& operator<<(Byte rhs);

        /// \brief Insert a 16-bit integer value.
        MsgpackStream& operator<<(std::int16_t rhs);

        /// \brief Insert a 32-bit integer value.
        MsgpackStream& operator<<(std::int32_t rhs);

        /// \brief Insert a 64-bit integer value.
        MsgpackStream& operator<<(Int rhs);

        /// \brief Insert a single precision floating point value.
        MsgpackStream& operator<<(Float rhs);

        /// \brief Insert a double precision floating point value.
        MsgpackStream& operator<<(double rhs);

        /// \brief Insert a string.
        MsgpackStream& operator<<(const String& rhs);

        /// \brief Insert a string.
        MsgpackStream& operator<<(const char* rhs);

        /// \brief Insert a byte-array.
        MsgpackStream& operator<<(const MemoryRange& rhs);

        /// \brief Insert a byte-array.
        MsgpackStream& operator<<(const ConstMemoryRange& rhs);

        /// \brief Insert an array.
        template <typename TElement>
        MsgpackStream& operator<<(const Vector<TElement>& rhs);

        /// \brief Insert a map.
        template <typename TKey, typename TValue>
        MsgpackStream& operator<<(const Map<TKey, TValue>& rhs);

        /// \brief Insert an extension-type value.
        template <typename TExtension>
        MsgpackStream& operator<<(const TExtension& rhs);

        /// \brief Extract a boolean value.
        MsgpackStream& operator>>(Bool& rhs);

        /// \brief Extract an 8-bit integer value.
        MsgpackStream& operator>>(Byte& rhs);

        /// \brief Extract a 16-bit integer value.
        MsgpackStream& operator>>(std::int16_t& rhs);

        /// \brief Extract a 32-bit integer value.
        MsgpackStream& operator>>(std::int32_t& rhs);

        /// \brief Extract a 64-bit integer value.
        MsgpackStream& operator>>(Int& rhs);

        /// \brief Extract a 8-bit unsigned integer value.
        /// Unsigned variant provided for external compatibility.
        MsgpackStream& operator>>(std::uint8_t& rhs);

        /// \brief Extract a 16-bit unsigned integer value.
        /// Unsigned variant provided for external compatibility.
        MsgpackStream& operator>>(std::uint16_t& rhs);

        /// \brief Extract a 32-bit unsigned integer value.
        /// Unsigned variant provided for external compatibility.
        MsgpackStream& operator>>(std::uint32_t& rhs);

        /// \brief Extract a 64-bit unsigned integer value.
        /// Unsigned variant provided for external compatibility.
        MsgpackStream& operator>>(std::uint64_t& rhs);

        /// \brief Extract a single precision floating point value.
        MsgpackStream& operator>>(Float& rhs);

        /// \brief Extract a double precision floating point value.
        MsgpackStream& operator>>(double& rhs);

        /// \brief Extract a string.
        MsgpackStream& operator>>(String& rhs);

        /// \brief Extract a byte-array.
        MsgpackStream& operator>>(MemoryRange& rhs);

        /// \brief Extract an array.
        template <typename TElement>
        MsgpackStream& operator>>(Vector<TElement>& rhs);

        /// \brief Extract a map.
        template <typename TKey, typename TValue>
        MsgpackStream& operator>>(Map<TKey, TValue>& rhs);

        /// \brief Extract an extension-type value.
        template <typename TExtension>
        MsgpackStream& operator>>(TExtension& rhs);

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

        /// \brief Clear the underlying stream.
        void Clear();

        /// \brief Access the underlying string.
        TString ToString() const;

    private:

        /// \brief Utility class used to rollback the status of the owning stream upon destruction unless dismissed.
        /// \author Raffaele D. Facendola - November 2019.
        class Sentry
        {
        public:

            /// \brief Create the sentry.
            Sentry(MsgpackStream& stream);

            /// \brief Destroy the sentry. If not dismissed rollback the status of the owning stream.
            ~Sentry();

            /// \brief Dismiss the sentry.
            void Dismiss();

        private:

            /// \brief Underlying stream.
            TStream& stream_;

            /// \brief Position of the stream upon sentry construction.
            std::optional<std::streampos> position_;

            /// \brief Whether the sentry was dismissed.
            Bool dismissed_{ false };

        };

        /// \brief Put a value inside the underlying stream.
        template <typename TType>
        void Put(TType value);

        /// \brief Put a buffer inside the underlying stream.
        void Put(const void* data, Int size);

        /// \brief Peek a single byte from the underlying stream.
        Byte Peek();

        /// \brief Peek a single byte from the underlying stream and test it with the provided type.
        /// \return If the peeked byte matches the provided type, consume the byte and returns true, otherwise returns false.
        Bool Test(MsgpackFormat type);

        /// \brief Get a value from the underlying stream.
        template <typename TType>
        TType Get();

        /// \brief Read data from the underlying stream.
        void Get(void* buffer, Int length);

        /// \brief Underlying stream.
        TStream stream_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackStream.

    inline MsgpackStream::MsgpackStream(TString stream)
        :stream_(std::move(stream))
    {

    }

    inline MsgpackStream& MsgpackStream::operator<<(std::nullptr_t rhs)
    {
        Put(Msgpack::Encode(rhs));

        return *this;
    }

    inline MsgpackStream& MsgpackStream::operator<<(Bool rhs)
    {
        Put(Msgpack::Encode(rhs));

        return *this;
    }

    inline MsgpackStream& MsgpackStream::operator<<(const char* rhs)
    {
        operator<<(String(rhs));

        return *this;
    }

    inline MsgpackStream& MsgpackStream::operator<<(Float rhs)
    {
        Put(MsgpackFormat::kFloat32);
        Put(Msgpack::Encode(rhs));

        return *this;
    }

    inline MsgpackStream& MsgpackStream::operator<<(double rhs)
    {
        Put(MsgpackFormat::kFloat64);
        Put(Msgpack::Encode(rhs));

        return *this;
    }

    inline MsgpackStream& MsgpackStream::operator<<(const MemoryRange& rhs)
    {
        return ((*this) << ConstMemoryRange(rhs));
    }

    template <typename TElement>
    MsgpackStream& MsgpackStream::operator<<(const Vector<TElement>& rhs)
    {
        if (Msgpack::IsFixArray(rhs))
        {
            Put(Msgpack::EncodeFixArrayLength(static_cast<Byte>(rhs.size())));
        }
        else if (Msgpack::IsArray16(rhs))
        {
            Put(MsgpackFormat::kArray16);
            Put(Msgpack::Encode(static_cast<std::int16_t>(rhs.size())));
        }
        else if (Msgpack::IsArray32(rhs))
        {
            Put(MsgpackFormat::kArray32);
            Put(Msgpack::Encode(static_cast<std::int32_t>(rhs.size())));
        }

        for (auto&& element : rhs)
        {
            operator<<(element);
        }

        return *this;
    }

    template <typename TKey, typename TValue>
    MsgpackStream& MsgpackStream::operator<<(const Map<TKey, TValue>& rhs)
    {
        if (Msgpack::IsFixMap(rhs))
        {
            Put(Msgpack::EncodeFixMapLength(static_cast<Byte>(rhs.size())));
        }
        else if (Msgpack::IsMap16(rhs))
        {
            Put(MsgpackFormat::kMap16);
            Put(Msgpack::Encode(static_cast<std::int16_t>(rhs.size())));
        }
        else if (Msgpack::IsMap32(rhs))
        {
            Put(MsgpackFormat::kMap32);
            Put(Msgpack::Encode(static_cast<std::int32_t>(rhs.size())));
        }

        for (auto&& element : rhs)
        {
            operator<<(element.first);
            operator<<(element.second);
        }

        return *this;
    }

    template <typename TExtension>
    MsgpackStream& MsgpackStream::operator<<(const TExtension& rhs)
    {
        using TMsgpackExtensionType = MsgpackExtensionType<TExtension>;

        auto size = *TMsgpackExtensionType::GetSize(rhs);

        // Format | (size) | Type | Data...

        if (Msgpack::IsFixExt1(rhs))
        {
            Put(MsgpackFormat::kFixExt1);
        }
        else if (Msgpack::IsFixExt2(rhs))
        {
            Put(MsgpackFormat::kFixExt2);
        }
        else if (Msgpack::IsFixExt4(rhs))
        {
            Put(MsgpackFormat::kFixExt4);
        }
        else if (Msgpack::IsFixExt8(rhs))
        {
            Put(MsgpackFormat::kFixExt8);
        }
        else if (Msgpack::IsFixExt16(rhs))
        {
            Put(MsgpackFormat::kFixExt16);
        }
        else if (Msgpack::IsExt8(rhs))
        {
            Put(MsgpackFormat::kExt8);
            Put(Msgpack::Encode(static_cast<Byte>(size)));
        }
        else if (Msgpack::IsExt16(rhs))
        {
            Put(MsgpackFormat::kExt16);
            Put(Msgpack::Encode(static_cast<std::int16_t>(size)));
        }
        else if (Msgpack::IsExt32(rhs))
        {
            Put(MsgpackFormat::kExt32);
            Put(Msgpack::Encode(static_cast<std::int32_t>(size)));
        }

        Put(Byte(TMsgpackExtensionType::GetType()));

        TMsgpackExtensionType::Encode(static_cast<TOutputStream&>(stream_), rhs);

        return *this;
    }

    template <typename TElement>
    MsgpackStream& MsgpackStream::operator>>(Vector<TElement>& rhs)
    {
        auto sentry = Sentry(*this);

        auto length = std::optional<Int>{};

        if (Msgpack::IsFixArrayFormat(Peek()))
        {
            length = Msgpack::DecodeFixArrayLength(Get<Byte>());
        }
        else if (Test(MsgpackFormat::kArray16))
        {
            length = Msgpack::Decode<std::int16_t>(Get<std::int16_t>());
        }
        else if (Test(MsgpackFormat::kArray32))
        {
            length = Msgpack::Decode<std::int32_t>(Get<std::int32_t>());
        }

        if (length)
        {
            rhs.clear();
            rhs.reserve(*length);

            for (auto elements = *length; elements > 0u; --elements)
            {
                auto element = TElement{};

                operator>>(element);

                rhs.emplace_back(std::move(element));
            }

            sentry.Dismiss();
        }

        return *this;
    }

    template <typename TKey, typename TValue>
    MsgpackStream& MsgpackStream::operator>>(Map<TKey, TValue>& rhs)
    {
        auto sentry = Sentry(*this);

        auto length = std::optional<Int>{};

        if (Msgpack::IsFixMapFormat(Peek()))
        {
            length = Msgpack::DecodeFixMapLength(Get<Byte>());
        }
        else if (Test(MsgpackFormat::kMap16))
        {
            length = Msgpack::Decode<std::int16_t>(Get<std::int16_t>());
        }
        else if (Test(MsgpackFormat::kMap32))
        {
            length = Msgpack::Decode<std::int32_t>(Get<std::int32_t>());
        }

        if (length)
        {
            rhs.clear();
            rhs.reserve(*length);

            for (auto elements = *length; elements > 0u; --elements)
            {
                auto key = TKey{};
                auto value = TValue{};

                operator>>(key);
                operator>>(value);

                rhs.emplace(std::make_pair(std::move(key), std::move(value)));
            }

            sentry.Dismiss();
        }

        return *this;
    }

    template <typename TExtension>
    MsgpackStream& MsgpackStream::operator>>(TExtension& rhs)
    {
        using TMsgpackExtensionType = MsgpackExtensionType<TExtension>;

        auto sentry = Sentry(*this);

        auto size = std::optional<Bytes>{};

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
            size = Bytes(Msgpack::Decode<Byte>(Get<Byte>()));
        }
        else if (Test(MsgpackFormat::kExt16))
        {
            size = Bytes(Msgpack::Decode<std::int16_t>(Get<std::int16_t>()));
        }
        else if (Test(MsgpackFormat::kExt32))
        {
            size = Bytes(Msgpack::Decode<std::int32_t>(Get<std::int32_t>()));
        }

        if (size && Byte(TMsgpackExtensionType::GetType()) == Get<Byte>())
        {
            TMsgpackExtensionType::Decode(static_cast<TInputStream&>(stream_), *size, rhs);

            sentry.Dismiss();
        }

        return *this;
    }

    inline Bool MsgpackStream::IsFail() const
    {
        return stream_.fail();
    }

    inline Bool MsgpackStream::IsEndOfStream() const
    {
        return stream_.eof();
    }

    inline void MsgpackStream::Recover()
    {
        stream_.clear();
    }

    inline void MsgpackStream::Clear()
    {
        stream_.str({});
    }

    inline Int MsgpackStream::GetReadPosition() const
    {
        return static_cast<Int>(stream_.rdbuf()->pubseekoff(0, std::ios::cur, std::ios::in));
    }

    inline MsgpackStream::TString MsgpackStream::ToString() const
    {
        return stream_.str();
    }

    template <typename TType>
    inline void MsgpackStream::Put(TType value)
    {
        if constexpr (sizeof(TType) == sizeof(Byte))
        {
            stream_.put(static_cast<Byte>(value));
        }
        else
        {
            stream_.write(reinterpret_cast<Byte*>(&value), sizeof(TType));
        }
    }

    inline void MsgpackStream::Put(const void* data, Int size)
    {
        stream_.write(reinterpret_cast<const Byte*>(data), size);
    }

    inline Byte MsgpackStream::Peek()
    {
        return Byte(stream_.peek());
    }

    inline Bool MsgpackStream::Test(MsgpackFormat type)
    {
        if (Peek() == Byte(type))
        {
            Get<Byte>();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename TType>
    inline TType MsgpackStream::Get()
    {
        if constexpr (sizeof(TType) == sizeof(Byte))
        {
            return TType(stream_.get());
        }
        else
        {
            auto buffer = TType{};

            stream_.read(reinterpret_cast<Byte*>(&buffer), sizeof(TType));

            return buffer;
        }
    }

    inline void MsgpackStream::Get(void* buffer, Int length)
    {
        stream_.read(reinterpret_cast<Byte*>(buffer), length);
    }

    // MsgpackStream :: Sentry.

    inline MsgpackStream::Sentry::Sentry(MsgpackStream& stream)
        : stream_(stream.stream_)
    {
        if (!stream_.fail())
        {
            position_ = stream.stream_.tellg();
        }
    }

    inline MsgpackStream::Sentry::~Sentry()
    {
        // If the failbit is set an underlying read failed and wrong data were decoded: rollback!

        if ((!dismissed_ || stream_.fail()) && position_)
        {
            auto state = stream_.rdstate() | std::ios::failbit;

            stream_.clear();
            stream_.seekg(*position_);

            stream_.setstate(state);
        }
    }

    inline void MsgpackStream::Sentry::Dismiss()
    {
        dismissed_ = true;
    }

}

