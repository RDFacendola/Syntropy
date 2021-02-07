
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
    /* BASE BUFFER                                                          */
    /************************************************************************/

    /// \brief A contiguous sequence of bytes managed by an allocator.
    ///
    /// Memory is acquired during buffer constructor and released upon
    /// destruction.  BaseAllocator is set upon construction an is never
    /// propagated.
    ///
    /// \author Raffaele D. Facendola - February 2017
    template <typename TTraits>
    class BaseBuffer
    {
    public:

        /// \brief Type of the underlying span.
        using TSpan = typename TTraits::TSpan;

        /// \brief Type of the pointer to the underlying data.
        using TData = typename TTraits::TData;

        /// \brief Create a new empty buffer on the current allocator.
        ///
        /// This method participates in overload resolution only if it
        /// represents a read-write buffer.
        template <typename TThis = BaseBuffer>
        BaseBuffer(Mutable<BaseAllocator> allocator
                       = GetScopeAllocator()) noexcept;

        /// \brief Create a new zero-initialized buffer on the current
        ///        allocator.
        ///
        /// This method participates in overload resolution only if it
        /// represents a read-write buffer.
        template <typename TThis = BaseBuffer>
        BaseBuffer(Bytes size,
                   Mutable<BaseAllocator> allocator
                       = GetScopeAllocator()) noexcept;

        /// \brief Create a new aligned zero-initialized buffer on the current
        ///        allocator.
        ///
        /// This method participates in overload resolution only if it
        /// represents a read-write buffer.
        template <typename TThis = BaseBuffer>
        BaseBuffer(Bytes size,
                   Alignment alignment,
                   Mutable<BaseAllocator> allocator
                       = GetScopeAllocator()) noexcept;

        /// \brief Create a buffer which is a copy of rhs.
        template <typename UTraits>
        BaseBuffer(Immutable<BaseBuffer<UTraits>> rhs) noexcept;

        /// \brief Create a buffer which is a copy of rhs with a different
        ///        allocator.
        template <typename UTraits>
        BaseBuffer(Immutable<BaseBuffer<UTraits>> rhs,
                   Mutable<BaseAllocator> allocator) noexcept;

        /// \brief Create a buffer by acquiring the ownership of another
        ///        buffer.
        ///
        /// After this method rhs is guaranteed to be empty.
        template <typename UTraits>
        BaseBuffer(Movable<BaseBuffer<UTraits>> rhs) noexcept;

        /// \brief Create a buffer which is a copy of another buffer, possibly
        ///        by acquiring its ownership.
        ///
        /// If the provided allocator is not equal to rhs allocator, this
        /// method behaves as a copy-constructor.
        /// After this method rhs is left in a valid but unspecified state.
        template <typename UTraits>
        BaseBuffer(Movable<BaseBuffer<UTraits>> rhs,
                   Mutable<BaseAllocator> allocator) noexcept;

        /// \brief Copy-assignment operator.
        ///
        /// The allocator is not propagated.
        template <typename UTraits>
        Mutable<BaseBuffer>
        operator=(Immutable<BaseBuffer<UTraits>> rhs) noexcept;

        /// \brief Move-assignment operator.
        ///
        /// The allocator is not propagated, therefore if rhs allocator is
        /// different that this one's, this method behaves as a
        /// copy-assignment.
        template <typename UTraits>
        Mutable<BaseBuffer>
        operator=(Movable<BaseBuffer<UTraits>> rhs) noexcept;

        /// \brief Destructor.
        ~BaseBuffer() noexcept;

        /// \brief Access buffer data.
        [[nodiscard]] Immutable<TData>
        GetData() const noexcept;

        /// \brief Get the number of bytes in the buffer.
        [[nodiscard]] Bytes
        GetCount() const noexcept;

        /// \brief Get buffer alignment.
        [[nodiscard]] Immutable<Alignment>
        GetAlignment() const noexcept;

        /// \brief Get buffer allocator.
        [[nodiscard]] Mutable<BaseAllocator>
        GetAllocator() const noexcept;

        /// \brief Get the underlying data span.
        [[nodiscard]] Immutable<TSpan>
        GetRange() const noexcept;

        /// \brief Swap the content of this buffer with rhs.
        ///
        /// If both don't share the same allocator, the behavior of this
        /// method is undefined.
        void
        Swap(Mutable<BaseBuffer> rhs) noexcept;

    private:

        /// \brief Allocator the buffer was allocated on.
        RWPtr<BaseAllocator> allocator_{ nullptr };

        /// \brief Buffer data.
        TSpan data_;

        /// \brief BaseBuffer alignment.
        Alignment alignment_ = MaxAlignment();

    };

    /************************************************************************/
    /* BUFFER                                                               */
    /************************************************************************/

    /// \brief Traits for read-only buffers.
    struct BufferTraits
    {
        /// \brief Span type
        using TSpan = ByteSpan;

        /// \brief Pointer type.
        using TData = Ptr<Byte>;
    };

    /// \brief Represents a read-only buffer.
    using Buffer = BaseBuffer<BufferTraits>;

    /************************************************************************/
    /* RW BUFFER                                                            */
    /************************************************************************/

    /// \brief Traits for read-write buffers.
    struct RWBufferTraits
    {
        /// \brief Span type
        using TSpan = RWByteSpan;

        /// \brief Pointer type.
        using TData = RWPtr<Byte>;
    };

    /// \brief Represents a read-write buffer.
    using RWBuffer = BaseBuffer<RWBufferTraits>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Convert rhs to a read-only buffer.
    template <typename TTraits>
    [[nodiscard]] Buffer
    ToReadOnly(Movable<BaseBuffer<TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-write buffer.
    ///
    /// \remarks BaseBuffer has exclusive ownership of the underlying data,
    ///          making this conversion always unsafe.
    template <typename TTraits>
    [[nodiscard]] RWBuffer
    ToReadWrite(Immutable<BaseBuffer<TTraits>> rhs) noexcept = delete;

}

// ===========================================================================

#include "buffer.inl"

// ===========================================================================
