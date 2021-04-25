
/// \file buffer.h
///
/// \brief This header is part of the Syntropy memory module.
///        It contains definitions for memory buffers.
///
/// \author Raffaele D. Facendola - February 2017.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/diagnostics/foundation/assert.h"

#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/alignment.h"
#include "syntropy/memory/foundation/byte_span.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BUFFER                                                               */
    /************************************************************************/

    /// \brief A contiguous sequence of bytes.
    ///
    /// Memory is acquired during buffer constructor and released upon
    /// destruction.  BaseAllocator is set upon construction an is never
    /// propagated.
    ///
    /// \author Raffaele D. Facendola - February 2017
    class Buffer
    {
    public:

        /// \brief Create a new empty buffer on the current allocator.
        Buffer(Mutable<BaseAllocator> allocator
                   = Memory::GetScopeAllocator()) noexcept;

        /// \brief Create a new zero-initialized buffer on the current
        ///        allocator.
        Buffer(Bytes size,
               Mutable<BaseAllocator> allocator
                   = Memory::GetScopeAllocator()) noexcept;

        /// \brief Create a new aligned zero-initialized buffer on the current
        ///        allocator.
        Buffer(Bytes size,
               Alignment alignment,
               Mutable<BaseAllocator> allocator
                   = Memory::GetScopeAllocator()) noexcept;

        /// \brief Create a buffer which is a copy of rhs.
        Buffer(Immutable<Buffer> rhs) noexcept;

        /// \brief Create a buffer which is a copy of rhs with a different
        ///        allocator.
        Buffer(Immutable<Buffer> rhs,
               Mutable<BaseAllocator> allocator) noexcept;

        /// \brief Create a buffer by acquiring the ownership of another
        ///        buffer.
        ///
        /// After this method rhs is guaranteed to be empty.
        Buffer(Movable<Buffer> rhs) noexcept;

        /// \brief Copy-assignment operator.
        ///
        /// The allocator is not propagated.
        Mutable<Buffer>
        operator=(Immutable<Buffer> rhs) noexcept;

        /// \brief Move-assignment operator.
        ///
        /// The allocator is not propagated, therefore if rhs allocator is
        /// different that this one's, this method behaves as a
        /// copy-assignment.
        Mutable<Buffer>
        operator=(Movable<Buffer> rhs) noexcept;

        /// \brief Destructor.
        ~Buffer() noexcept;

        /// \brief Implicit conversion to ByteSpan.
        operator ByteSpan() const noexcept;

        /// \brief Implicit conversion to RWByteSpan.
        operator RWByteSpan() noexcept;

        /// \brief Access a byte by offset.
        [[nodiscard]] constexpr Mutable<Byte>
        operator[](Bytes offset) noexcept;

        /// \brief Access a byte by offset.
        [[nodiscard]] constexpr Immutable<Byte>
        operator[](Bytes offset) const noexcept;

        /// \brief Access buffer data.
        [[nodiscard]] BytePtr
        GetData() const noexcept;

        /// \brief Access buffer data.
        [[nodiscard]] RWBytePtr
        GetData() noexcept;

        /// \brief Get the number of bytes in the buffer.
        [[nodiscard]] Bytes
        GetCount() const noexcept;

        /// \brief Get buffer alignment.
        [[nodiscard]] Immutable<Alignment>
        GetAlignment() const noexcept;

        /// \brief Get buffer allocator.
        [[nodiscard]] Mutable<BaseAllocator>
        GetAllocator() const noexcept;

        /// \brief Swap the content of two buffers.
        ///
        /// \remarks If the buffers don't share a common allocaor, the behavior
        ///          of this method is undefined.
        void
        Swap(Movable<Buffer> rhs) noexcept;

    private:

        /// \brief Owning allocator.
        RWPtr<BaseAllocator> allocator_{ nullptr };

        /// \brief Buffer data.
        RWByteSpan data_;

        /// \brief Buffer alignment.
        Alignment alignment_ = Memory::MaxAlignment();

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Ranges.
    // =======

    /// \brief Get a read-only view to a buffer.
    [[nodiscard]] ByteSpan
    ViewOf(Immutable<Buffer> buffer) noexcept;

    /// \brief Get a read-write view to a buffer.
    [[nodiscard]] RWByteSpan
    ViewOf(Mutable<Buffer> buffer) noexcept;

    /// \brief Prevent from getting a view to a temporary buffer.
    void
    ViewOf(Immovable<Buffer> buffer) noexcept = delete;

}

// ===========================================================================

#include "details/buffer.inl"

// ===========================================================================
