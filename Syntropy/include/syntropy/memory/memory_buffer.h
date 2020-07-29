
/// \file memory_buffer.h
/// \brief This header is part of the Syntropy memory module. It contains classes and definitions for raw memory buffers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/byte_span.h"
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
        MemoryBuffer(MemoryResource& memory_resource = GetDefaultMemoryResource()) noexcept;

        /// \brief Create a new memory buffer.
        /// The buffer is zero-filled.
        /// \param size Size of the buffer, in bytes.
        /// \param memory_resource Memory resource the buffer will be allocated from.
        MemoryBuffer(Bytes size, MemoryResource& memory_resource = GetDefaultMemoryResource()) noexcept;

        /// \brief Create a new memory buffer.
        /// The buffer is zero-filled.
        /// \param size Size of the buffer, in bytes.
        /// \param alignment Buffer alignment.
        /// \param memory_resource Memory resource the buffer will be allocated from.
        MemoryBuffer(Bytes size, Alignment alignment, MemoryResource& memory_resource = GetDefaultMemoryResource()) noexcept;

        /// \brief Copy constructor.
        /// This method allocates a new buffer and copy the content to the other instance.
        /// \param other Buffer to copy.
        MemoryBuffer(const MemoryBuffer& other) noexcept;

        /// \brief Move constructor.
        /// This method moves the other buffer to this instance.
        /// \param other Buffer to move.
        MemoryBuffer(MemoryBuffer&& other) noexcept;

        /// \brief Unified assignment operator.
        /// This method propagates underlying memory resource as well.
        MemoryBuffer& operator=(MemoryBuffer other) noexcept;

        /// \brief Destructor.
        ~MemoryBuffer() noexcept;

        /// \brief Access the underlying byte span.
        ByteSpan GetData() const noexcept;

        /// \brief Access the underlying byte span.
        RWByteSpan GetData() noexcept;

        /// \brief Get the buffer alignment.
        /// \return Returns the buffer alignment.
        Alignment GetAlignment() const noexcept;

        /// \brief Access the memory resource this buffer is allocated on.
        MemoryResource& GetMemoryResource() const noexcept;

        /// \brief Swap the content of this buffer with another one.
        /// \remarks This method swaps underlying memory resources as well.
        void Swap(MemoryBuffer& other) noexcept;

    private:

        ///< \brief Memory buffer.
        RWByteSpan buffer_;

        ///< \brief Memory resource the buffer was allocated on.
        Pointer<MemoryResource> memory_resource_{ nullptr };

        /// \brief Buffer alignment.
        Alignment alignment_;

    };

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Exposes memory-related definitions.
    namespace Memory
    {
        /// \brief Get the memory footprint of a memory buffer.
        Bytes Size(const MemoryBuffer& span) noexcept;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two memory buffers.
    void swap(MemoryBuffer& lhs, MemoryBuffer& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryBuffer.
    // =============

    inline MemoryBuffer::MemoryBuffer(MemoryResource& memory_resource) noexcept
        : MemoryBuffer(Bytes{}, Alignment{}, memory_resource)
    {

    }

    inline MemoryBuffer::MemoryBuffer(Bytes size, MemoryResource& memory_resource) noexcept
        : MemoryBuffer(size, Alignment{}, memory_resource)
    {

    }

    inline MemoryBuffer::MemoryBuffer(Bytes size, Alignment alignment, MemoryResource& memory_resource) noexcept
        : memory_resource_(&memory_resource)
        , alignment_(alignment)
        , buffer_(memory_resource.Allocate(size, alignment))
    {
        Memory::Zero(buffer_);
    }

    inline MemoryBuffer::MemoryBuffer(const MemoryBuffer& other) noexcept
        : MemoryBuffer(Memory::Size(other), other.alignment_, *other.memory_resource_)
    {
        Memory::Copy(buffer_, other.buffer_);
    }

    inline MemoryBuffer::MemoryBuffer(MemoryBuffer&& other) noexcept
        : memory_resource_(other.memory_resource_)
        , alignment_(other.alignment_)
        , buffer_(other.buffer_)
    {
        other.buffer_ = {}; // Null-out other's buffer.
    }

    inline MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer other) noexcept
    {
        Swap(other);

        return *this;
    }

    inline MemoryBuffer::~MemoryBuffer() noexcept
    {
        if (memory_resource_)
        {
            memory_resource_->Deallocate(buffer_, alignment_);
        }
    }

    inline ByteSpan MemoryBuffer::GetData() const noexcept
    {
        return buffer_;
    }

    inline RWByteSpan MemoryBuffer::GetData() noexcept
    {
        return buffer_;
    }

    inline Alignment MemoryBuffer::GetAlignment() const noexcept
    {
        return alignment_;
    }

    inline MemoryResource& MemoryBuffer::GetMemoryResource() const noexcept
    {
        return *memory_resource_;
    }

    inline void MemoryBuffer::Swap(MemoryBuffer& other) noexcept
    {
        std::swap(memory_resource_, other.memory_resource_);
        std::swap(alignment_, other.alignment_);
        std::swap(buffer_, other.buffer_);
    }

    // Memory.
    // =======

    inline Bytes Memory::Size(const MemoryBuffer& span) noexcept
    {
        return Memory::Size(span.GetData());
    }

    // Non-member functions.
    // =====================

    inline void swap(MemoryBuffer& lhs, MemoryBuffer& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}

