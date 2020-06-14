
/// \file memory_buffer.h
/// \brief This header is part of the Syntropy memory module. It contains classes and definitions for raw memory buffers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <utility>

#include "syntropy/core/range.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory.h"
#include "syntropy/allocators/memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY BUFFER                                                        */
    /************************************************************************/

    /// \brief Represents a raw memory buffer allocated from a memory resource.
    /// Buffer size is immutable and decided during construction.
    /// \author Raffaele D. Facendola - February 2017
    class MemoryBuffer
    {
    public:

        /// \brief Create a new empty buffer.
        /// The buffer is zero-filled.
        MemoryBuffer(MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Create a new memory buffer.
        /// The buffer is zero-filled.
        /// \param size Size of the buffer, in bytes.
        /// \param memory_resource Memory resource the buffer will be allocated from.
        MemoryBuffer(Bytes size, MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Create a new memory buffer.
        /// The buffer is zero-filled.
        /// \param size Size of the buffer, in bytes.
        /// \param alignment Buffer alignment.
        /// \param memory_resource Memory resource the buffer will be allocated from.
        MemoryBuffer(Bytes size, Alignment alignment, MemoryResource& memory_resource = GetDefaultMemoryResource());

        /// \brief Copy constructor.
        /// This method allocates a new buffer and copy the content of the other instance.
        /// \param other Buffer to copy.
        MemoryBuffer(const MemoryBuffer& other);

        /// \brief Move constructor.
        /// This method moves the other buffer to this instance.
        /// \param other Buffer to move.
        MemoryBuffer(MemoryBuffer&& other);

        /// \brief Unified assignment operator.
        /// This method propagates underlying memory resource as well.
        MemoryBuffer& operator=(MemoryBuffer other);

        /// \brief Destructor.
        ~MemoryBuffer();

        /// \brief Get an iterator to the first element in the range.
        auto Begin();

        /// \brief Get an iterator past the last element in the range.
        auto End();

        /// \brief Get an iterator to the first element in the range.
        auto Begin() const;

        /// \brief Get an iterator past the last element in the range.
        auto End() const;

        /// \brief Get the underlying data range.
        MemoryRange GetData();

        /// \brief Get the underlying const data range.
        ConstMemoryRange GetData() const;

        /// \brief Get the underlying const data range.
        ConstMemoryRange GetConstData() const;

        /// \brief Get the size of the buffer, in bytes.
        /// \return Returns the size of the buffer, in bytes.
        Bytes GetSize() const;

        /// \brief Get the buffer alignment.
        /// \return Returns the buffer alignment.
        Alignment GetAlignment() const;

        /// \brief Access the memory resource this buffer is allocated on.
        MemoryResource& GetMemoryResource() const;

        /// \brief Swap the content of this buffer with another one.
        /// \remarks This method swaps underlying memory resources as well.
        void Swap(MemoryBuffer& other) noexcept;

    private:

        ///< \brief Buffer memory range.
        MemoryRange buffer_;

        ///< \brief Memory resource the buffer was allocated on.
        ObserverPtr<MemoryResource> memory_resource_{ nullptr };

        /// \brief Buffer alignment.
        Alignment alignment_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two memory buffers.
    void swap(MemoryBuffer& lhs, MemoryBuffer& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryBuffer.

    inline MemoryBuffer::MemoryBuffer(MemoryResource& memory_resource)
        : MemoryBuffer(Bytes{}, Alignment{}, memory_resource)
    {

    }

    inline MemoryBuffer::MemoryBuffer(Bytes size, MemoryResource& memory_resource)
        : MemoryBuffer(size, Alignment{}, memory_resource)
    {

    }

    inline MemoryBuffer::MemoryBuffer(Bytes size, Alignment alignment, MemoryResource& memory_resource)
        : memory_resource_(&memory_resource)
        , alignment_(alignment)
        , buffer_(memory_resource.Allocate(size, alignment))
    {
        Memory::Zero(buffer_);
    }

    inline MemoryBuffer::MemoryBuffer(const MemoryBuffer& other)
        : MemoryBuffer(other.GetSize(), other.alignment_, *other.memory_resource_)
    {
        Memory::Copy(buffer_, other.buffer_);
    }

    inline MemoryBuffer::MemoryBuffer(MemoryBuffer&& other)
        : memory_resource_(other.memory_resource_)
        , alignment_(other.alignment_)
        , buffer_(other.buffer_)
    {
        other.buffer_ = {}; // Null-out other's buffer.
    }

    inline MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer other)
    {
        Swap(other);
        return *this;
    }

    inline MemoryBuffer::~MemoryBuffer()
    {
        if (memory_resource_)
        {
            memory_resource_->Deallocate(buffer_, alignment_);
        }
    }

    inline auto MemoryBuffer::Begin()
    {
        return buffer_.Begin();
    }

    inline auto MemoryBuffer::End()
    {
        return buffer_.End();
    }

    inline auto MemoryBuffer::Begin() const
    {
        return buffer_.Begin();
    }

    inline auto MemoryBuffer::End() const
    {
        return buffer_.End();
    }

    inline MemoryRange MemoryBuffer::GetData()
    {
        return buffer_;
    }

    inline ConstMemoryRange MemoryBuffer::GetData() const
    {
        return buffer_;
    }

    inline ConstMemoryRange MemoryBuffer::GetConstData() const
    {
        return buffer_;
    }

    inline Bytes MemoryBuffer::GetSize() const
    {
        return buffer_.GetSize();
    }

    inline Alignment MemoryBuffer::GetAlignment() const
    {
        return alignment_;
    }

    inline MemoryResource& MemoryBuffer::GetMemoryResource() const
    {
        return *memory_resource_;
    }

    inline void MemoryBuffer::Swap(MemoryBuffer& other) noexcept
    {
        std::swap(memory_resource_, other.memory_resource_);
        std::swap(alignment_, other.alignment_);
        std::swap(buffer_, other.buffer_);
    }

    inline void swap(MemoryBuffer& lhs, MemoryBuffer& rhs)
    {
        lhs.Swap(rhs);
    }

}

