
/// \file bit_buffer.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of classes used to handle bit-addressed buffers.
///
/// \author Raffaele D. Facendola - June 2019

#pragma once

#include <vector>

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
        BitBuffer(MemoryAddress& address, Bits size);

        /// \brief Create a new buffer by moving from an existing buffer.
        BitBuffer(std::vector<uint8_t> data, Bits size);

        /// \brief Access the buffer data.
        ConstMemoryBitAddress GetData() const;

        /// \brief Get the size of the buffer, in bits.
        Bits GetSize() const;

    private:

        /// \brief Buffer data.
        std::vector<uint8_t> data_;

        /// \brief Size of the buffer.
        Bits size_ = 0_Bits;

    };

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

        /// \brief Create a new view to a buffer, starting at a given offset.
        BitBufferView(const BitBuffer& buffer, Bits offset);

        /// \brief Create a new view to a buffer, starting at a given offset and with an explicit size.
        BitBufferView(const BitBuffer& buffer, Bits offset, Bits size);

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

    private:

        /// \brief Underlying bit buffer.
        const BitBuffer& buffer_;

        /// \brief Bit offset to start the view from.
        Bits offset_ = 0_Bits;

        /// \brief Size of the view.
        Bits size_ = 0_Bits;

    };

    /************************************************************************/
    /* BIT WRITER                                                           */
    /************************************************************************/

    /// \brief Class used to sequentially write values to a bit-addressed buffer.
    /// \author Raffaele D. Facendola - June 2019.
    class BitWriter
    {
    public:

        /// \brief Default constructor.
        BitWriter() = default;

        /// \brief Default copy constructor.
        BitWriter(const BitWriter&) = default;

        /// \brief Default move constructor.
        BitWriter(BitWriter&&) = default;

        /// \brief Append a value to the buffer.
        template <typename TType>
        BitWriter& operator<<(const TType& value);

        /// \brief Get the written buffer and reset the writer status.
        /// \return Returns a buffer containing all the values written so far through this writer.
        BitBuffer GetBuffer();

        /// \brief Reset the writer status.
        void Reset();

    private:

        /// \brief Buffer data.
        std::vector<uint8_t> data_;

        /// \brief Size of the buffer.
        Bits size_ = 0_Bits;

    };

    /************************************************************************/
    /* BIT READER                                                           */
    /************************************************************************/

    /// \brief Class used to sequentially read values from a bit-addressed buffer.
    /// \author Raffaele D. Facendola - June 2019.
    class BitReader
    {
    public:

        /// \brief Default constructor.
        BitReader() = default;

        /// \brief Default copy constructor.
        BitReader(const BitReader&) = default;

        /// \brief Default move constructor.
        BitReader(BitReader&&) = default;

        /// \brief Create a new buffer read for a buffer view.
        BitReader(const BitBufferView& buffer_view);

        /// \brief Read a value from the underlying buffer view.
        /// \remarks Reading past the end of the view behaves as a no-op.
        template <typename TType>
        BitReader& operator>>(TType& value);

        /// \brief Get the underlying reading buffer view.
        const BitBufferView& GetBuffer() const;

        /// \brief Reset the reader status.
        void Reset();

    private:

        /// \brief View to the buffer to read the values from.
        BitBufferView buffer_view_;

        /// \brief Offset to start reading from.
        Bits cursor_ = 0_Bits;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // BitBuffer.

    inline BitBuffer::BitBuffer(MemoryAddress& address, Bits size)
        : size_(size)
    {
        data_.resize(std::size_t(ToBytesCeil(size)));

        BitMemCopy(&data_.front(), address, size);
    }

    inline BitBuffer::BitBuffer(std::vector<uint8_t> data, Bits size)
        : data_(std::move(data))
        , size_(size)
    {
        SYNTROPY_ASSERT(size <= Bytes(data_.size()));
    }

    inline ConstMemoryBitAddress BitBuffer::GetData() const
    {
        return &data_.front();
    }

    inline Bits BitBuffer::GetSize() const
    {
        return size_;
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
        return buffer_.GetData() + offset_;
    }

    inline Bits BitBufferView::GetSize() const
    {
        return size_;
    }

    // BitWriter.

    template <typename TType>
    inline BitWriter& BitWriter::operator<<(const TType& value)
    {
        auto offset = size_;
        auto value_size = BitsOf(value);

        size_ += value_size;

        data_.resize(std::size_t(ToBytesCeil(size_)));

        BitMemCopy(MemoryBitAddress(&data_.front(), offset), &value, value_size);

        return *this;
    }

    inline BitBuffer BitWriter::GetBuffer()
    {
        BitBuffer buffer{ std::move(data_), size_ };

        data_ = {};
        size_ = 0_Bits;

        return buffer;
    }

    inline void BitWriter::Reset()
    {
        data_.clear();
        size_ = 0_Bits;
    }

    // BitReader.

    inline BitReader::BitReader(const BitBufferView& buffer_view)
        : buffer_view_(buffer_view)
    {

    }

    template <typename TType>
    inline BitReader& BitReader::operator>>(TType& value)
    {
        auto value_size = std::min(BitsOf(value), buffer_view_.GetSize() - cursor_);

        BitMemCopy(&value, buffer_view_.GetData() + cursor_, value_size);

        cursor_ += value_size;

        return *this;
    }

    inline const BitBufferView& BitReader::GetBuffer() const
    {
        return buffer_view_;
    }

    inline void BitReader::Reset()
    {
        cursor_ = 0_Bits;
    }
}
