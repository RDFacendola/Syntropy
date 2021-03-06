
/// \file stream_buffer.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for a stream buffer.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/support.h"

#include "syntropy/deprecated/core/algorithms/algorithm.h"

#include "syntropy/experimental/memory/smart_pointers.h"
#include "syntropy/diagnostics/foundation/assert.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/buffer.h"
#include "syntropy/allocators/allocator.h"
#include "syntropy/math/constants.h"
#include "syntropy/math/math.h"

namespace Syntropy
{
    /************************************************************************/
    /* STREAM BUFFER                                                        */
    /************************************************************************/

    class StreamBufferTransaction;

    /// \brief Represents a raw stream of bytes.
    /// Stream buffer supports both FIFO and random I\O operations.
    /// \author Raffaele D. Facendola - June 2020
    class StreamBuffer
    {
        friend class StreamBufferTransaction;

    public:

        /// \brief Create a new empty stream.
        StreamBuffer(Allocator& memory_resource = Memory::GetAllocator());

        /// \brief Create a new stream by moving an existing memory buffer.
        StreamBuffer(Memory::Buffer&& buffer);

        /// \brief Create a new stream by copying a memory buffer.
        StreamBuffer(const Memory::Buffer& buffer, Allocator& memory_resource);

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
        /// Append operations are performed tentatively if there's an active transaction.
        /// \return Returns the range containing unwritten data.
        Memory::ByteSpan Append(const Memory::ByteSpan& data);

        /// \brief Read data sequentially from the stream, causing it to shrink.
        /// Consume operations are performed tentatively if there's an active transaction.
        /// \return Returns the range containing read data.
        Memory::RWByteSpan Consume(const Memory::RWByteSpan& data);

        /// \brief Write data at given position from buffer start.
        /// Writes past the end of the stream are no-ops. This method does not change stream allocation.
        /// \return Returns the range containing unwritten data.
        Memory::ByteSpan Write(Memory::Bytes position, const Memory::ByteSpan& data);

        /// \brief Read data at given position from buffer start.
        /// Reads past the end of the stream are no-ops. This method does not change stream allocation.
        /// \return Returns the range containing read data.
        Memory::RWByteSpan Read(Memory::Bytes position, const Memory::RWByteSpan& data) const;

        /// \brief Discard data content and clear the underlying buffer.
        void Discard();

        /// \brief Release and return underlying memory buffer and clear the stream buffer.
        Memory::Buffer Release();

        /// \brief Increase the underlying buffer allocation up to a given size.
        void Reserve(Memory::Bytes capacity);

        /// \brief Increase the underlying buffer allocation size by a given amount.
        void Grow(Memory::Bytes capacity);

        /// \brief Shrink the allocation size up to the current content size.
        void Shrink();

        /// \brief Check whether the underlying buffer is empty.
        Bool IsEmpty() const;

        /// \brief Get the stream content size, in bytes.
        Memory::Bytes GetSize() const;

        /// \brief Get the effective memory footprint of the underlying buffer, in bytes.
        Memory::Bytes GetCapacity() const;

        /// \brief Access the memory resource the underlying buffer is allocated on.
        Allocator& GetMemoryResource() const;

        /// \brief Swap the content of this stream with another one.
        /// \remarks This method swaps underlying memory resources as well.
        void Swap(StreamBuffer& other) noexcept;

    private:

        /// \brief Growing factor after each reallocation.
        static constexpr auto kGrowthFactor = Math::kPhi;

        /// \brief Growth bias added to each reallocation.
        static constexpr auto kGrowthBias = Int{ 8 };

        /// \brief Reallocate the underlying buffer, filling additional bytes with zeros.
        /// This method affects only buffer capacity, not stream size.
        /// This method unfolds the previous circular content into the new buffer.
        void Realloc(Memory::Bytes capacity);

        /// \brief Get the address of a byte at given offset from the base pointer, wrapping around.
        Memory::RWBytePtr GetAddress(Memory::Bytes offset);

