
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

#include "syntropy/serialization/msgpack/msgpack.h"

#include "syntropy/platform/endianness.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK STREAM                                                       */
    /************************************************************************/

    /// \brief Stream encoding data using msgpack specification.
    /// Specification: https://github.com/msgpack/msgpack/blob/master/spec.md
    /// \author Raffaele D. Facendola - November 2019.
    class MsgpackStream
    {
    public:

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
        MsgpackStream& operator<<(const char* rhs);

        /// \brief Insert a string.
        MsgpackStream& operator<<(const std::string& rhs);

        /// \brief Insert a byte-array.
        MsgpackStream& operator<<(const std::vector<std::uint8_t>& rhs);

        /// \brief Insert an array.
        template <typename TElement>
        MsgpackStream& operator<<(const std::vector<TElement>& rhs);

        /// \brief Insert a map.
        template <typename TKey, typename TValue>
        MsgpackStream& operator<<(const std::unordered_map<TKey, TValue>& rhs);

        /// \brief Extract a boolean value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(bool& rhs);

        /// \brief Extract an 8-bit integer value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::int8_t& rhs);

        /// \brief Extract a 16-bit integer value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::int16_t& rhs);

        /// \brief Extract a 32-bit integer value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::int32_t& rhs);

        /// \brief Extract a 64-bit integer value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::int64_t& rhs);

        /// \brief Extract a 8-bit unsigned integer value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::uint8_t& rhs);

        /// \brief Extract a 16-bit unsigned integer value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::uint16_t& rhs);

        /// \brief Extract a 32-bit unsigned integer value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::uint32_t& rhs);

        /// \brief Extract a 64-bit unsigned integer value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::uint64_t& rhs);

        /// \brief Extract a single precision floating point value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(float& rhs);

        /// \brief Extract a double precision floating point value.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(double& rhs);

        /// \brief Extract a string.
        /// If the read operation succeeded rhs contains the extracted value, otherwise a read error is signaled.
        MsgpackStream& operator>>(std::string& rhs);

        /// \brief Extract an array.
        template <typename TElement>
        MsgpackStream& operator>>(std::vector<TElement>& rhs);

        /// \brief Extract a map.
        template <typename TKey, typename TValue>
        MsgpackStream& operator>>(std::unordered_map<TKey, TValue>& rhs);

        /// \brief Check whether the last read operation failed.
        /// \return Returns true if the last read operation failed, returns false otherwise
        bool IsReadFail() const;

        /// \brief Check whether the last byte in the stream was read.
        /// \return Returns true if the last byte in the stream was read, returns false otherwise.
        bool IsEndOfStream() const;

        /// \brief Clear the status of the stream after a reading error.
        void Recover();

    private:

        /// \brief Write a value that can be implicitly converted to a 8-bit unsigned integer to the underlying buffer.
        template <typename TValue>
        MsgpackStream& WriteByte(TValue value);

        /// \brief Write a value to the underlying buffer.
        template <typename TValue>
        MsgpackStream& WriteBytes(TValue value);

        /// \brief Write a buffer to the underlying buffer.
        MsgpackStream& WriteBytes(const void* buffer, std::size_t length);

        /// \brief Read a byte from the underlying buffer.
        template <typename TValue>
        MsgpackStream& ReadByte(TValue& value);

        /// \brief Read many bytes from the underlying buffer.
        template <typename TValue>
        MsgpackStream& ReadBytes(TValue& value);

        /// \brief Read data from the underlying buffer.
        MsgpackStream& ReadBytes(void* buffer, std::size_t length);

        /// \brief Notify a read failure.
        void SetReadFail();

        /// \brief Check whether the next unread byte matches a given value.
        /// \param value Value to test the next byte to read against.
        /// \param mask Mask used while performing the test.
        /// \return If the test succeeds returns the next value and advances the cursor, otherwise returns false.
        template <typename TValue, typename TMask>
        std::optional<std::uint8_t> TestByte(TValue value, TMask mask);

        /// \brief Check whether the next unread byte matches a given value.
        /// \param value Value to test the next byte to read against.
        /// \return If the test succeeds returns the next value and advances the cursor, otherwise returns false.
        template <typename TValue>
        std::optional<std::uint8_t> TestByte(TValue value);

        /// \brief Underlying buffer.
        std::vector<std::uint8_t> buffer_;

        /// \brief Whether the last read operation failed.
        bool read_fail_{ false };

        /// \brief Index of the next byte to read.
        std::size_t cursor_{ 0 };
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackStream.

    inline MsgpackStream& MsgpackStream::operator<<(std::nullptr_t rhs)
    {
        return WriteByte(MsgpackFormat::kNil);
    }

    inline MsgpackStream& MsgpackStream::operator<<(bool rhs)
    {
        return WriteByte(rhs ? MsgpackFormat::kTrue : MsgpackFormat::kFalse);
    }

    inline MsgpackStream& MsgpackStream::operator<<(const char* rhs)
    {
        return (*this << std::string(rhs));
    }

    template <typename TElement>
    MsgpackStream& MsgpackStream::operator<<(const std::vector<TElement>& rhs)
    {
        auto count = rhs.size();
        auto capacity = buffer_.size() + count * sizeof(TElement);

        // Header.

        if (count <= 15)
        {
            buffer_.reserve(capacity + 1);

            WriteByte(static_cast<std::uint8_t>(MsgpackFormat::kFixArray) | std::uint8_t(count));
        }
        else if (count <= ((1ull << 16u) - 1u))
        {
            buffer_.reserve(capacity + 3);

            WriteByte(MsgpackFormat::kArray16).WriteBytes(Endianness::ToBigEndian(std::uint16_t(count)));
        }
        else if (count <= ((1ull << 32u) - 1u))
        {
            buffer_.reserve(capacity + 5);

            WriteByte(MsgpackFormat::kArray32).WriteBytes(Endianness::ToBigEndian(std::uint32_t(count)));
        }

        // Insert each element.

        for (auto&& element : rhs)
        {
            *this << element;
        }

        return *this;
    }

    template <typename TKey, typename TValue>
    MsgpackStream& MsgpackStream::operator<<(const std::unordered_map<TKey, TValue>& rhs)
    {
        auto count = rhs.size();
        auto capacity = buffer_.size() + count * (sizeof(TKey) + sizeof(TValue));

        // Header.

        if (count <= 15)
        {
            buffer_.reserve(capacity + 1);

            WriteByte(static_cast<std::uint8_t>(MsgpackFormat::kFixMap) | std::uint8_t(count));
        }
        else if (count <= ((1ull << 16u) - 1u))
        {
            buffer_.reserve(capacity + 3);

            WriteByte(MsgpackFormat::kMap16).WriteBytes(Endianness::ToBigEndian(std::uint16_t(count)));
        }
        else if (count <= ((1ull << 32u) - 1u))
        {
            buffer_.reserve(capacity + 5);

            WriteByte(MsgpackFormat::kMap32).WriteBytes(Endianness::ToBigEndian(std::uint32_t(count)));
        }

        // Insert each element.

        for (auto&& element : rhs)
        {
            *this << element.first << element.second;
        }

        return *this;
    }

    template <typename TElement>
    MsgpackStream& MsgpackStream::operator>>(std::vector<TElement>& rhs)
    {
        auto old_cursor = cursor_;

        // Header.

        auto elements = std::uint32_t{};

        if (auto FixArray = TestByte(MsgpackFormat::kFixArray, MsgpackFixTypeMask::kFixArray))
        {
            elements = std::uint32_t((*FixArray) & ~static_cast<std::uint8_t>(MsgpackFixTypeMask::kFixArray));
        }
        else if (TestByte(MsgpackFormat::kArray16))
        {
            auto elements_low = std::uint16_t{};

            ReadByte(elements_low);

            elements = std::uint32_t(elements_low);
        }
        else if (TestByte(MsgpackFormat::kArray32))
        {
            auto elements_low = std::uint32_t{};

            ReadByte(elements_low);

            elements = std::uint32_t(Endianness::FromBigEndian(elements_low));
        }
        else
        {
            SetReadFail();

            return *this;
        }

        // Data.

        rhs.clear();
        rhs.reserve(elements);

        for (; elements > 0; --elements)
        {
            if(auto element = TElement{}; !(*this >> element).IsReadFail())
            {
                rhs.emplace_back(std::move(element));
            }
            else
            {
                break;
            }
        }

        if (IsReadFail())
        {
            cursor_ = old_cursor;       // Rollback!
        }

        return *this;
    }

    template <typename TKey, typename TValue>
    MsgpackStream& MsgpackStream::operator>>(std::unordered_map<TKey, TValue>& rhs)
    {
        auto old_cursor = cursor_;

        // Header.

        auto elements = std::uint32_t{};

        if (auto FixMap = TestByte(MsgpackFormat::kFixMap, MsgpackFixTypeMask::kFixMap))
        {
            elements = std::uint32_t((*FixMap) & ~static_cast<std::uint8_t>(MsgpackFixTypeMask::kFixMap));
        }
        else if (TestByte(MsgpackFormat::kMap16))
        {
            auto elements_low = std::uint16_t{};

            ReadByte(elements_low);

            elements = std::uint32_t(elements_low);
        }
        else if (TestByte(MsgpackFormat::kMap32))
        {
            auto elements_low = std::uint32_t{};

            ReadByte(elements_low);

            elements = std::uint32_t(Endianness::FromBigEndian(elements_low));
        }
        else
        {
            SetReadFail();

            return *this;
        }

        // Data.

        rhs.clear();
        rhs.reserve(elements);

        for (; elements > 0; --elements)
        {
            auto key = TKey{};
            auto value = TValue{};

            if(!(*this >> key >> value).IsReadFail())
            {
                rhs.emplace(std::make_pair(key, value));
            }
            else
            {
                break;
            }
        }

        if (IsReadFail())
        {
            cursor_ = old_cursor;       // Rollback!
        }

        return *this;
    }

    inline bool MsgpackStream::IsReadFail() const
    {
        return read_fail_;
    }

    inline bool MsgpackStream::IsEndOfStream() const
    {
        return cursor_ >= buffer_.size();
    }

    inline void MsgpackStream::Recover()
    {
        read_fail_ = false;
    }

    template <typename TValue>
    inline MsgpackStream& MsgpackStream::WriteByte(TValue value)
    {
        buffer_.emplace_back(static_cast<std::uint8_t>(value));
        return *this;
    }

    template <typename TValue>
    inline MsgpackStream& MsgpackStream::WriteBytes(TValue value)
    {
        if constexpr (sizeof(value) == sizeof(std::uint8_t))
        {
            return WriteByte(value);
        }
        else
        {
            return WriteBytes(&value, sizeof(value));
        }
    }

    inline MsgpackStream& MsgpackStream::WriteBytes(const void* buffer, std::size_t length)
    {
        auto cursor = buffer_.size();

        buffer_.resize(cursor + length);

        std::memcpy(buffer_.data() + cursor, buffer, length);

        return *this;
    }

    template <typename TValue>
    inline MsgpackStream& MsgpackStream::ReadByte(TValue& value)
    {
        if (cursor_ + 1 <= buffer_.size())
        {
            value = TValue(buffer_[cursor_++]);
        }
        else
        {
            SetReadFail();
        }

        return *this;
    }

    template <typename TValue>
    inline MsgpackStream& MsgpackStream::ReadBytes(TValue& value)
    {
        if (cursor_ + sizeof(TValue) <= buffer_.size())
        {
            std::memcpy(&value, buffer_.data() + cursor_, sizeof(TValue));

            cursor_ += sizeof(TValue);
        }
        else
        {
            SetReadFail();
        }

        return *this;
    }

    inline MsgpackStream& MsgpackStream::ReadBytes(void* buffer, std::size_t length)
    {
        if (!IsReadFail())
        {
            if (cursor_ + length <= buffer_.size())
            {
                std::memcpy(buffer, buffer_.data() + cursor_, length);

                cursor_ += length;
            }
            else
            {
                SetReadFail();          // Not enough data.
            }
        }

        return *this;
    }

    inline void MsgpackStream::SetReadFail()
    {
        read_fail_ = true;
    }

    template <typename TValue, typename TMask>
    inline std::optional<std::uint8_t> MsgpackStream::TestByte(TValue value, TMask mask)
    {
        auto value_byte = static_cast<std::uint8_t>(value);
        auto mask_byte = static_cast<std::uint8_t>(mask);

        if (!IsReadFail() && !IsEndOfStream() && ((buffer_[cursor_] & mask_byte) == (value_byte & mask_byte)))
        {
            return buffer_[cursor_++];
        }

        return {};
    }

    template <typename TValue>
    inline std::optional<std::uint8_t> MsgpackStream::TestByte(TValue value)
    {
        return TestByte(value, -1);
    }
}

