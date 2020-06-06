
/// \file stream_buffer.h
/// \brief This header is part of the Syntropy core module. It contains definition for a memory stream buffer.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/utility.h"
#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/memory_buffer.h"
#include "syntropy/allocators/memory_resource.h"
#include "syntropy/math/constants.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM BUFFER                                                        */
    /************************************************************************/

    /// \brief Represents a raw stream of bytes.
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

        /// \brief Access a byte on the underlying buffer.
        Byte& operator[](Int index);

        /// \brief Access a byte on the underlying buffer.
        const Byte& operator[](Int index) const;

        /// \brief Append data at the end of the stream.
        void Append(const ConstMemoryRange& data);

        /// \brief Write data at given position.
        /// Writing past the end of the buffer causes the underlying stream to grow. Bytes not being written to are zero-filled.
        /// Additional bytes that are not copied over are zero-initialized.
        void Write(Int position, const ConstMemoryRange& data);

        /// \brief Read data at given position.
        /// Reading past the end of the buffer behave as if the underlying buffer continued with a sequence of bytes equal to zero.
        /// \return Returns the range containing read data. This range is expected to be contained inside the provided range.
        MemoryRange Read(Int position, const MemoryRange& data);

        /// \brief Clear the byte string.
        void Clear();

        /// \brief Resize the byte string up to an exact given size.
        /// Eventual additional memory is zero-initialized.
        void Resize(Bytes size);

        /// \brief Change the underlying allocation size.
        /// If a capacity lower than the current amount causes this method to behave as no-op.
        void Reserve(Bytes capacity);

        /// \brief Shrink the allocation size up to the current string size.
        void Shrink();

        /// \brief Check whether the string is empty.
        Bool IsEmpty() const;

        /// \brief Get the underlying data range.
        MemoryRange GetData();

        /// \brief Get the underlying const data range.
        ConstMemoryRange GetData() const;

        /// \brief Get the size of the string, in bytes.
        /// \return Returns the size of the string, in bytes.
        Bytes GetSize() const;

        /// \brief Get the effective memory footprint of the string, in bytes.
        /// \return Returns the effective memory footprint of the string, in bytes.
        Bytes GetCapacity() const;

        /// \brief Access the memory resource this string is allocated on.
        MemoryResource& GetMemoryResource() const;

        /// \brief Swap the content of this string with another one.
        /// \remarks This method swaps underlying memory resources as well.
        void Swap(StreamBuffer& other) noexcept;

    private:

        /// \brief Growing factor after each reallocation.
        static constexpr auto kGrowthFactor = Math::kPhi;

        /// \brief Growth bias added to each reallocation.
        static constexpr auto kGrowthBias = Int{ 8 };

        /// \brief Reallocate the underlying buffer. Additional bytes allocated by this method are zero-initialized.
        /// This method affects only buffer capacity, not size.
        void Realloc(Bytes size);

        /// \brief Underlying memory buffer. May be larger than actual size.
        MemoryBuffer buffer_;

        /// \brief String size.
        Bytes size_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two byte string.
    void swap(StreamBuffer& lhs, StreamBuffer& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ByteString.

    inline StreamBuffer::StreamBuffer(MemoryResource& memory_resource)
        : buffer_(memory_resource)
    {

    }

    inline Byte& StreamBuffer::operator[](Int index)
    {
        return const_cast<Byte&>(AsConst(*this)[index]);
    }

    inline const Byte& StreamBuffer::operator[](Int index) const
    {
        auto byte_address = ConstMemoryAddress{ buffer_.GetData()[ToBytes(index)] };     // operator[] resolves the MemoryAddress to a Int pointer type.

        return *byte_address.As<Byte>();
    }

    inline void StreamBuffer::Append(const ConstMemoryRange& data)
    {
        Write(*size_, data);
    }

    inline void StreamBuffer::Clear()
    {
        using namespace Literals;

        size_ = 0_Bytes;
    }

    inline void StreamBuffer::Resize(Bytes size)
    {
        Realloc(size);
        size_ = size;
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
        if (GetCapacity() > size_)
        {
            Realloc(size_);
        }
    }

    inline Bool StreamBuffer::IsEmpty() const
    {
        using namespace Literals;

        return size_ == 0_Bytes;
    }

    inline MemoryRange StreamBuffer::GetData()
    {
        return { buffer_.GetData().Begin(), size_ };    // Shall not exceed string size.
    }

    inline ConstMemoryRange StreamBuffer::GetData() const
    {
        return { buffer_.GetData().Begin(), size_ };    // Shall not exceed string size.
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

    inline void swap(StreamBuffer& lhs, StreamBuffer& rhs)
    {
        lhs.Swap(rhs);
    }

}

