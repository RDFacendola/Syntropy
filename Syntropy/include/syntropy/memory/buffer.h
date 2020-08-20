
/// \file buffer.h
/// \brief This header is part of the Syntropy memory module. It contains classes and definitions for raw memory buffers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory.h"
#include "syntropy/allocators/allocator.h"
#include "syntropy/diagnostics/assert.h"

namespace Syntropy
{
    /************************************************************************/
    /* BUFFER                                                               */
    /************************************************************************/

    /// \brief Represents a contiguous sequence of bytes.
    /// \author Raffaele D. Facendola - February 2017
    class Buffer
    {
    public:

        /// \brief Create a new empty buffer.
        /// The buffer is zero-filled.
        Buffer(Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Create a new memory buffer.
        /// The buffer is zero-filled.
        /// \param size Size of the buffer, in bytes.
        /// \param allocator Allocator the buffer will be allocated from.
        Buffer(Bytes size, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Create a new memory buffer.
        /// The buffer is zero-filled.
        /// \param size Size of the buffer, in bytes.
        /// \param alignment Buffer alignment.
        /// \param allocator Allocator the buffer will be allocated from.
        Buffer(Bytes size, Alignment alignment, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Copy constructor.
        /// This method allocates a new buffer and copy the content to the other instance.
        /// \param other Buffer to copy.
        Buffer(const Buffer& other) noexcept;

        /// \brief Move constructor.
        /// This method moves the other buffer to this instance.
        /// \param other Buffer to move.
        Buffer(Buffer&& other) noexcept;

        /// \brief Unified assignment operator.
        /// This method propagates underlying allocator as well.
        Buffer& operator=(Buffer other) noexcept;

        /// \brief Destructor.
        ~Buffer() noexcept;

        /// \brief Access the underlying byte span.
        ByteSpan GetData() const noexcept;

        /// \brief Access the underlying byte span.
        RWByteSpan GetData() noexcept;

        /// \brief Get the buffer alignment.
        /// \return Returns the buffer alignment.
        Alignment GetAlignment() const noexcept;

        /// \brief Access the allocator this buffer is allocated on.
        Allocator& GetAllocator() const noexcept;

        /// \brief Swap the content of this buffer with another one.
        /// \remarks This method swaps underlying allocator as well.
        void Swap(Buffer& other) noexcept;

    private:

        /// \brief Buffer.
        RWByteSpan buffer_;

        /// \brief Allocator the buffer was allocated on.
        RWPointer<Allocator> allocator_{ nullptr };

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
        Bytes Size(const Buffer& span) noexcept;
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Buffer.
    // =======

    inline Buffer::Buffer(Allocator& allocator) noexcept
        : Buffer(Bytes{}, Alignment{}, allocator)
    {

    }

    inline Buffer::Buffer(Bytes size, Allocator& allocator) noexcept
        : Buffer(size, Alignment{}, allocator)
    {

    }

    inline Buffer::Buffer(Bytes size, Alignment alignment, Allocator& allocator) noexcept
        : allocator_(&allocator)
        , alignment_(alignment)
        , buffer_(allocator.Allocate(size, alignment))
    {
        SYNTROPY_ASSERT(Memory::Size(buffer_) == size);     // Out of memory.

        Memory::Zero(buffer_);
    }

    inline Buffer::Buffer(const Buffer& other) noexcept
        : Buffer(Memory::Size(other), other.alignment_, *other.allocator_)
    {
        Memory::Copy(buffer_, other.buffer_);
    }

    inline Buffer::Buffer(Buffer&& other) noexcept
        : allocator_(other.allocator_)
        , alignment_(other.alignment_)
        , buffer_(other.buffer_)
    {
        other.buffer_ = {};
    }

    inline Buffer& Buffer::operator=(Buffer other) noexcept
    {
        Swap(other);

        return *this;
    }

    inline Buffer::~Buffer() noexcept
    {
        allocator_->Deallocate(buffer_, alignment_);
    }

    inline ByteSpan Buffer::GetData() const noexcept
    {
        return buffer_;
    }

    inline RWByteSpan Buffer::GetData() noexcept
    {
        return buffer_;
    }

    inline Alignment Buffer::GetAlignment() const noexcept
    {
        return alignment_;
    }

    inline Allocator& Buffer::GetAllocator() const noexcept
    {
        return *allocator_;
    }

    inline void Buffer::Swap(Buffer& other) noexcept
    {
        std::swap(allocator_, other.allocator_);
        std::swap(alignment_, other.alignment_);
        std::swap(buffer_, other.buffer_);
    }

    // Memory.
    // =======

    inline Bytes Memory::Size(const Buffer& span) noexcept
    {
        return Memory::Size(span.GetData());
    }

}

