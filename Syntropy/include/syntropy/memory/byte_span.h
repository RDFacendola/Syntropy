
/// \file byte_span.h
/// \brief This header is part of Syntropy memory module. It contains definitions for byte spans.
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
    /* BYTE SPAN                                                            */
    /************************************************************************/

    /// \brief Represents a span of read-only bytes.
    using ByteSpan = Span<Byte>;

    /// \brief Represents a span of read-write bytes.
    using RWByteSpan = RWSpan<Byte>;

    /************************************************************************/
    /* NON MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Observers.

    /// \brief Get the memory footprint of a span.
    template <typename TElement>
    constexpr Bytes Size(const SpanT<TElement>& span) noexcept;

    /// \brief Check whether a pointer is aligned to a given boundary.
    Bool IsAlignedTo(BytePtr pointer, Alignment alignment) noexcept;

    /// \brief Check whether a byte span is aligned to a given alignment value.
    /// If the provided span is empty the behavior of this method is undefined.
    Bool IsAlignedTo(const ByteSpan& byte_span, Alignment alignment) noexcept;

    // Memory operations.

    /// \brief Move a byte pointer forward until it gets aligned to a specified value.
    BytePtr Align(BytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer forward until it gets aligned to a specified value.
    RWBytePtr Align(RWBytePtr pointer, Alignment alignment) noexcept;

    /// \brief Consume a byte span from the back until its first byte is aligned to a given boundary or the span is exhausted.
    ByteSpan Align(const ByteSpan& byte_span, Alignment alignment) noexcept;

    /// \brief Consume a byte span from the back until its first byte is aligned to a given boundary or the span is exhausted.
    RWByteSpan Align(const RWByteSpan& byte_span, Alignment alignment) noexcept;

    // Conversions.

    /// \brief Convert a pointer to a byte pointer.
    template <typename TType>
    BytePtr ToBytePtr(Pointer<const TType> pointer) noexcept;

    /// \brief Convert a read-write pointer to a read-write byte pointer.
    template <typename TType>
    RWBytePtr ToRWBytePtr(Pointer<TType> pointer) noexcept;

    /// \brief Convert a read-only byte span to a read-only typed span.
    /// If the byte span doesn't refer to instances of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> ToSpan(const ByteSpan& byte_span) noexcept;

    /// \brief Convert a read-write byte span to a read-write typed span.
    /// If the byte span doesn't refer to instances of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    RWSpan<TElement> ToRWSpan(const RWByteSpan& byte_span) noexcept;

    /// \brief Convert an read-only span to a read-only byte span.
    template <typename TElement>
    ByteSpan ToByteSpan(const SpanT<TElement>& span) noexcept;

    /// \brief Convert a read-write span to a read-write byte span.
    template <typename TElement>
    RWByteSpan ToRWByteSpan(const SpanT<TElement>& span) noexcept;

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

    inline Bool IsAlignedTo(const ByteSpan& byte_span, Alignment alignment) noexcept
    {
        return IsAlignedTo(byte_span.GetData(), alignment);
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

    inline ByteSpan Align(const ByteSpan& byte_span, Alignment alignment) noexcept
    {
        auto aligned_data = Align(byte_span.GetData(), alignment);

        return { Math::Min(aligned_data, End(byte_span)), End(byte_span) };
    }

    inline RWByteSpan Align(const RWByteSpan& byte_span, Alignment alignment) noexcept
    {
        auto aligned_data = Align(byte_span.GetData(), alignment);

        return { Math::Min(aligned_data, End(byte_span)), End(byte_span) };
    }

    // Conversions.

    template <typename TType>
    inline BytePtr ToBytePtr(Pointer<const TType> pointer) noexcept
    {
        return reinterpret_cast<BytePtr>(pointer);
    }

    template <typename TType>
    inline RWBytePtr ToRWBytePtr(Pointer<TType> pointer) noexcept
    {
        return reinterpret_cast<RWBytePtr>(pointer);
    }

    template <typename TElement>
    inline Span<TElement> ToSpan(const ByteSpan& byte_span) noexcept
    {
        using TPointer = Pointer<AddConstT<TElement>>;

        auto begin = reinterpret_cast<TPointer>(Begin(byte_span));
        auto end = reinterpret_cast<TPointer>(End(byte_span));

        return { begin, end };
    }

    template <typename TElement>
    inline RWSpan<TElement> ToRWSpan(const RWByteSpan& byte_span) noexcept
    {
        using TRWPointer = Pointer<RemoveConstT<TElement>>;

        auto begin = reinterpret_cast<TRWPointer>(Begin(byte_span));
        auto end = reinterpret_cast<TRWPointer>(End(byte_span));

        return { begin, end };
    }

    template <typename TElement>
    inline ByteSpan ToByteSpan(const SpanT<TElement>& span) noexcept
    {
        auto begin = ToBytePtr(Begin(span));
        auto end = ToBytePtr(End(span));

        return { begin, end };
    }

    template <typename TElement>
    inline RWByteSpan ToRWByteSpan(const SpanT<TElement>& span) noexcept
    {
        auto begin = ToRWBytePtr(Begin(span));
        auto end = ToRWBytePtr(End(span));

        return { begin, end };
    }

}
