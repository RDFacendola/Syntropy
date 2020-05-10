
/// \file memory_buffer.h
/// \brief This header is part of the Syntropy memory module. It contains classes and definitions for raw memory buffers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/memory/memory.h"
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

    /// \brief Represents a raw memory buffer allocated from a memory resource.
    /// \author Raffaele D. Facendola - February 2017
    class MemoryBuffer
    {
    public:

        /// \brief Create a new empty buffer.
        MemoryBuffer() = default;

        /// \brief Create a new memory buffer.
        /// \param size Size of the buffer, in bytes.
        /// \param memory_resource Memory resource the buffer will be allocated from.
        MemoryBuffer(Bytes size, MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Create a new memory buffer.
        /// \param size Size of the buffer, in bytes.
        /// \param alignment Buffer alignment.
        /// \param memory_resource Memory resource the buffer will be allocated from.
        MemoryBuffer(Bytes size, Alignment alignment, MemoryResource& memory_resource = GetDefaultMemoryResource());

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

        /// \brief Get the size of the buffer, in bytes.
        /// \return Returns the size of the buffer, in bytes.
        Bytes GetSize() const;

        /// \brief Get the buffer memory range.
        MemoryRange ToMemoryRange();

        /// \brief Get the buffer memory range.
        ConstMemoryRange ToMemoryRange() const;

        /// \brief Get the buffer memory range.
        ConstMemoryRange ToConstMemoryRange() const;

        /// \brief Get the underlying strongly-typed pointer.
        template <typename TType>
        TType* To();

        /// \brief Get the underlying strongly-typed pointer.
        template <typename TType>
        const TType* To() const;

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

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two memory buffers.
    void swap(syntropy::MemoryBuffer& lhs, syntropy::MemoryBuffer& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryBuffer.

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
        Memory::Move(buffer_, other.buffer_);       // Copy the buffer content.
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

    inline MemoryRange MemoryBuffer::ToMemoryRange()
    {
        return buffer_;
    }

    inline ConstMemoryRange MemoryBuffer::ToMemoryRange() const
    {
        return buffer_;
    }

    inline ConstMemoryRange MemoryBuffer::ToConstMemoryRange() const
    {
        return buffer_;
    }

    template <typename TType>
    inline TType* MemoryBuffer::To()
    {
        return buffer_.Begin().As<TType>();
    }

    template <typename TType>
    inline const TType* MemoryBuffer::To() const
    {
        return buffer_.Begin().As<const TType>();
    }

    inline Bytes MemoryBuffer::GetSize() const
    {
        return buffer_.GetSize();
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

