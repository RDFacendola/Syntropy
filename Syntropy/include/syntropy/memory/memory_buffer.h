
/// \file memory_buffer.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definitions for memory buffers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/memory_address.h"

#include "syntropy/allocators/memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY BUFFER                                                        */
    /************************************************************************/

    /// \brief Represents a raw memory buffer.
    /// The buffer must be allocated via an explicit allocator.
    /// \author Raffaele D. Facendola - February 2017
    class MemoryBuffer
    {
    public:

        /// \brief Create a new empty buffer.
        MemoryBuffer() = default;

        /// \brief Create a new memory buffer using the default memory resource.
        /// \param size Size of the buffer, in bytes.
        MemoryBuffer(Bytes size);

        /// \brief Create a new aligned memory buffer using the default memory resource.
        /// \param size Size of the buffer, in bytes.
        MemoryBuffer(Bytes size, Alignment alignment);

        /// \brief Create a new memory buffer.
        /// \param size Size of the buffer, in bytes.
        /// \param memory_resource Memory resource the buffer will be allocated from.
        MemoryBuffer(Bytes size, MemoryResource& memory_resource);

        /// \brief Create a new memory buffer.
        /// \param size Size of the buffer, in bytes.
        /// \param alignment Buffer alignment.
        /// \param memory_resource Memory resource the buffer will be allocated from.
        MemoryBuffer(Bytes size, Alignment alignment, MemoryResource& memory_resource);

        /// \brief Copy constructor.
        /// Copy the content of another buffer to this one.
        /// \param other Buffer to copy.
        MemoryBuffer(const MemoryBuffer& other);

        /// \brief Move constructor.
        /// Assign the memory buffer of another instance to this one.
        /// \param other Buffer to move.
        MemoryBuffer(MemoryBuffer&& other);

        /// \brief Destructor.
        ~MemoryBuffer();

        /// \brief Unified assignment operator.
        MemoryBuffer& operator=(MemoryBuffer other);

        /// \brief Dereferencing operator. Access the base address of the buffer.
        /// \return Returns the base address of the buffer.
        MemoryAddress operator*();

        /// \brief Dereferencing operator. Access the base address of the buffer.
        /// \return Returns the base address of the buffer.
        ConstMemoryAddress operator*() const;

        /// \brief Access an element in the buffer.
        /// \param offset Offset with respect to the first element of the buffer.
        /// \return Returns a pointer to the element (buffer+offset).
        MemoryAddress operator[](Bytes offset);

        /// \brief Access an element in the buffer.
        /// \param offset Offset with respect to the first element of the buffer.
        /// \return Returns a pointer to the element (buffer+offset).
        ConstMemoryAddress operator[](Bytes offset) const;

        /// \brief Get the size of the buffer, in bytes.
        /// \return Returns the size of the buffer, in bytes.
        Bytes GetSize() const;

        /// \brief Get the buffer memory range.
        MemoryRange GetRange();

        /// \brief Get the buffer memory range.
        ConstMemoryRange GetRange() const;

        /// \brief Swap the content of this buffer with another one.
        void Swap(MemoryBuffer& other) noexcept;

    private:

        ///< \brief Memory resource the buffer was allocated on.
        MemoryResource* memory_resource_{ nullptr };

        /// \brief Buffer alignment.
        Alignment alignment_;

        ///< \brief Buffer memory range.
        MemoryRange buffer_;

    };

    /// \brief Swaps two memory buffers.
    void swap(syntropy::MemoryBuffer& lhs, syntropy::MemoryBuffer& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryBuffer.

    inline MemoryBuffer::MemoryBuffer(Bytes size)
        : MemoryBuffer(size, Alignment{})
    {

    }

    inline MemoryBuffer::MemoryBuffer(Bytes size, Alignment alignment)
        : MemoryBuffer(size, alignment, GetDefaultMemoryResource())
    {

    }


    inline MemoryBuffer::MemoryBuffer(Bytes size, MemoryResource& memory_resource)
        : MemoryBuffer(size, Alignment{}, memory_resource)
    {

    }

    inline MemoryBuffer::MemoryBuffer(Bytes size, Alignment alignment, MemoryResource& memory_resource)
        : memory_resource_(std::addressof(memory_resource))
        , alignment_(alignment)
        , buffer_(memory_resource.Allocate(size, alignment))
    {

    }

    inline MemoryBuffer::MemoryBuffer(const MemoryBuffer& other)
        : memory_resource_(other.memory_resource_)
        , alignment_(other.alignment_)
        , buffer_(memory_resource_->Allocate(other.GetSize(), alignment_))

    {
        std::memmove(buffer_.Begin(), other.buffer_.Begin(), std::size_t(other.GetSize()));
    }

    inline MemoryBuffer::MemoryBuffer(MemoryBuffer&& other)
        : memory_resource_(other.memory_resource_)
        , alignment_(other.alignment_)
        , buffer_(other.buffer_)
    {
        other.buffer_ = {};
    }

    inline MemoryBuffer::~MemoryBuffer()
    {
        if (memory_resource_)
        {
            memory_resource_->Deallocate(buffer_, alignment_);
        }
    }

    inline MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer other)
    {
        Swap(other);
        return *this;
    }

    inline MemoryAddress MemoryBuffer::operator*()
    {
        return buffer_.Begin();
    }

    inline ConstMemoryAddress MemoryBuffer::operator*() const
    {
        return buffer_.Begin();
    }

    inline MemoryAddress MemoryBuffer::operator[](Bytes offset)
    {
        return buffer_[offset];
    }

    inline ConstMemoryAddress MemoryBuffer::operator[](Bytes offset) const
    {
        return buffer_[offset];
    }

    inline Bytes MemoryBuffer::GetSize() const
    {
        return buffer_.GetSize();
    }

    inline MemoryRange MemoryBuffer::GetRange()
    {
        return buffer_;
    }

    inline ConstMemoryRange MemoryBuffer::GetRange() const
    {
        return buffer_;
    }

    inline void MemoryBuffer::Swap(MemoryBuffer& other) noexcept
    {
        std::swap(memory_resource_, other.memory_resource_);
        std::swap(alignment_, other.alignment_);
        std::swap(buffer_, other.buffer_);
    }

    inline void swap(syntropy::MemoryBuffer& lhs, syntropy::MemoryBuffer& rhs)
    {
        lhs.Swap(rhs);
    }

}

