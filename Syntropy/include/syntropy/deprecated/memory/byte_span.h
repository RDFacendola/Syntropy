
/// \file byte_span.h
/// \brief This header is part of Syntropy memory module. It contains definitions for byte spans and related utility functions.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include <typeinfo>

#include "syntropy/diagnostics/assert.h"

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/foundation/span.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte.h"

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BYTE SPAN                                                            */
    /************************************************************************/

    /// \brief Represents a span of read-only bytes.
    using ByteSpan = Span<Byte>;

    /// \brief Represents a span of read-write bytes.
    using RWByteSpan = RWSpan<Byte>;

    /************************************************************************/
    /* BASIC                                                                */
    /************************************************************************/

    /// \brief Get the memory footprint of rhs.
    template <typename TElement>
    constexpr Bytes Size(const SpanT<TElement>& rhs) noexcept;

    /// \brief Get the read-only object representation of rhs.
    /// An object representation is the sequence of bytes starting from the object address.
    /// If rhs type is not exactly TObject, the behavior of this method is undefined.
    template <typename TObject>
    ByteSpan BytesOf(const TObject& rhs) noexcept;

    /// \brief Get the read-write object representation of rhs.
    /// An object representation is the sequence of bytes starting from the object address.
    /// If rhs type is not exactly TObject, the behavior of this method is undefined.
    template <typename TObject>
    RWByteSpan RWBytesOf(TObject& rhs) noexcept;

    /************************************************************************/
    /* ALIGNMENT                                                            */
    /************************************************************************/

    /// \brief Check whether the first byte in lhs is aligned to alignment
    /// If the provided span is empty the behavior of this method is undefined.
    Bool IsAlignedTo(const ByteSpan& lhs, Alignment alignment) noexcept;

    /// \brief Consume lhs from the front until its first byte is aligned to alignment or lhs is exhausted.
    ByteSpan Align(const ByteSpan& lhs, Alignment alignment) noexcept;

    /// \brief Consume lhs from the front until its first byte is aligned to alignment or lhs is exhausted.
    RWByteSpan Align(const RWByteSpan& lhs, Alignment alignment) noexcept;

    /// \brief Consume lhs from both sides until its first byte is aligned to alignment and its size is a multiple of size or lhs is exhausted.
    ByteSpan Align(const ByteSpan& lhs, Bytes size, Alignment alignment) noexcept;

    /// \brief Consume lhs from both sides until its first byte is aligned to alignment and its size is a multiple of size or lhs is exhausted.
    RWByteSpan Align(const RWByteSpan& lhs, Bytes size, Alignment alignment) noexcept;

    /// \brief Consume lhs from both sides until lhs is aligned as TType and its size is a multiple of TType size or lhs is exhausted.
    template <typename TType>
    ByteSpan AlignAs(const ByteSpan& lhs) noexcept;

    /// \brief Consume lhs from both sides until lhs is aligned as TType and its size is a multiple of TType size or lhs is exhausted.
    template <typename TType>
    RWByteSpan AlignAs(const RWByteSpan& lhs) noexcept;

    /************************************************************************/
    /* SPAN OPERATIONS                                                      */
    /************************************************************************/

    /// \brief Obtain a span consisting of the first "size" bytes of lhs.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr ByteSpan Front(const ByteSpan& lhs, Bytes size) noexcept;

    /// \brief Obtain a span consisting of the first "size" bytes of lhs.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr RWByteSpan Front(const RWByteSpan& lhs, Bytes size) noexcept;

    /// \brief Obtain a sub-span given an offset and a size.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr ByteSpan Select(const ByteSpan& span, Bytes offset, Bytes count) noexcept;

    /// \brief Obtain a sub-span given an offset and a size.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr RWByteSpan Select(const RWByteSpan& span, Bytes offset, Bytes count) noexcept;

    /// \brief Obtain a span consisting of the last "size" bytes of lhs.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr ByteSpan Back(const ByteSpan& lhs, Bytes size) noexcept;

    /// \brief Obtain a span consisting of the last "size" bytes of lhs.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr RWByteSpan Back(const RWByteSpan& lhs, Bytes size) noexcept;

    /// \brief Discard the first "size" bytes in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] ByteSpan PopFront(const ByteSpan& lhs, Bytes size) noexcept;

    /// \brief Discard the first "size" bytes in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] RWByteSpan PopFront(const RWByteSpan& lhs, Bytes size) noexcept;

    /// \brief Discard the last "size" bytes in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] ByteSpan PopBack(const ByteSpan& lhs, Bytes size) noexcept;

    /// \brief Discard the last  "size" bytes in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] RWByteSpan PopBack(const RWByteSpan& lhs, Bytes size) noexcept;

    /// \brief Discard an amount of bytes equal to the memory requirement for TType from lhs front and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] ByteSpan PopFront(const ByteSpan& lhs) noexcept;

    /// \brief Discard an amount of bytes equal to the memory requirement for TType from lhs front and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] RWByteSpan PopFront(const RWByteSpan& lhs) noexcept;

    /// \brief Discard an amount of bytes equal to the memory requirement for TType from lhs back and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] ByteSpan PopBack(const ByteSpan& lhs) noexcept;

    /// \brief Discard an amount of bytes equal to the memory requirement for TType from lhs back and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] RWByteSpan PopBack(const RWByteSpan& lhs) noexcept;

    /// \brief Slice lhs returning a span to the first "size" bytes and a span to the remaining elements of lhs.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] constexpr TupleT<ByteSpan, ByteSpan> SliceFront(const ByteSpan& lhs, Bytes size) noexcept;

    /// \brief Slice lhs returning a span to the first "size" bytes and a span to the remaining elements of lhs.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] constexpr TupleT<RWByteSpan, RWByteSpan> SliceFront(const RWByteSpan& lhs, Bytes size) noexcept;

    /// \brief Slice lhs returning a span to the last "size" bytes and a span to the remaining elements of lhs.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] constexpr TupleT<ByteSpan, ByteSpan> SliceBack(const ByteSpan& lhs, Bytes size) noexcept;

    /// \brief Slice lhs returning a span to the last "size" bytes and a span to the remaining elements of lhs.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] constexpr TupleT<RWByteSpan, RWByteSpan> SliceBack(const RWByteSpan& lhs, Bytes size) noexcept;

    /************************************************************************/
    /* CONVERSION                                                           */
    /************************************************************************/

    /// \brief Convert rhs to a read-only typed span.
    /// If rhs doesn't refer to a span of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> ToSpan(const ByteSpan& rhs) noexcept;

    /// \brief Convert rhs to a read-write typed span.
    /// If rhs doesn't refer to a span of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    RWSpan<TElement> ToRWSpan(const RWByteSpan& rhs) noexcept;

    /// \brief Convert an read-only span to a read-only byte span.
    template <typename TElement>
    ByteSpan ToByteSpan(const SpanT<TElement>& rhs) noexcept;

    /// \brief Convert a read-write span to a read-write byte span.
    template <typename TElement>
    RWByteSpan ToRWByteSpan(const SpanT<TElement>& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Basic.
    // ======

    template <typename TElement>
    constexpr Bytes Size(const SpanT<TElement>& rhs) noexcept
    {
        return Count(rhs) * SizeOf<TElement>();
    }

    template <typename TObject>
    inline ByteSpan BytesOf(const TObject& rhs) noexcept
    {
        auto& type_from = typeid(rhs);
        auto& type_to = typeid(TObject);

        SYNTROPY_UNDEFINED_BEHAVIOR(type_from == type_to, "Dynamic type mismatch.");

        return ByteSpan{ ToBytePtr(&rhs), ToInt(SizeOf<TObject>()) };
    }

    template <typename TObject>
    inline RWByteSpan RWBytesOf(TObject& rhs) noexcept
    {
        auto& type_from = typeid(rhs);
        auto& type_to = typeid(TObject);

        SYNTROPY_UNDEFINED_BEHAVIOR(type_from == type_to, "Dynamic type mismatch.");

        return RWByteSpan{ ToRWBytePtr(&rhs), ToInt(SizeOf<TObject>()) };
    }

    // Alignment.
    // ==========

    inline Bool IsAlignedTo(const ByteSpan& lhs, Alignment alignment) noexcept
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(!IsEmpty(lhs), "Empty spans don't have a well-defined alignment.");

        return IsAlignedTo(Begin(lhs), alignment);
    }

    inline ByteSpan Align(const ByteSpan& lhs, Alignment alignment) noexcept
    {
        auto begin = Align(lhs.GetData(), alignment);
        auto end = End(lhs);

        return { Math::Min(begin, end), end };
    }

    inline RWByteSpan Align(const RWByteSpan& lhs, Alignment alignment) noexcept
    {
        return ReadWrite(Align(ReadOnly(lhs), alignment));
    }

    inline ByteSpan Align(const ByteSpan& lhs, Bytes size, Alignment alignment) noexcept
    {
        auto aligned_lhs = Align(lhs, alignment);

        size = Math::Floor(Size(aligned_lhs), size);

        return Front(aligned_lhs, size);
    }

    inline RWByteSpan Align(const RWByteSpan& lhs, Bytes size, Alignment alignment) noexcept
    {
        return ReadWrite(Align(ReadOnly(lhs), size, alignment));
    }

    template <typename TType>
    inline ByteSpan AlignAs(const ByteSpan& lhs) noexcept
    {
        return Align(lhs, SizeOf<TType>(), AlignmentOf<TType>());
    }

    template <typename TType>
    inline RWByteSpan AlignAs(const RWByteSpan& lhs) noexcept
    {
        return ReadWrite(AlignAs<TType>(ReadOnly(lhs)));
    }

    // Span operations.
    // ================

    inline constexpr ByteSpan Select(const ByteSpan& span, Bytes offset, Bytes count) noexcept
    {
        return Select(span, ToInt(offset), ToInt(count));
    }

    inline constexpr RWByteSpan Select(const RWByteSpan& span, Bytes offset, Bytes count) noexcept
    {
        return Select(span, ToInt(offset), ToInt(count));
    }

    inline constexpr ByteSpan Front(const ByteSpan& lhs, Bytes size) noexcept
    {
        return Front(lhs, ToInt(size));
    }

    inline constexpr RWByteSpan Front(const RWByteSpan& lhs, Bytes size) noexcept
    {
        return Front(lhs, ToInt(size));
    }

    inline constexpr ByteSpan Back(const ByteSpan& lhs, Bytes size) noexcept
    {
        return Back(lhs, ToInt(size));
    }

    inline constexpr RWByteSpan Back(const RWByteSpan& lhs, Bytes size) noexcept
    {
        return Back(lhs, ToInt(size));
    }

    inline ByteSpan PopFront(const ByteSpan& lhs, Bytes size) noexcept
    {
        return PopFront(lhs, ToInt(size));
    }

    inline RWByteSpan PopFront(const RWByteSpan& lhs, Bytes size) noexcept
    {
        return PopFront(lhs, ToInt(size));
    }

    inline ByteSpan PopBack(const ByteSpan& lhs, Bytes size) noexcept
    {
        return PopBack(lhs, ToInt(size));
    }

    inline RWByteSpan PopBack(const RWByteSpan& lhs, Bytes size) noexcept
    {
        return PopBack(lhs, ToInt(size));
    }

    template <typename TType>
    inline ByteSpan PopFront(const ByteSpan& lhs) noexcept
    {
        return PopFront(lhs, SizeOf(lhs));
    }

    template <typename TType>
    inline RWByteSpan PopFront(const RWByteSpan& lhs) noexcept
    {
        return PopFront(lhs, SizeOf(lhs));
    }

    template <typename TType>
    inline ByteSpan PopBack(const ByteSpan& lhs) noexcept
    {
        return PopBack(lhs, SizeOf(lhs));
    }

    template <typename TType>
    inline RWByteSpan PopBack(const RWByteSpan& lhs) noexcept
    {
        return PopBack(lhs, SizeOf(lhs));
    }

    inline constexpr TupleT<ByteSpan, ByteSpan> SliceFront(const ByteSpan& lhs, Bytes size) noexcept
    {
        return SliceFront(lhs, ToInt(size));
    }

    inline constexpr TupleT<RWByteSpan, RWByteSpan> SliceFront(const RWByteSpan& lhs, Bytes size) noexcept
    {
        return SliceFront(lhs, ToInt(size));
    }

    inline constexpr TupleT<ByteSpan, ByteSpan> SliceBack(const ByteSpan& lhs, Bytes size) noexcept
    {
        return SliceBack(lhs, ToInt(size));
    }

    inline constexpr TupleT<RWByteSpan, RWByteSpan> SliceBack(const RWByteSpan& lhs, Bytes size) noexcept
    {
        return SliceBack(lhs, ToInt(size));
    }

    // Conversion.
    // ==========

    template <typename TElement>
    inline Span<TElement> ToSpan(const ByteSpan& rhs) noexcept
    {
        auto begin = FromTypeless<TElement>(Begin(rhs));
        auto end = FromTypeless<TElement>(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline RWSpan<TElement> ToRWSpan(const RWByteSpan& rhs) noexcept
    {
        auto begin = FromTypeless<TElement>(Begin(rhs));
        auto end = FromTypeless<TElement>(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline ByteSpan ToByteSpan(const SpanT<TElement>& rhs) noexcept
    {
        auto begin = ToBytePtr(Begin(rhs));
        auto end = ToBytePtr(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline RWByteSpan ToRWByteSpan(const SpanT<TElement>& rhs) noexcept
    {
        auto begin = ToRWBytePtr(Begin(rhs));
        auto end = ToRWBytePtr(End(rhs));

        return { begin, end };
    }

}