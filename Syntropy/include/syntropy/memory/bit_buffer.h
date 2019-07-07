
/// \file bit_buffer.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of classes used to handle bit-addressed buffers.
///
/// \author Raffaele D. Facendola - June 2019

#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "syntropy/diagnostics/assert.h"
#include "syntropy/memory/bit.h"
#include "syntropy/memory/memory_address.h"

namespace syntropy
{
    /************************************************************************/
    /* BIT BUFFER                                                           */
    /************************************************************************/

    /// \brief Represents a raw sequence of bits.
    /// \author Raffaele D. Facendola - June 2019.
    class BitBuffer
    {
    public:

        /// \brief Create an empty buffer.
        BitBuffer() = default;

        /// \brief Default copy constructor.
        BitBuffer(const BitBuffer&) = default;

        /// \brief Default move constructor.
        BitBuffer(BitBuffer&&) = default;

        /// \brief Create a new buffer copying an existing memory region.
        BitBuffer(ConstMemoryAddress address, Bits size);

        /// \brief Create a new buffer using an explicit value.
        template <typename TType>
        BitBuffer(const TType& value);

        /// \brief Default copy assignment operator.
        BitBuffer& operator=(const BitBuffer&) = default;

        /// \brief Default move assignment operator.
        BitBuffer& operator=(BitBuffer&&) = default;

        /// \brief Bitwise-and operator. Operators are padded with trailing zeros.
        /// \return Returns a reference to this element.
        BitBuffer& operator&=(const BitBuffer& rhs);

        /// \brief Bitwise-or operator. Operators are padded with trailing zeros.
        /// \return Returns a reference to this element.
        BitBuffer& operator|=(const BitBuffer& rhs);

        /// \brief Bitwise-xor operator. Operators are padded with trailing zeros.
        /// \return Returns a reference to this element.
        BitBuffer& operator^=(const BitBuffer& rhs);

        /// \brief Bitwise-not operator.
        BitBuffer operator~() const;

        /// \brief Access the buffer data.
        ConstMemoryAddress GetData() const;

        /// \brief Access the buffer data.
        MemoryAddress GetData();

        /// \brief Get the size of the buffer, in bits.
        Bits GetSize() const;

        /// \brief Reserve some memory, but leaves the current content of the buffer unchanged.
        /// \param capacity Memory to reserve. If this value is less than the current size this method behaves as no-op.
        void Reserve(Bits capacity);

        /// \brief Resize the buffer.
        /// \remarks If the new size is greater than the current one, the exceeding bits are zero-initialized.
        void Resize(Bits size);

        /// \brief Read a value at given position.
        /// \remarks Reading past the end of the buffer behaves as if the buffer ended with a trail of zeroes.
        template <typename TType>
        TType Read(Bits position) const;

        /// \brief Write a value at given position, overwriting existing bits and resizing the buffer if necessary.
        template <typename TType>
        void Write(Bits position, const TType& value);

        /// \brief Append a value at the end of the buffer.
        template <typename TType>
        void Append(const TType& value);

    private:

        /// \brief Perform a binary word-wise operation on this buffer and rhs.
        /// This buffer is resized to maximum size between this and rhs.
        template <typename TOp>
        BitBuffer& BinaryOp(const BitBuffer& rhs, TOp operation);

        /// \brief Perform a unary word-wise operation on this buffer.
        /// This buffer is resized to maximum size between this and rhs.
        template <typename TOp>
        BitBuffer& UnaryOp(TOp operation);

        /// \brief Sanitize the end of the buffer with trailing zeroes.
        void Sanitize();

        /// \brief Buffer data.
        std::vector<uint8_t> data_;

        /// \brief Size of the buffer.
        Bits size_ = 0_Bits;

    };

    /// \brief Bitwise and between two bit-buffers. Operands are padded with trailing zeros.
    /// \return Returns a bit-buffer which is equal to the bitwise and between lhs and rhs.
    BitBuffer operator&(const BitBuffer& lhs, const BitBuffer& rhs);

    /// \brief Bitwise or between two bit-buffers. Operands are padded with trailing zeros.
    /// \return Returns a bit-buffer  amount which is equal to the bitwise or between lhs and rhs.
    BitBuffer operator|(const BitBuffer& lhs, const BitBuffer& rhs);

