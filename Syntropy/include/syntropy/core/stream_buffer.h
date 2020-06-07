
/// \file stream_buffer.h
/// \brief This header is part of the Syntropy core module. It contains definition for a memory stream buffer.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/utility.h"
#include "syntropy/core/types.h"
#include "syntropy/diagnostics/assert.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/memory_buffer.h"
#include "syntropy/allocators/memory_resource.h"
#include "syntropy/math/constants.h"
#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM BUFFER                                                        */
    /************************************************************************/

    /// \brief Represents a raw stream of bytes.
    /// Stream buffer supports FIFO and random I\O operations.
    /// \author Raffaele D. Facendola - June 2020
    class StreamBuffer
    {
    public:

        /// \brief Create a new empty stream.
        StreamBuffer(MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Default copy constructor.
        StreamBuffer(const StreamBuffer& other) = default;

        /// \brief Default move constructor.
        StreamBuffer(StreamBuffer&& other) = default;

        /// \brief Default copy-assignment operator.
        StreamBuffer& operator=(const StreamBuffer& other) = default;

        /// \brief Default move-assignment operator.
        StreamBuffer& operator=(StreamBuffer&& other) = default;

        /// \brief Default destructor.
        ~StreamBuffer() = default;

        /// \brief Write data sequentially to the stream, causing it to grow.
        /// \return Returns the range containing unwritten data.
        ConstMemoryRange WriteSequential(const ConstMemoryRange& data);

        /// \brief Read data sequentially from the stream, causing it to shrink.
        /// \return Returns the range containing read data.
        MemoryRange ReadSequential(const MemoryRange& data);

        /// \brief Write data at given position from buffer start.
        /// Writes past the end of the stream are no-ops. This method does not change stream allocation.
        /// \return Returns the range containing unwritten data.
        ConstMemoryRange WriteRandom(Bytes position, const ConstMemoryRange& data);

        /// \brief Read data at given position from buffer start.
        /// Reads past the end of the stream are no-ops. This method does not change stream allocation.
        /// \return Returns the range containing read data.
        MemoryRange ReadRandom(Bytes position, const MemoryRange& data);

        /// \brief Flush the underlying buffer, discarding its content.
        void Flush();

        /// \brief Increase the underlying buffer allocation size.
        /// This method preserve the buffer content, therefore it behaves as no-op if the specified capacity is lower than the current one.
        /// This method may cause buffer reallocation.
        void Reserve(Bytes capacity);

        /// \brief Shrink the allocation size up to the current buffer size.
        /// This method preserve the buffer content and may reallocate the underlying buffer.
        void Shrink();

        /// \brief Check whether the underlying buffer is empty.
        Bool IsEmpty() const;

        /// \brief Get the stream content size, in bytes.
        Bytes GetSize() const;

        /// \brief Get the effective memory footprint of the underlying buffer, in bytes.
        Bytes GetCapacity() const;

        /// \brief Access the memory resource the underlying buffer is allocated on.
        MemoryResource& GetMemoryResource() const;

        /// \brief Swap the content of this stream with another one.
        /// \remarks This method swaps underlying memory resources as well.
        void Swap(StreamBuffer& other) noexcept;

    private:

        /// \brief Growing factor after each reallocation.
        static constexpr auto kGrowthFactor = Math::kPhi;

        /// \brief Growth bias added to each reallocation.
        static constexpr auto kGrowthBias = Int{ 8 };

        /// \brief Increase the underlying buffer allocation size.
        /// This method preserve the buffer content, therefore it behaves as no-op if the specified capacity is lower than the current one.
        /// This method over-allocates to reduce frequent reallocations.
        void Grow(Bytes capacity);

        /// \brief Get the physical position inside the underlying buffer.
        Bytes GetBufferPosition(Bytes position) const;

        /// \brief Reallocate the underlying buffer, filling additional bytes with zeros.
        /// This method affects only buffer capacity, not stream size.
        /// This method unfolds the previous circular content into the new buffer.
        void Realloc(Bytes capacity);

        /// \brief Underlying memory buffer, may be larger than current stream size.
        /// This buffer is circular to prevent reallocations from sequential reads.
        /// Exceeding the current buffer size causes reallocation: content is preserved.
        MemoryBuffer buffer_;

        /// \brief String size.
        Bytes size_;

        /// \brief Index of the first element in the circular buffer.
        Bytes start_position_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two byte string.
    void swap(StreamBuffer& lhs, StreamBuffer& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamBuffer.

    inline StreamBuffer::StreamBuffer(MemoryResource& memory_resource)
        : buffer_(memory_resource)
    {

    }

    inline ConstMemoryRange StreamBuffer::WriteSequential(const ConstMemoryRange& data)
    {
        auto write_position = size_;

        size_ += data.GetSize();                                                            // Size after writing additional data.

        Grow(size_);                                                                        // Allocate space to fit new data, if necessary.

        return WriteRandom(write_position, data);                                           // Returned range is expected to be empty.
    }

    inline MemoryRange StreamBuffer::ReadSequential(const MemoryRange& data)
    {
        auto range = ReadRandom(Bytes{ 0 }, data);                                          // Read from the buffer begin, wrapping around.

        start_position_ = GetBufferPosition(range.GetSize());                               // Consume read data and move the start forward, wrapping around.

        size_ -= range.GetSize();

        return range;
    }

    inline ConstMemoryRange StreamBuffer::WriteRandom(Bytes position, const ConstMemoryRange& data)
    {
        auto write_position = GetBufferPosition(position);

        auto source = UpperBound(data, size_);

        auto bytes = Memory::CopyFold(buffer_.GetData(), source, write_position);

        return { source.End(), data.End() };
    }

    inline MemoryRange StreamBuffer::ReadRandom(Bytes position, const MemoryRange& data)
    {
        auto read_position = GetBufferPosition(position);

        auto destination = UpperBound(data, size_);

        auto bytes = Memory::CopyUnfold(destination, buffer_.GetConstData(), read_position);

        return { data.Begin(), bytes };
    }

    inline void StreamBuffer::Flush()
    {
        Memory::Zero(buffer_.GetData());

        start_position_ = Bytes{ 0 };
    }

    inline void StreamBuffer::Reserve(Bytes capacity)
    {
        if (capacity > GetCapacity())
        {
            Realloc(capacity);
        }
    }

    inline void StreamBuffer::Shrink()
    {
        if (size_ < GetCapacity())
        {
            Realloc(size_);
        }
    }

    inline Bool StreamBuffer::IsEmpty() const
    {
        return size_ == Bytes{ 0 };
    }

    inline Bytes StreamBuffer::GetSize() const
    {
        return size_;
    }

    inline Bytes StreamBuffer::GetCapacity() const
    {
        return buffer_.GetSize();
    }

    inline MemoryResource& StreamBuffer::GetMemoryResource() const
    {
        return buffer_.GetMemoryResource();
    }

    inline void StreamBuffer::Swap(StreamBuffer& other) noexcept
    {
        using std::swap;

        swap(buffer_, other.buffer_);
        swap(size_, other.size_);
    }

    inline void StreamBuffer::Grow(Bytes capacity)
    {
        if (capacity > GetCapacity())
        {
            capacity = ToBytes(Math::CeilTo<Int>((*capacity) * kGrowthFactor + kGrowthBias));

            Realloc(capacity);
        }
    }

    inline Bytes StreamBuffer::GetBufferPosition(Bytes position) const
    {
        return Bytes{ (start_position_ + position) % GetCapacity() };
    }

    inline void StreamBuffer::Realloc(Bytes capacity)
    {
        SYNTROPY_ASSERT(capacity > size_);

        auto buffer = MemoryBuffer{ capacity, buffer_.GetMemoryResource() };

        Memory::CopyUnfold(buffer.GetData(), buffer_.GetConstData(), start_position_);

        buffer_.Swap(buffer);

        start_position_ = Bytes{ 0 };
    }

    // Non-member functions.

    inline void swap(StreamBuffer& lhs, StreamBuffer& rhs)
    {
        lhs.Swap(rhs);
    }

}