        /// \brief Get the address of a byte at given offset from the base pointer, wrapping around.
        Memory::BytePtr GetAddress(Memory::Bytes offset) const;

        /// \brief 
        void Commit(Memory::Bytes append_size, Memory::Bytes consume_size);

        void Rollback(Memory::Bytes append_size, Memory::Bytes consume_size);

        /// \brief Underlying memory buffer, may be larger than current stream size.
        /// This buffer is circular to prevent reallocations from consume operations.
        Memory::Buffer buffer_;

        /// \brief Offset within the buffer data start from (inclusive).
        Memory::RWBytePtr base_pointer_;

        /// \brief Number of committed bytes in the underlying buffer.
        Memory::Bytes size_;

        /// \brief Size of appended bytes, including pending ones. This value is always greater or equal to the committed size.
        Memory::Bytes append_size_;

        /// \brief Size of consumed bytes, including pending ones. This value is always less or equal to the committed size.
        Memory::Bytes consume_size_;

        /// \brief Current active transaction.
        RWPointer<StreamBufferTransaction> transaction_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamBuffer.

    inline StreamBuffer::StreamBuffer(Allocator& memory_resource)
        : buffer_(memory_resource)
        , base_pointer_(Begin(buffer_.GetData()))
    {

    }

    inline StreamBuffer::StreamBuffer(Memory::Buffer&& buffer)
        : buffer_(Move(buffer))
        , base_pointer_(Begin(buffer_.GetData()))
        , size_(Memory::Size(buffer_))
        , append_size_(size_)
        , consume_size_(size_)
    {

    }

    inline StreamBuffer::StreamBuffer(const Memory::Buffer& buffer, Allocator& memory_resource)
        : buffer_(Memory::Size(buffer), memory_resource)
        , base_pointer_(Begin(buffer_.GetData()))
        , size_(Memory::Size(buffer_))
    {
        Memory::Copy(buffer_.GetData(), buffer.GetData());
    }

    inline void StreamBuffer::Discard()
    {
        Memory::Zero(buffer_.GetData());

        base_pointer_ = Begin(buffer_.GetData());
        size_ = Memory::Bytes{ 0 };
    }

    inline void StreamBuffer::Reserve(Memory::Bytes capacity)
    {
        if (capacity > GetCapacity())
        {
            Realloc(capacity);
        }
    }

    inline void StreamBuffer::Grow(Memory::Bytes capacity)
    {
        Reserve(GetCapacity() + capacity);
    }

    inline void StreamBuffer::Shrink()
    {
        if (auto size = GetSize(); size < GetCapacity())
        {
            Realloc(size);
        }
    }

    inline Bool StreamBuffer::IsEmpty() const
    {
        return GetSize() == Memory::Bytes{ 0 };
    }

    inline Memory::Bytes StreamBuffer::GetSize() const
    {
        return size_;
    }

    inline Memory::Bytes StreamBuffer::GetCapacity() const
    {
        return Memory::Size(buffer_);
    }

    inline Allocator& StreamBuffer::GetMemoryResource() const
    {
        return buffer_.GetAllocator();
    }

    inline Memory::Buffer StreamBuffer::Release()
    {
        // Rotate the underlying buffer such that base pointer is shifted in first position.

        Rotate(buffer_.GetData(), base_pointer_);

        // Clear.

        auto buffer = Memory::Buffer(buffer_.GetAllocator());

        buffer.Swap(buffer_);
        base_pointer_ = Begin(buffer_.GetData());
        size_ = Memory::Bytes{};

        return buffer;
    }

    inline void StreamBuffer::Swap(StreamBuffer& other) noexcept
    {
        Syntropy::Swap(buffer_, other.buffer_);
        Syntropy::Swap(base_pointer_, other.base_pointer_);
        Syntropy::Swap(size_, other.size_);
    }

}