    /// \brief Bitwise xor between two bit-buffers. Operands are padded with trailing zeros.
    /// \return Returns a bit-buffer  amount which is equal to the bitwise xor between lhs and rhs.
    BitBuffer operator^(const BitBuffer& lhs, const BitBuffer& rhs);

    /************************************************************************/
    /* BIT BUFFER VIEW                                                      */
    /************************************************************************/

    /// \brief Represents a view to a BitBuffer.
    /// \author Raffaele D. Facendola - June 2019.
    class BitBufferView
    {
    public:

        /// \brief Create a new view to a buffer.
        BitBufferView(const BitBuffer& buffer);

        /// \brief A view to a temporary buffer is not allowed.
        BitBufferView(const BitBuffer&&) = delete;

        /// \brief Create a new view to a buffer, starting at a given offset.
        BitBufferView(const BitBuffer& buffer, Bits offset);

        /// \brief A view to a temporary buffer is not allowed.
        BitBufferView(const BitBuffer&&, Bits) = delete;

        /// \brief Create a new view to a buffer, starting at a given offset and with an explicit size.
        BitBufferView(const BitBuffer& buffer, Bits offset, Bits size);

        /// \brief A view to a temporary buffer is not allowed.
        BitBufferView(const BitBuffer&&, Bits, Bits) = delete;

        /// \brief Default copy constructor.
        BitBufferView(const BitBufferView& rhs) = default;

        /// \brief Create a new view to a buffer, starting at a given offset.
        BitBufferView(const BitBufferView& buffer_view, Bits offset);

        /// \brief Create a new view to a buffer, starting at a given offset and with an explicit size.
        BitBufferView(const BitBufferView& buffer_view, Bits offset, Bits size);

        /// \brief Access the buffer data.
        ConstMemoryBitAddress GetData() const;

        /// \brief Get the size of the buffer, in bits.
        Bits GetSize() const;

        /// \brief Read a value at given position.
        /// \remarks Reading past the end of the buffer behaves as if the buffer ended with a trail of zeroes.
        template <typename TType>
        TType Read(Bits position) const;

    private:

        /// \brief Underlying bit buffer.
        const BitBuffer& buffer_;

        /// \brief Bit offset to start the view from.
        Bits offset_ = 0_Bits;

        /// \brief Size of the view.
        Bits size_ = 0_Bits;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // BitBuffer.

    inline BitBuffer::BitBuffer(ConstMemoryAddress address, Bits size)
        : size_(size)
    {
        data_.resize(std::size_t(ToBytesCeil(size)));

        BitMemCopy(&data_.front(), address, size);
    }

    template <typename TType>
    inline BitBuffer::BitBuffer(const TType& value)
    {
        Append(value);
    }

    inline ConstMemoryAddress BitBuffer::GetData() const
    {
        return &data_.front();
    }

    inline MemoryAddress BitBuffer::GetData()
    {
        return &data_.front();
    }

    inline BitBuffer& BitBuffer::operator&=(const BitBuffer& rhs)
    {
        return BinaryOp(rhs, std::bit_and<uint8_t>{});
    }

    inline BitBuffer& BitBuffer::operator|=(const BitBuffer& rhs)
    {
        return BinaryOp(rhs, std::bit_or<uint8_t>{});
    }

    inline BitBuffer& BitBuffer::operator^=(const BitBuffer& rhs)
    {
        return BinaryOp(rhs, std::bit_xor<uint8_t>{});
    }

    inline BitBuffer BitBuffer::operator~() const
    {
        return BitBuffer(*this).UnaryOp(std::bit_not<uint8_t>{});
    }

    inline Bits BitBuffer::GetSize() const
    {
        return size_;
    }

    inline void BitBuffer::Reserve(Bits capacity)
    {
        data_.reserve(std::size_t(ToBytesCeil(capacity)));
    }

    inline void BitBuffer::Resize(Bits size)
    {
        data_.resize(std::size_t(ToBytesCeil(size)), 0u);
        size_ = size;
    }

