
/// \file memory_span.h
/// \brief This header is part of Syntropy memory module. It contains definitions for memory spans.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/span.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"

namespace syntropy
{
    /************************************************************************/
    /* BYTE PTR                                                             */
    /************************************************************************/

    /// \brief Represents a pointer to a read-only memory location.
    using BytePtr = Pointer<const Byte>;

    /// \brief Represents a pointer to a read-write memory location.
    using RWBytePtr = Pointer<Byte>;

    /************************************************************************/
    /* MEMORY SPAN                                                          */
    /************************************************************************/

    /// \brief Represents a span of read-only raw bytes.
    using MemorySpan = Span<Byte>;

    /// \brief Represents a span of read-write raw bytes.
    using RWMemorySpan = RWSpan<Byte>;

    /************************************************************************/
    /* NON MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Observers.

    /// \brief Get the memory footprint of a span.
    template <typename TElement>
    constexpr Bytes Size(const SpanT<TElement>& span) noexcept;

    /// \brief Check whether a pointer is aligned to a given boundary.
    Bool IsAlignedTo(BytePtr pointer, Alignment alignment) noexcept;

    /// \brief Check whether a memory span is aligned to a given alignment value.
    Bool IsAlignedTo(const MemorySpan& memory_span, Alignment alignment) noexcept;

    // Memory operations.

    /// \brief Align a byte pointer to a given alignment value.
    BytePtr Align(BytePtr pointer, Alignment alignment) noexcept;

    /// \brief Align a byte pointer to a given alignment value.
    RWBytePtr Align(RWBytePtr pointer, Alignment alignment) noexcept;

    /// \brief Consume a memory span from the back until its first byte is aligned to a given boundary or the span is exhausted.
    MemorySpan Align(const MemorySpan& memory_span, Alignment alignment) noexcept;

    /// \brief Consume a memory span from the back until its first byte is aligned to a given boundary or the span is exhausted.
    RWMemorySpan Align(const RWMemorySpan& memory_span, Alignment alignment) noexcept;

    // Conversions.

    /// \brief Convert a read-only memory span to a read-only typed span.
    /// If the memory span doesn't refer to instances of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> ToSpan(const MemorySpan& memory_span) noexcept;

    /// \brief Convert a read-write memory span to a read-only typed span.
    /// If the memory span doesn't refer to instances of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> ToSpan(const RWMemorySpan& memory_span) noexcept;

    /// \brief Convert a read-write memory span to a read-write typed span.
    /// If the memory span doesn't refer to instances of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    RWSpan<TElement> ToRWSpan(const RWMemorySpan& memory_span) noexcept;

    /// \brief Convert an read-only span to a read-only memory span.
    template <typename TElement>
    MemorySpan ToMemorySpan(const Span<TElement>& span) noexcept;

    /// \brief Convert a read-write span to a read-only memory span.
    template <typename TElement>
    MemorySpan ToMemorySpan(const RWSpan<TElement>& span) noexcept;

    /// \brief Convert a read-write span to a read-write memory span.
    template <typename TElement>
    RWMemorySpan ToRWMemorySpan(const RWSpan<TElement>& span) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non member functions.
    // =====================

    // Observers.

    template <typename TElement>
    constexpr Bytes Size(const SpanT<TElement>& span) noexcept
    {
        return Count(span) * BytesOf<TElement>();
    }

    inline Bool IsAlignedTo(BytePtr pointer, Alignment alignment) noexcept
    {
        auto address = reinterpret_cast<Int>(pointer);

        auto alignment_mask = ToInt(alignment) - 1;

        return (address & alignment_mask) == 0;
    }

    inline Bool IsAlignedTo(const MemorySpan& memory_span, Alignment alignment) noexcept
    {
        return IsAlignedTo(memory_span.GetData(), alignment);
    }

    // Memory operations.

    inline BytePtr Align(BytePtr pointer, Alignment alignment) noexcept
    {
        auto address = reinterpret_cast<Int>(pointer);

        auto alignment_mask = ToInt(alignment) - 1;

        address = (address + alignment_mask) & ~alignment_mask;

        return reinterpret_cast<BytePtr>(address);
    }

    inline RWBytePtr Align(RWBytePtr pointer, Alignment alignment) noexcept
    {
        return const_cast<RWBytePtr>(Align(BytePtr{ pointer }, alignment));
    }

    inline MemorySpan Align(const MemorySpan& memory_span, Alignment alignment) noexcept
    {
        auto aligned_data = Align(memory_span.GetData(), alignment);

        return { Math::Min(aligned_data, End(memory_span)), End(memory_span) };
    }

    inline RWMemorySpan Align(const RWMemorySpan& memory_span, Alignment alignment) noexcept
    {
        auto aligned_data = Align(memory_span.GetData(), alignment);

        return { Math::Min(aligned_data, End(memory_span)), End(memory_span) };
    }

    // Conversions.

    template <typename TElement>
    inline Span<TElement> ToSpan(const MemorySpan& memory_span) noexcept
    {
        auto begin = reinterpret_cast<Pointer<const TElement>>(Begin(memory_span));
        auto end = reinterpret_cast<Pointer<const TElement>>(End(memory_span));

        return { begin, end };
    }

    template <typename TElement>
    inline Span<TElement> ToSpan(const RWMemorySpan& memory_span) noexcept
    {
        auto rd_memory_span = ReadOnly(memory_span);

        return ToSpan<TElement>(rd_memory_span);
    }

    template <typename TElement>
    inline RWSpan<TElement> ToRWSpan(const RWMemorySpan& memory_span) noexcept
    {
        auto span = ToSpan<TElement>(memory_span);

        return ReadWrite(span);
    }

    template <typename TElement>
    inline MemorySpan ToMemorySpan(const Span<TElement>& span) noexcept
    {
        auto begin = reinterpret_cast<BytePtr>(Begin(span));
        auto end = reinterpret_cast<BytePtr>(End(span));

        return { begin, end };
    }

    template <typename TElement>
    inline MemorySpan ToMemorySpan(const RWSpan<TElement>& span) noexcept
    {
        auto rd_span = ReadOnly(span);

        return ToMemorySpan(rd_span);
    }

    template <typename TElement>
    inline RWMemorySpan ToRWMemorySpan(const RWSpan<TElement>& span) noexcept
    {
        auto memory_span = ToMemorySpan(span);

        return ReadWrite(memory_span);
    }

}
