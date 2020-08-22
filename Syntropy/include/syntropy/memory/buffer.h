
/// \file buffer.h
/// \brief This header is part of the Syntropy memory module. It contains classes and definitions for raw memory buffers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/language.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/memory.h"
#include "syntropy/allocators/allocator.h"
#include "syntropy/diagnostics/assert.h"

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BUFFER                                                               */
    /************************************************************************/

    /// \brief A contiguous sequence of bytes allocated by an allocator.
    /// Allocator is set upon construction an is never propagated between instances.
    /// \author Raffaele D. Facendola - February 2017
    class Buffer
    {
    public:

        /// \brief Create a new empty buffer.
        Buffer(Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Create a new memory buffer with unspecified content.
        Buffer(Bytes size, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Create a new aligned memory buffer with unspecified content.
        Buffer(Bytes size, Alignment alignment, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Create a buffer which is a copy of rhs.
        Buffer(Reference<Buffer> rhs) noexcept;

        /// \brief Create a buffer which is a copy of rhs with a different allocator.
        Buffer(Reference<Buffer> rhs, Allocator& allocator) noexcept;

        /// \brief Create a buffer by transferring ownership of resources.
        /// After this method rhs is guaranteed to be valid and empty.
        Buffer(Buffer&& other) noexcept;

        /// \brief Create a buffer by transferring ownership of resources.
        /// If the provided allocator is not equal to rhs allocator, this method behaves as a copy-constructor.
        /// After this method rhs is left in a valid but unspecified state.
        Buffer(Buffer&& other, Allocator& allocator) noexcept;

        /// \brief Copy-assignment operator.
        /// Rhs allocator is not propagated.
        /// \return Returns a reference to this.
        Buffer& operator=(Reference<Buffer> rhs) noexcept;

        /// \brief Move-assignment operator.
        /// If rhs allocator is different than this instance allocator, this method behaves as a copy-assignment.
        /// \return Returns a reference to this.
        Buffer& operator=(Buffer&& rhs) noexcept;

        /// \brief Destructor.
        ~Buffer() noexcept;

        /// \brief Read-only access to buffer data.
        ByteSpan GetData() const noexcept;

        /// \brief Read-write access to buffer data.
        RWByteSpan GetData() noexcept;

        /// \brief Get buffer alignment.
        Alignment GetAlignment() const noexcept;

        /// \brief Get buffer allocator.
        Allocator& GetAllocator() const noexcept;

        /// \brief Swap the content of this buffer with rhs.
        /// If both don't share the same allocator, the behavior of this method is undefined.
        void Swap(RWReference<Buffer> rhs) noexcept;

    private:

        /// \brief Allocator the buffer was allocated on.
        RWPointer<Allocator> allocator_{ nullptr };

        /// \brief Buffer data.
        RWByteSpan data_;

        /// \brief Buffer alignment.
        Alignment alignment_ = MaxAlignment();

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get the memory footprint of a buffer.
    Bytes Size(const Buffer& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Buffer.
    // =======

    inline Buffer::Buffer(Allocator& allocator) noexcept
        : Buffer(ToBytes(0), MaxAlignment(), allocator)
    {

    }

    inline Buffer::Buffer(Bytes size, Allocator& allocator) noexcept
        : Buffer(size, MaxAlignment(), allocator)
    {

    }

    inline Buffer::Buffer(Bytes size, Alignment alignment, Allocator& allocator) noexcept
        : allocator_(&allocator)
        , data_(allocator.Allocate(size, alignment))
        , alignment_(alignment)
    {
        SYNTROPY_ASSERT(Size(data_) == size);       // Out of memory?
    }

    inline Buffer::Buffer(Reference<Buffer> rhs) noexcept
        : Buffer(rhs, rhs.GetAllocator())
    {

    }

    inline Buffer::Buffer(Reference<Buffer> rhs, Allocator& allocator) noexcept
        : Buffer(Size(rhs), rhs.GetAlignment(), allocator)
    {
        Copy(data_, rhs.data_);
    }

    inline Buffer::Buffer(Buffer&& rhs) noexcept
        : allocator_(rhs.allocator_)
        , alignment_(rhs.alignment_)
    {
        std::swap(data_, rhs.data_);
    }

    inline Buffer::Buffer(Buffer&& rhs, Allocator& allocator) noexcept
        : allocator_(allocator_)
        , alignment_(rhs.alignment_)
    {
        // Either move or copy.

        if (allocator_ == rhs.allocator_)
        {
            std::swap(data_, rhs.data_);
        }
        else
        {
            Copy(data_, rhs.data_);
        }
    }

    inline Buffer& Buffer::operator=(Reference<Buffer> rhs) noexcept
    {
        if (this != &rhs)
        {
            if (Size(*this) != Size(rhs))
            {
                data_ = allocator_->Allocate(Size(rhs), rhs.GetAlignment());
            }

            Copy(data_, rhs.GetData());
        }

        return *this;
    }

    inline Buffer& Buffer::operator=(Buffer&& rhs) noexcept
    {
        if (allocator_ == rhs.allocator_)
        {
            Swap(rhs);                          // Move by swap.
        }
        else
        {
            *this = ReadOnly(rhs);              // Copy reference.
        }

        return *this;
    }

    inline Buffer::~Buffer() noexcept
    {
        allocator_->Deallocate(data_, alignment_);
    }

    inline ByteSpan Buffer::GetData() const noexcept
    {
        return data_;
    }

    inline RWByteSpan Buffer::GetData() noexcept
    {
        return data_;
    }

    inline Alignment Buffer::GetAlignment() const noexcept
    {
        return alignment_;
    }

    inline Allocator& Buffer::GetAllocator() const noexcept
    {
        return *allocator_;
    }
  
    inline void Buffer::Swap(RWReference<Buffer> rhs) noexcept
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(allocator_ == rhs.allocator_, "Buffers must share the same allocator.");

        std::swap(data_, rhs.data_);
        std::swap(alignment_, rhs.alignment_);
    }

    // Non-member functions.
    // =====================

    inline Bytes Size(const Buffer& span) noexcept
    {
        return Memory::Size(span.GetData());
    }

}

