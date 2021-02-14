
/// \file buffer.h
///
/// \brief This header is part of the Syntropy memory module.
///        It contains definitions for memory buffers.
///
/// \author Raffaele D. Facendola - February 2017

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/alignment.h"
#include "syntropy/memory/foundation/byte_span.h"

// ===========================================================================

namespace Syntropy::Memory
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
        friend Buffer
        Exchange(Mutable<Buffer> lhs, Mutable<Buffer> rhs) noexcept;

    public:

        /// \brief Create a new empty buffer on the current allocator.
        Buffer(Mutable<BaseAllocator> allocator
                   = GetScopeAllocator()) noexcept;

        /// \brief Create a new zero-initialized buffer on the current
        ///        allocator.
        Buffer(Bytes size,
               Mutable<BaseAllocator> allocator
                   = GetScopeAllocator()) noexcept;

        /// \brief Create a new aligned zero-initialized buffer on the current
        ///        allocator.
        Buffer(Bytes size,
               Alignment alignment,
               Mutable<BaseAllocator> allocator
                   = GetScopeAllocator()) noexcept;

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

    private:

        /// \brief Owning allocator.
        RWPtr<BaseAllocator> allocator_{ nullptr };

        /// \brief Buffer data.
        RWByteSpan data_;

        /// \brief Buffer alignment.
        Alignment alignment_ = MaxAlignment();

    };

}

// ===========================================================================

#include "buffer.inl"

// ===========================================================================
