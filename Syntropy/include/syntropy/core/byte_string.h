
/// \file byte_string.h
/// \brief This header is part of the Syntropy core module. It contains definition for byte strings.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/range.h"
#include "syntropy/language/utility.h"
#include "syntropy/allocators/memory_resource.h"
#include "syntropy/math/constants.h"
#include "syntropy/math/math.h"
#include "syntropy/memory/memory_buffer.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"

namespace syntropy
{
    /************************************************************************/
    /* BYTE STRING                                                          */
    /************************************************************************/

    /// \brief Represents a raw string of bytes.
    /// \author Raffaele D. Facendola - June 2020
    class ByteString
    {
    public:

        /// \brief Create a new empty string.
        ByteString(MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Create a new zero-filled string
        /// \param size Size of the string, in bytes.
        /// \param memory_resource Memory resource the string will be allocated from.
        ByteString(Bytes size, MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Create a new zero-filled aligned string.
        /// \param size Size of the string, in bytes.
        /// \param alignment String alignment.
        /// \param memory_resource Memory resource the string will be allocated from.
        ByteString(Bytes size, Alignment alignment, MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Default copy constructor.
        ByteString(const ByteString& other) = default;

        /// \brief Default move constructor.
        ByteString(ByteString&& other) = default;

        /// \brief Default copy-assignment operator.
        ByteString& operator=(const ByteString& other) = default;

        /// \brief Default move-assignment operator.
        ByteString& operator=(ByteString&& other) = default;

        /// \brief Default destructor.
        ~ByteString() = default;

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

        /// \brief Get the string alignment.
        /// \return Returns the string alignment.
        Alignment GetAlignment() const;

        /// \brief Access the memory resource this string is allocated on.
        MemoryResource& GetMemoryResource() const;

        /// \brief Swap the content of this string with another one.
        /// \remarks This method swaps underlying memory resources as well.
        void Swap(ByteString& other) noexcept;

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
    void swap(ByteString& lhs, ByteString& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ByteString.

    inline ByteString::ByteString(MemoryResource& memory_resource)
        : ByteString(Bytes{}, memory_resource)
    {

    }

    inline ByteString::ByteString(Bytes size, MemoryResource& memory_resource)
        : ByteString(size, Alignment{}, memory_resource)
    {

    }

    inline ByteString::ByteString(Bytes size, Alignment alignment, MemoryResource& memory_resource)
        : buffer_(size, alignment, memory_resource)
    {

    }

    inline Byte& ByteString::operator[](Int index)
    {
        return const_cast<Byte&>(AsConst(*this)[index]);
    }

    inline const Byte& ByteString::operator[](Int index) const
    {
        auto byte_address = ConstMemoryAddress{ buffer_.GetData()[ToBytes(index)] };     // operator[] resolves the MemoryAddress to a Int pointer type.

        return *byte_address.As<Byte>();
    }

    inline void ByteString::Append(const ConstMemoryRange& data)
    {
        Write(*size_, data);
    }

    inline void ByteString::Clear()
    {
        using namespace Literals;

        size_ = 0_Bytes;
    }

    inline void ByteString::Resize(Bytes size)
    {
        Realloc(size);
        size_ = size;
    }

    inline void ByteString::Reserve(Bytes capacity)
    {
        if (capacity > GetCapacity())
        {
            Realloc(capacity);
        }
    }

    inline void ByteString::Shrink()
    {
        if (GetCapacity() > size_)
        {
            Realloc(size_);
        }
    }

    inline Bool ByteString::IsEmpty() const
    {
        using namespace Literals;

        return size_ == 0_Bytes;
    }

    inline MemoryRange ByteString::GetData()
    {
        return { buffer_.GetData().Begin(), size_ };    // Shall not exceed string size.
    }

    inline ConstMemoryRange ByteString::GetData() const
    {
        return { buffer_.GetData().Begin(), size_ };    // Shall not exceed string size.
    }

    inline Bytes ByteString::GetSize() const
    {
        return size_;
    }

    inline Bytes ByteString::GetCapacity() const
    {
        return buffer_.GetSize();
    }

    inline Alignment ByteString::GetAlignment() const
    {
        return buffer_.GetAlignment();
    }

    inline MemoryResource& ByteString::GetMemoryResource() const
    {
        return buffer_.GetMemoryResource();
    }

    inline void ByteString::Swap(ByteString& other) noexcept
    {
        using std::swap;

        swap(buffer_, other.buffer_);
        swap(size_, other.size_);

    }

    inline void swap(ByteString& lhs, ByteString& rhs)
    {
        lhs.Swap(rhs);
    }

}

