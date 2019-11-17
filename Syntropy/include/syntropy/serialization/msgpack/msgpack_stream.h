
/// \file msgpack_stream.h
/// \brief This header is part of the syntropy serialization system. It contains definition for streams encoding data using the msgpack specification.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>
#include <vector>
#include <limits>
#include <string>
#include <unordered_map>
#include <optional>
#include <sstream>

#include "syntropy/serialization/msgpack/msgpack.h"

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

        /// \brief Create an empty stream.
        MsgpackStream() = default;

        /// \brief Create a stream.
        MsgpackStream(std::string stream);

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
        MsgpackStream& operator<<(const std::string& rhs);

        /// \brief Insert a string.
        MsgpackStream& operator<<(const char* rhs);

        /// \brief Insert a byte-array.
        MsgpackStream& operator<<(const std::vector<std::uint8_t>& rhs);

        /// \brief Insert an array.
        template <typename TElement>
        MsgpackStream& operator<<(const std::vector<TElement>& rhs);

        /// \brief Insert a map.
        template <typename TKey, typename TValue>
        MsgpackStream& operator<<(const std::unordered_map<TKey, TValue>& rhs);

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
        MsgpackStream& operator>>(std::string& rhs);

        /// \brief Extract an array.
        template <typename TElement>
        MsgpackStream& operator>>(std::vector<TElement>& rhs);

        /// \brief Extract a map.
        template <typename TKey, typename TValue>
        MsgpackStream& operator>>(std::unordered_map<TKey, TValue>& rhs);

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

        /// \brief Access the underlying stream.
        std::string ToString() const;

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
            std::stringstream& stream_;

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

        /// \brief Get a single byte from the underlying stream.
        std::int8_t Get();

        /// \brief Peek a single byte from the underlying stream.
        std::int8_t Peek();

        /// \brief Peek a single byte from the underlying stream and test it with the provided type.
        /// \return If the peeked byte matches the provided type, consume the byte and returns true, otherwise returns false.
        bool Test(MsgpackFormat type);

        /// \brief Get a value from the underlying stream.
        template <typename TType>
        void Get(TType& value);

        /// \brief Read data from the underlying stream.
        void Get(void* buffer, std::size_t length);

        /// \brief Underlying stream.
        std::stringstream stream_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackStream.

    inline MsgpackStream::MsgpackStream(std::string stream)
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
        operator<<(std::string(rhs));

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

    template <typename TElement>
    MsgpackStream& MsgpackStream::operator<<(const std::vector<TElement>& rhs)
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
    MsgpackStream& MsgpackStream::operator<<(const std::unordered_map<TKey, TValue>& rhs)
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

    template <typename TElement>
    MsgpackStream& MsgpackStream::operator>>(std::vector<TElement>& rhs)
    {
        auto sentry = Sentry(*this);

        auto length = std::optional<std::size_t>{};

        if (Msgpack::IsFixArrayFormat(Peek()))
        {
            length = Msgpack::DecodeFixArrayLength(Get());
        }
        else if (Test(MsgpackFormat::kArray16))
        {
            auto length_encoded = std::int16_t{};
            Get(length_encoded);
            length = Msgpack::DecodeUInt16(length_encoded);
        }
        else if (Test(MsgpackFormat::kArray32))
        {
            auto length_encoded = std::int32_t{};
            Get(length_encoded);
            length = Msgpack::DecodeUInt32(length_encoded);
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
    MsgpackStream& MsgpackStream::operator>>(std::unordered_map<TKey, TValue>& rhs)
    {
        auto sentry = Sentry(*this);

        auto length = std::optional<std::size_t>{};

        if (Msgpack::IsFixMapFormat(Peek()))
        {
            length = Msgpack::DecodeFixMapLength(Get());
        }
        else if (Test(MsgpackFormat::kMap16))
        {
            auto length_encoded = std::int16_t{};
            Get(length_encoded);
            length = Msgpack::DecodeUInt16(length_encoded);
        }
        else if (Test(MsgpackFormat::kMap32))
        {
            auto length_encoded = std::int32_t{};
            Get(length_encoded);
            length = Msgpack::DecodeUInt32(length_encoded);
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
        stream_.str("");
    }

    inline std::size_t MsgpackStream::GetReadPosition() const
    {
        return stream_.rdbuf()->pubseekoff(0, std::ios::cur, std::ios::in);
    }

    inline std::string MsgpackStream::ToString() const
    {
        return stream_.str();
    }

    template <typename TType>
    inline void MsgpackStream::Put(TType value)
    {
        if constexpr (sizeof(TType) == sizeof(char))
        {
            stream_.put(char(value));
        }
        else
        {
            stream_.write(reinterpret_cast<char*>(&value), sizeof(TType));
        }
    }

    inline void MsgpackStream::Put(const void* data, std::size_t size)
    {
        stream_.write(reinterpret_cast<const char*>(data), size);
    }

    inline std::int8_t MsgpackStream::Get()
    {
        return std::int8_t(stream_.get());
    }

    inline std::int8_t MsgpackStream::Peek()
    {
        return std::int8_t(stream_.peek());
    }

    inline bool MsgpackStream::Test(MsgpackFormat type)
    {
        if (Peek() == std::int8_t(type))
        {
            Get();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename TType>
    inline void MsgpackStream::Get(TType& value)
    {
        if constexpr (sizeof(TType) == sizeof(char))
        {
            value = TType(stream_.get());
        }
        else
        {
            stream_.read(reinterpret_cast<char*>(&value), sizeof(TType));
        }
    }

    inline void MsgpackStream::Get(void* buffer, std::size_t length)
    {
        stream_.read(reinterpret_cast<char*>(buffer), length);
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

