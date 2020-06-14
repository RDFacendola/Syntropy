
/// \file memory_stream_buffer.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for a memory stream buffer.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/language/initializer_list.h"
#include "syntropy/language/algorithm.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/memory_buffer.h"
#include "syntropy/allocators/memory_resource.h"
#include "syntropy/math/constants.h"
#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY STREAM BUFFER                                                 */
    /************************************************************************/

    /// \brief Represents a raw stream of bytes.
    /// Stream buffer supports both FIFO and random I\O operations.
    /// \author Raffaele D. Facendola - June 2020
    class MemoryStreamBuffer
    {
    public:

        /// \brief Create a new empty stream.
        MemoryStreamBuffer(MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Create a new stream by moving an existing memory buffer.
        MemoryStreamBuffer(MemoryBuffer&& buffer);

        /// \brief Create a new stream by copying a memory buffer.
        MemoryStreamBuffer(const MemoryBuffer& buffer, MemoryResource& memory_resource);

        /// \brief Default copy constructor.
        MemoryStreamBuffer(const MemoryStreamBuffer& other) = default;

        /// \brief Default move constructor.
        MemoryStreamBuffer(MemoryStreamBuffer&& other) = default;

        /// \brief Default copy-assignment operator.
        MemoryStreamBuffer& operator=(const MemoryStreamBuffer& other) = default;

        /// \brief Default move-assignment operator.
        MemoryStreamBuffer& operator=(MemoryStreamBuffer&& other) = default;

        /// \brief Default destructor.
        ~MemoryStreamBuffer() = default;

        /// \brief Write data sequentially to the stream, causing it to grow.
        /// \return Returns the range containing unwritten data.
        ConstMemoryRange Append(const ConstMemoryRange& data);

        /// \brief Read data sequentially from the stream, causing it to shrink.
        /// \return Returns the range containing read data.
        MemoryRange Consume(const MemoryRange& data);

        /// \brief Write data at given position from buffer start.
        /// Writes past the end of the stream are no-ops. This method does not change stream allocation.
        /// \return Returns the range containing unwritten data.
        ConstMemoryRange Write(Bytes position, const ConstMemoryRange& data);

        /// \brief Read data at given position from buffer start.
        /// Reads past the end of the stream are no-ops. This method does not change stream allocation.
        /// \return Returns the range containing read data.
        MemoryRange Read(Bytes position, const MemoryRange& data) const;

        /// \brief Discard data content and clear the underlying buffer.
        void Clear();

        /// \brief Release and return underlying memory buffer and clear stream buffer state.
        MemoryBuffer Release();

        /// \brief Increase the underlying buffer allocation to a given size.
        /// This method reallocates only if the provided capacity exceeds the current allocation size, otherwise it behaves as no-op.
        void Reserve(Bytes capacity);

        /// \brief Increase the underlying buffer allocation size by a given amount.
        void Grow(Bytes capacity);

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
        void Swap(MemoryStreamBuffer& other) noexcept;

    private:

        /// \brief Growing factor after each reallocation.
        static constexpr auto kGrowthFactor = Math::kPhi;

        /// \brief Growth bias added to each reallocation.
        static constexpr auto kGrowthBias = Int{ 8 };

        /// \brief Reallocate the underlying buffer, filling additional bytes with zeros.
        /// This method affects only buffer capacity, not stream size.
        /// This method unfolds the previous circular content into the new buffer.
        void Realloc(Bytes capacity);

        /// \brief Get the address of a byte at given offset from the base pointer, wrapping around.
        MemoryAddress GetAddress(Bytes offset);

        /// \brief Get the address of a byte at given offset from the base pointer, wrapping around.
        ConstMemoryAddress GetAddress(Bytes offset) const;

        /// \brief Underlying memory buffer, may be larger than current stream size.
        /// This buffer is circular to prevent reallocations from consume operations.
        MemoryBuffer buffer_;

        /// \brief Offset within the buffer data start from (inclusive).
        MemoryAddress base_pointer_;

        /// \brief Current data size, can be lower than buffer's capacity.
        Bytes size_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two memory stream buffer.
    void swap(MemoryStreamBuffer& lhs, MemoryStreamBuffer& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryStreamBuffer.

    inline MemoryStreamBuffer::MemoryStreamBuffer(MemoryResource& memory_resource)
        : buffer_(memory_resource)
        , base_pointer_(buffer_.GetData().Begin())
    {

    }

    inline MemoryStreamBuffer::MemoryStreamBuffer(MemoryBuffer&& buffer)
        : buffer_(std::move(buffer))
        , base_pointer_(buffer_.GetData().Begin())
        , size_(buffer_.GetSize())
    {

    }

    inline MemoryStreamBuffer::MemoryStreamBuffer(const MemoryBuffer& buffer, MemoryResource& memory_resource)
        : buffer_(buffer.GetSize(), memory_resource)
        , base_pointer_(buffer_.GetData().Begin())
        , size_(buffer_.GetSize())
    {
        Memory::Copy(buffer_.GetData(), buffer.GetData());
    }

    inline void MemoryStreamBuffer::Clear()
    {
        Memory::Zero(buffer_.GetData());

        base_pointer_ = buffer_.GetData().Begin();
        size_ = Bytes{ 0 };
    }

    inline void MemoryStreamBuffer::Reserve(Bytes capacity)
    {
        if (capacity > GetCapacity())
        {
            Realloc(capacity);
        }
    }

    inline void MemoryStreamBuffer::Grow(Bytes capacity)
    {
        Reserve(GetCapacity() + capacity);
    }

    inline void MemoryStreamBuffer::Shrink()
    {
        if (auto size = GetSize(); size < GetCapacity())
        {
            Realloc(size);
        }
    }

    inline Bool MemoryStreamBuffer::IsEmpty() const
    {
        return GetSize() == Bytes{ 0 };
    }

    inline Bytes MemoryStreamBuffer::GetSize() const
    {
        return size_;
    }

    inline Bytes MemoryStreamBuffer::GetCapacity() const
    {
        return buffer_.GetSize();
    }

    inline MemoryResource& MemoryStreamBuffer::GetMemoryResource() const
    {
        return buffer_.GetMemoryResource();
    }

    inline MemoryBuffer MemoryStreamBuffer::Release()
    {
        // Rotate the underlying buffer such that base pointer is shifted in first position.

        Rotate(buffer_.GetData(), base_pointer_);

        // Clear.

        auto buffer = MemoryBuffer(buffer_.GetMemoryResource());

        buffer.Swap(buffer_);
        base_pointer_ = buffer_.Begin();
        size_ = Bytes{};

        return buffer;
    }

    inline void MemoryStreamBuffer::Swap(MemoryStreamBuffer& other) noexcept
    {
        using std::swap;

        swap(buffer_, other.buffer_);
        swap(base_pointer_, other.base_pointer_);
        swap(size_, other.size_);
    }

    // Non-member functions.

    inline void swap(MemoryStreamBuffer& lhs, MemoryStreamBuffer& rhs)
    {
        lhs.Swap(rhs);
    }

}