    template <typename TType>
    inline TType BitBuffer::Read(Bits position) const
    {
        TType value;

        std::memset(&value, 0, sizeof(TType));

        auto read_count = std::min(BitsOf<TType>(), size_ - position);

        BitMemCopy(&value, MemoryBitAddress(GetData(), position), read_count);

        return value;
    }

    template <typename TType>
    inline void BitBuffer::Write(Bits position, const TType& value)
    {
        Resize(std::max(size_, position + BitsOf<TType>()));

        BitMemCopy(MemoryBitAddress(GetData(), position), &value, BitsOf<TType>());
    }

    template <typename TType>
    inline void BitBuffer::Append(const TType& value)
    {
        Write(GetSize(), value);
    }

    template <typename TOp>
    BitBuffer& BitBuffer::BinaryOp(const BitBuffer& rhs, TOp operation)
    {
        Resize(std::max(rhs.size_, size_));

        auto lhs_word = std::begin(data_);

        for (auto&& rhs_word : rhs.data_)
        {
            *lhs_word = operation(*lhs_word, rhs_word);

            ++lhs_word;
        }

        Sanitize();

        return *this;
    }

    template <typename TOp>
    inline BitBuffer& BitBuffer::UnaryOp(TOp operation)
    {
        for (auto&& lhs_word : data_)
        {
            lhs_word = operation(lhs_word);
        }

        Sanitize();

        return *this;
    }

    inline void BitBuffer::Sanitize()
    {
        if (auto trail = std::size_t(size_) % Bits::kByte; trail > 0u)
        {
            data_.back() &= std::uint8_t((1u << trail) - 1u);
        }
    }

    inline BitBuffer operator&(const BitBuffer& lhs, const BitBuffer& rhs)
    {
        return BitBuffer(lhs) &= rhs;
    }

    inline BitBuffer operator|(const BitBuffer& lhs, const BitBuffer& rhs)
    {
        return BitBuffer(lhs) |= rhs;
    }

    inline BitBuffer operator^(const BitBuffer& lhs, const BitBuffer& rhs)
    {
        return BitBuffer(lhs) ^= rhs;
    }

    // BitBufferView.

    inline BitBufferView::BitBufferView(const BitBuffer& buffer)
        : buffer_(buffer)
        , size_(buffer.GetSize())
    {

    }

    inline BitBufferView::BitBufferView(const BitBuffer& buffer, Bits offset)
        : buffer_(buffer)
        , offset_(offset)
        , size_(buffer.GetSize() - offset)
    {
        SYNTROPY_ASSERT(offset < buffer_.GetSize());
    }

    inline BitBufferView::BitBufferView(const BitBuffer& buffer, Bits offset, Bits size)
        : buffer_(buffer)
        , offset_(offset)
        , size_(size)
    {
        SYNTROPY_ASSERT(offset < buffer_.GetSize());
        SYNTROPY_ASSERT((offset + size) <= buffer_.GetSize());
    }

    inline BitBufferView::BitBufferView(const BitBufferView& buffer_view, Bits offset)
        : buffer_(buffer_view.buffer_)
        , offset_(buffer_view.offset_ + offset)
        , size_(buffer_view.GetSize() - offset)
    {
        SYNTROPY_ASSERT(offset < buffer_view.GetSize());
    }

    inline BitBufferView::BitBufferView(const BitBufferView& buffer_view, Bits offset, Bits size)
        : buffer_(buffer_view.buffer_)
        , offset_(buffer_view.offset_ + offset)
        , size_(size)
    {
        SYNTROPY_ASSERT(offset < buffer_view.GetSize());
        SYNTROPY_ASSERT((offset + size) <= buffer_view.GetSize());
    }

    inline ConstMemoryBitAddress BitBufferView::GetData() const
    {
        return ConstMemoryBitAddress(buffer_.GetData()) + offset_;
    }

    inline Bits BitBufferView::GetSize() const
    {
        return size_;
    }

    template <typename TType>
    inline TType BitBufferView::Read(Bits position) const
    {
        TType value;

        std::memset(&value, 0, sizeof(TType));

        auto read_count = std::min(BitsOf<TType>(), GetSize() - position);

        BitMemCopy(&value, GetData() + position, read_count);

        return value;
    }

}
