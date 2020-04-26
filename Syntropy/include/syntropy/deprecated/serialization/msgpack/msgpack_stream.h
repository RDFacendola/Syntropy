
/// \file msgpack_stream.h
/// \brief This header is part of the syntropy serialization system. It contains definition for streams encoding data using the msgpack specification.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>
#include "syntropy/containers/vector.h"
#include <limits>
#include "syntropy/types/string.h"
#include "syntropy/containers/map.h"
#include <optional>

#include "syntropy/serialization/msgpack/msgpack.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/types/sstream.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK STREAM                                                       */
    /************************************************************************/

    /// \brief Stream whose data are encoded via msgpack specification.
    /// \author Raffaele D. Facendola - November 2019.
    /// \see https://github.com/msgpack/msgpack/blob/master/spec.md
    class MsgpackStream
    {
    public:

        /// \brief Type of the underlying string.
        using TString = BasicString<std::int8_t>;

        /// \brief Type of the underlying stream.
        using TStream = BasicStringStream<std::int8_t>;

        /// \brief Type of the underlying input stream.
        using TInputStream = std::basic_ostream<std::int8_t>;

        /// \brief Type of the underlying output stream.
        using TOutputStream = std::basic_ostream<std::int8_t>;

        /// \brief Create an empty stream.
        MsgpackStream() = default;

        /// \brief Create a stream.
        MsgpackStream(TString stream);

        /// \brief Insert a null value.
        MsgpackStream& operator<<(std::nullptr_t rhs);

        /// \brief Insert a boolean value.
        MsgpackStream& operator<<(bool rhs);

        /// \brief Insert an 8-bit integer value.
        MsgpackStream& operator<<(std::int8_t rhs);

        /// \brief Insert a 16-bit integer value.
        MsgpackStream& operator<<(std::int16_t rhs);

        /// \brief Insert a 32-bit integer value.
        MsgpackStream& operator<<(std::int32_t rhs);

        /// \brief Insert a 64-bit integer value.
        MsgpackStream& operator<<(std::int64_t rhs);

        /// \brief Insert an 8-bit unsigned integer value.
        MsgpackStream& operator<<(std::uint8_t rhs);

        /// \brief Insert a 16-bit unsigned integer value.
        MsgpackStream& operator<<(std::uint16_t rhs);

        /// \brief Insert a 32-bit unsigned integer value.
        MsgpackStream& operator<<(std::uint32_t rhs);

        /// \brief Insert a 64-bit unsigned integer value.
        MsgpackStream& operator<<(std::uint64_t rhs);

        /// \brief Insert a single precision floating point value.
        MsgpackStream& operator<<(float rhs);

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
        MsgpackStream& operator>>(bool& rhs);

        /// \brief Extract an 8-bit integer value.
        MsgpackStream& operator>>(std::int8_t& rhs);

        /// \brief Extract a 16-bit integer value.
        MsgpackStream& operator>>(std::int16_t& rhs);

        /// \brief Extract a 32-bit integer value.
        MsgpackStream& operator>>(std::int32_t& rhs);

        /// \brief Extract a 64-bit integer value.
        MsgpackStream& operator>>(std::int64_t& rhs);

        /// \brief Extract a 8-bit unsigned integer value.
        MsgpackStream& operator>>(std::uint8_t& rhs);

        /// \brief Extract a 16-bit unsigned integer value.
        MsgpackStream& operator>>(std::uint16_t& rhs);

        /// \brief Extract a 32-bit unsigned integer value.
        MsgpackStream& operator>>(std::uint32_t& rhs);

        /// \brief Extract a 64-bit unsigned integer value.
        MsgpackStream& operator>>(std::uint64_t& rhs);

        /// \brief Extract a single precision floating point value.
        MsgpackStream& operator>>(float& rhs);

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
        bool IsFail() const;

        /// \brief Check whether the last byte in the stream was read.
        /// \return Returns true if the last byte in the stream was read, returns false otherwise.
        bool IsEndOfStream() const;

        /// \brief Clear the status of the stream after a reading error.
        void Recover();

        /// \brief Get the index of the next byte to read.
        std::size_t GetReadPosition() const;

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
            bool dismissed_{ false };

        };

        /// \brief Put a value inside the underlying stream.
        template <typename TType>
        void Put(TType value);

        /// \brief Put a buffer inside the underlying stream.
        void Put(const void* data, std::size_t size);

        /// \brief Peek a single byte from the underlying stream.
        std::int8_t Peek();

        /// \brief Peek a single byte from the underlying stream and test it with the provided type.
        /// \return If the peeked byte matches the provided type, consume the byte and returns true, otherwise returns false.
        bool Test(MsgpackFormat type);

        /// \brief Get a value from the underlying stream.
        template <typename TType>
        TType Get();

        /// \brief Read data from the underlying stream.
        void Get(void* buffer, std::size_t length);

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

    inline MsgpackStream& MsgpackStream::operator<<(bool rhs)
    {
        Put(Msgpack::Encode(rhs));

        return *this;
    }

    inline MsgpackStream& MsgpackStream::operator<<(const char* rhs)
    {
        operator<<(String(rhs));

        return *this;
    }

    inline MsgpackStream& MsgpackStream::operator<<(float rhs)
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
            Put(Msgpack::EncodeFixArrayLength(std::uint8_t(rhs.size())));
        }
        else if (Msgpack::IsArray16(rhs))
        {
            Put(MsgpackFormat::kArray16);
            Put(Msgpack::Encode(std::uint16_t(rhs.size())));
        }
        else if (Msgpack::IsArray32(rhs))
        {
            Put(MsgpackFormat::kArray32);
            Put(Msgpack::Encode(std::uint32_t(rhs.size())));
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
            Put(Msgpack::EncodeFixMapLength(std::uint8_t(rhs.size())));
        }
        else if (Msgpack::IsMap16(rhs))
        {
            Put(MsgpackFormat::kMap16);
            Put(Msgpack::Encode(std::uint16_t(rhs.size())));
        }
        else if (Msgpack::IsMap32(rhs))
        {
            Put(MsgpackFormat::kMap32);
            Put(Msgpack::Encode(std::uint32_t(rhs.size())));
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

        auto size = std::size_t(TMsgpackExtensionType::GetSize(rhs));

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
            Put(Msgpack::Encode(std::uint8_t(size)));
        }
        else if (Msgpack::IsExt16(rhs))
        {
            Put(MsgpackFormat::kExt16);
            Put(Msgpack::Encode(std::uint16_t(size)));
        }
        else if (Msgpack::IsExt32(rhs))
        {
            Put(MsgpackFormat::kExt32);
            Put(Msgpack::Encode(std::uint32_t(size)));
        }

        Put(std::int8_t(TMsgpackExtensionType::GetType()));

        TMsgpackExtensionType::Encode(static_cast<TOutputStream&>(stream_), rhs);

        return *this;
    }

    template <typename TElement>
    MsgpackStream& MsgpackStream::operator>>(Vector<TElement>& rhs)
    {
        auto sentry = Sentry(*this);

        auto length = std::optional<std::size_t>{};

        if (Msgpack::IsFixArrayFormat(Peek()))
        {
            length = Msgpack::DecodeFixArrayLength(Get<std::int8_t>());
        }
        else if (Test(MsgpackFormat::kArray16))
        {
            length = Msgpack::Decode<std::uint16_t>(Get<std::int16_t>());
        }
        else if (Test(MsgpackFormat::kArray32))
        {
            length = Msgpack::Decode<std::uint32_t>(Get<std::int32_t>());
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

        auto length = std::optional<std::size_t>{};

        if (Msgpack::IsFixMapFormat(Peek()))
        {
            length = Msgpack::DecodeFixMapLength(Get<std::int8_t>());
        }
        else if (Test(MsgpackFormat::kMap16))
        {
            length = Msgpack::Decode<std::uint16_t>(Get<std::int16_t>());
        }
        else if (Test(MsgpackFormat::kMap32))
        {
            length = Msgpack::Decode<std::uint32_t>(Get<std::int32_t>());
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
            size = Bytes(Msgpack::Decode<std::uint8_t>(Get<std::int8_t>()));
        }
        else if (Test(MsgpackFormat::kExt16))
        {
            size = Bytes(Msgpack::Decode<std::uint16_t>(Get<std::int16_t>()));
        }
        else if (Test(MsgpackFormat::kExt32))
        {
            size = Bytes(Msgpack::Decode<std::uint32_t>(Get<std::int32_t>()));
        }

        if (size && std::int8_t(TMsgpackExtensionType::GetType()) == Get<std::int8_t>())
        {
            TMsgpackExtensionType::Decode(static_cast<TInputStream&>(stream_), *size, rhs);

            sentry.Dismiss();
        }

        return *this;
    }

    inline bool MsgpackStream::IsFail() const
    {
        return stream_.fail();
    }

    inline bool MsgpackStream::IsEndOfStream() const
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

    inline std::size_t MsgpackStream::GetReadPosition() const
    {
        return stream_.rdbuf()->pubseekoff(0, std::ios::cur, std::ios::in);
    }

    inline MsgpackStream::TString MsgpackStream::ToString() const
    {
        return stream_.str();
    }

    template <typename TType>
    inline void MsgpackStream::Put(TType value)
    {
        if constexpr (sizeof(TType) == sizeof(std::int8_t))
        {
            stream_.put(std::int8_t(value));
        }
        else
        {
            stream_.write(reinterpret_cast<std::int8_t*>(&value), sizeof(TType));
        }
    }

    inline void MsgpackStream::Put(const void* data, std::size_t size)
    {
        stream_.write(reinterpret_cast<const std::int8_t*>(data), size);
    }

    inline std::int8_t MsgpackStream::Peek()
    {
        return std::int8_t(stream_.peek());
    }

    inline bool MsgpackStream::Test(MsgpackFormat type)
    {
        if (Peek() == std::int8_t(type))
        {
            Get<std::int8_t>();
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
        if constexpr (sizeof(TType) == sizeof(std::int8_t))
        {
            return TType(stream_.get());
        }
        else
        {
            auto buffer = TType{};

            stream_.read(reinterpret_cast<std::int8_t*>(&buffer), sizeof(TType));

            return buffer;
        }
    }

    inline void MsgpackStream::Get(void* buffer, std::size_t length)
    {
        stream_.read(reinterpret_cast<std::int8_t*>(buffer), length);
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

