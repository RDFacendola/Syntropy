
/// \file byte_span.h
/// \brief This header is part of Syntropy memory module. It contains definitions for byte spans and related utility functions.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include <typeinfo>

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/foundation/span.h"
#include "syntropy/core/foundation/tuple.h"

#include "syntropy/memory/data_size.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BYTE SPAN                                                            */
    /************************************************************************/

    /// \brief Represents a span of read-only bytes.
    using ByteSpan = Span<const Byte>;

    /// \brief Represents a span of read-write bytes.
    using RWByteSpan = Span<Byte>;

    /************************************************************************/
    /* BASIC                                                                */
    /************************************************************************/

    /// \brief Get the memory footprint of rhs.
    template <typename TElement>
    constexpr Bytes Size(Immutable<Span<TElement>> rhs) noexcept;

    /// \brief Get the read-only object representation of rhs.
    /// An object representation is the sequence of bytes starting from the object address.
    /// If rhs type is not exactly TObject, the behavior of this method is undefined.
    template <typename TObject>
    ByteSpan BytesOf(Immutable<TObject> rhs) noexcept;

    /// \brief Get the read-write object representation of rhs.
    /// An object representation is the sequence of bytes starting from the object address.
    /// If rhs type is not exactly TObject, the behavior of this method is undefined.
    template <typename TObject>
    RWByteSpan RWBytesOf(Mutable<TObject> rhs) noexcept;

    /************************************************************************/
    /* ALIGNMENT                                                            */
    /************************************************************************/

    /// \brief Check whether the first byte in lhs is aligned to rhs
    /// If the provided span is empty the behavior of this method is undefined.
    Bool IsAlignedTo(Immutable<ByteSpan> lhs, Immutable<Alignment> rhs) noexcept;

    /// \brief Consume lhs from the front until its first byte is aligned to rhs or lhs is exhausted.
    ByteSpan Align(Immutable<ByteSpan> lhs, Immutable<Alignment> rhs) noexcept;

    /// \brief Consume lhs from the front until its first byte is aligned to rhs or lhs is exhausted.
    RWByteSpan Align(Immutable<RWByteSpan> lhs, Immutable<Alignment> rhs) noexcept;

    /// \brief Consume lhs from both sides until its first byte is aligned to rhs and its size is a multiple of size or lhs is exhausted.
    ByteSpan Align(Immutable<ByteSpan> lhs, Bytes size, Immutable<Alignment> rhs) noexcept;

    /// \brief Consume lhs from both sides until its first byte is aligned to rhs and its size is a multiple of size or lhs is exhausted.
    RWByteSpan Align(Immutable<RWByteSpan> lhs, Bytes size, Immutable<Alignment> rhs) noexcept;

    /// \brief Consume lhs from both sides until lhs is aligned as TType and its size is a multiple of TType size or lhs is exhausted.
    template <typename TType>
    ByteSpan AlignAs(Immutable<ByteSpan> lhs) noexcept;

    /// \brief Consume lhs from both sides until lhs is aligned as TType and its size is a multiple of TType size or lhs is exhausted.
    template <typename TType>
    RWByteSpan AlignAs(Immutable<RWByteSpan> lhs) noexcept;

    /************************************************************************/
    /* SPAN OPERATIONS                                                      */
    /************************************************************************/

    /// \brief Obtain a span consisting of the first "size" bytes of lhs.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr ByteSpan Front(Immutable<ByteSpan> lhs, Bytes size) noexcept;

    /// \brief Obtain a span consisting of the first "size" bytes of lhs.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr RWByteSpan Front(Immutable<RWByteSpan> lhs, Bytes size) noexcept;

    /// \brief Obtain a sub-span given an offset and a size.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr ByteSpan Select(Immutable<ByteSpan> span, Bytes offset, Bytes count) noexcept;

    /// \brief Obtain a sub-span given an offset and a size.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr RWByteSpan Select(Immutable<RWByteSpan> span, Bytes offset, Bytes count) noexcept;

    /// \brief Obtain a span consisting of the last "size" bytes of lhs.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr ByteSpan Back(Immutable<ByteSpan> lhs, Bytes size) noexcept;

    /// \brief Obtain a span consisting of the last "size" bytes of lhs.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    [[nodiscard]] constexpr RWByteSpan Back(Immutable<RWByteSpan> lhs, Bytes size) noexcept;

    /// \brief Discard the first "size" bytes in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] ByteSpan PopFront(Immutable<ByteSpan> lhs, Bytes size) noexcept;

    /// \brief Discard the first "size" bytes in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] RWByteSpan PopFront(Immutable<RWByteSpan> lhs, Bytes size) noexcept;

    /// \brief Discard the last "size" bytes in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] ByteSpan PopBack(Immutable<ByteSpan> lhs, Bytes size) noexcept;

    /// \brief Discard the last  "size" bytes in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] RWByteSpan PopBack(Immutable<RWByteSpan> lhs, Bytes size) noexcept;

    /// \brief Discard an amount of bytes equal to the memory requirement for TType from lhs front and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] ByteSpan PopFront(Immutable<ByteSpan> lhs) noexcept;

    /// \brief Discard an amount of bytes equal to the memory requirement for TType from lhs front and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] RWByteSpan PopFront(Immutable<RWByteSpan> lhs) noexcept;

    /// \brief Discard an amount of bytes equal to the memory requirement for TType from lhs back and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] ByteSpan PopBack(Immutable<ByteSpan> lhs) noexcept;

    /// \brief Discard an amount of bytes equal to the memory requirement for TType from lhs back and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] RWByteSpan PopBack(Immutable<RWByteSpan> lhs) noexcept;

    /// \brief Slice lhs returning a span to the first "size" bytes and a span to the remaining elements of lhs.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] constexpr Tuple<ByteSpan, ByteSpan> SliceFront(Immutable<ByteSpan> lhs, Bytes size) noexcept;

    /// \brief Slice lhs returning a span to the first "size" bytes and a span to the remaining elements of lhs.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] constexpr Tuple<RWByteSpan, RWByteSpan> SliceFront(Immutable<RWByteSpan> lhs, Bytes size) noexcept;

    /// \brief Slice lhs returning a span to the last "size" bytes and a span to the remaining elements of lhs.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] constexpr Tuple<ByteSpan, ByteSpan> SliceBack(Immutable<ByteSpan> lhs, Bytes size) noexcept;

    /// \brief Slice lhs returning a span to the last "size" bytes and a span to the remaining elements of lhs.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    [[nodiscard]] constexpr Tuple<RWByteSpan, RWByteSpan> SliceBack(Immutable<RWByteSpan> lhs, Bytes size) noexcept;

    /************************************************************************/
    /* CONVERSION                                                           */
    /************************************************************************/

    /// \brief Convert rhs to a read-only typed span.
    /// If rhs doesn't refer to a span of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    Span<Const<TElement>> ToSpan(Immutable<ByteSpan> rhs) noexcept;

    /// \brief Convert rhs to a read-write typed span.
    /// If rhs doesn't refer to a span of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> ToRWSpan(Immutable<RWByteSpan> rhs) noexcept;

    /// \brief Convert an read-only span to a read-only byte span.
    template <typename TElement>
    ByteSpan ToByteSpan(Immutable<Span<TElement>> rhs) noexcept;

    /// \brief Convert a read-write span to a read-write byte span.
    template <typename TElement>
    RWByteSpan ToRWByteSpan(Immutable<Span<TElement>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Basic.
    // ======

    template <typename TElement>
    constexpr Bytes Size(Immutable<Span<TElement>> rhs) noexcept
    {
        return Count(rhs) * SizeOf<TElement>();
    }

    template <typename TObject>
    inline ByteSpan BytesOf(Immutable<TObject> rhs) noexcept
    {
        auto& type_from = typeid(rhs);
        auto& type_to = typeid(TObject);

        return ByteSpan{ ToBytePtr(&rhs), ToInt(SizeOf<TObject>()) };
    }

    template <typename TObject>
    inline RWByteSpan RWBytesOf(Mutable<TObject> rhs) noexcept
    {
        auto& type_from = typeid(rhs);
        auto& type_to = typeid(TObject);

        return RWByteSpan{ ToBytePtr(&rhs), ToInt(SizeOf<TObject>()) };
    }

    // Alignment.
    // ==========

    inline Bool IsAlignedTo(Immutable<ByteSpan> lhs, Immutable<Alignment> rhs) noexcept
    {
        return IsAlignedTo(Begin(lhs), rhs);
    }

    inline ByteSpan Align(Immutable<ByteSpan> lhs, Immutable<Alignment> rhs) noexcept
    {
        auto begin = Align(Data(lhs), rhs);
        auto end = End(lhs);

        return { Math::Min(begin, end), end };
    }

    inline RWByteSpan Align(Immutable<RWByteSpan> lhs, Immutable<Alignment> rhs) noexcept
    {
        auto begin = Align(Data(lhs), rhs);
        auto end = End(lhs);

        return { Math::Min(begin, end), end };
    }

    inline ByteSpan Align(Immutable<ByteSpan> lhs, Bytes size, Immutable<Alignment> rhs) noexcept
    {
        auto aligned_lhs = Align(lhs, rhs);

        size = Math::Floor(Size(aligned_lhs), size);

        return Front(aligned_lhs, size);
    }

    inline RWByteSpan Align(Immutable<RWByteSpan> lhs, Bytes size, Immutable<Alignment> rhs) noexcept
    {
        auto aligned_lhs = Align(lhs, rhs);

        size = Math::Floor(Size(aligned_lhs), size);

        return Front(aligned_lhs, size);
    }

    template <typename TType>
    inline ByteSpan AlignAs(Immutable<ByteSpan> lhs) noexcept
    {
        return Align(lhs, SizeOf<TType>(), AlignmentOf<TType>());
    }

    template <typename TType>
    inline RWByteSpan AlignAs(Immutable<RWByteSpan> lhs) noexcept
    {
        return Align(lhs, SizeOf<TType>(), AlignmentOf<TType>());
    }

    // Span operations.
    // ================

    inline constexpr ByteSpan Select(Immutable<ByteSpan> span, Bytes offset, Bytes count) noexcept
    {
        return Select(span, ToInt(offset), ToInt(count));
    }

    inline constexpr RWByteSpan Select(Immutable<RWByteSpan> span, Bytes offset, Bytes count) noexcept
    {
        return Select(span, ToInt(offset), ToInt(count));
    }

    inline constexpr ByteSpan Front(Immutable<ByteSpan> lhs, Bytes size) noexcept
    {
        return Front(lhs, ToInt(size));
    }

    inline constexpr RWByteSpan Front(Immutable<RWByteSpan> lhs, Bytes size) noexcept
    {
        return Front(lhs, ToInt(size));
    }

    inline constexpr ByteSpan Back(Immutable<ByteSpan> lhs, Bytes size) noexcept
    {
        return Back(lhs, ToInt(size));
    }

    inline constexpr RWByteSpan Back(Immutable<RWByteSpan> lhs, Bytes size) noexcept
    {
        return Back(lhs, ToInt(size));
    }

    inline ByteSpan PopFront(Immutable<ByteSpan> lhs, Bytes size) noexcept
    {
        return PopFront(lhs, ToInt(size));
    }

    inline RWByteSpan PopFront(Immutable<RWByteSpan> lhs, Bytes size) noexcept
    {
        return PopFront(lhs, ToInt(size));
    }

    inline ByteSpan PopBack(Immutable<ByteSpan> lhs, Bytes size) noexcept
    {
        return PopBack(lhs, ToInt(size));
    }

    inline RWByteSpan PopBack(Immutable<RWByteSpan> lhs, Bytes size) noexcept
    {
        return PopBack(lhs, ToInt(size));
    }

    template <typename TType>
    inline ByteSpan PopFront(Immutable<ByteSpan> lhs) noexcept
    {
        return PopFront(lhs, SizeOf(lhs));
    }

    template <typename TType>
    inline RWByteSpan PopFront(Immutable<RWByteSpan> lhs) noexcept
    {
        return PopFront(lhs, SizeOf(lhs));
    }

    template <typename TType>
    inline ByteSpan PopBack(Immutable<ByteSpan> lhs) noexcept
    {
        return PopBack(lhs, SizeOf(lhs));
    }

    template <typename TType>
    inline RWByteSpan PopBack(Immutable<RWByteSpan> lhs) noexcept
    {
        return PopBack(lhs, SizeOf(lhs));
    }

    inline constexpr Tuple<ByteSpan, ByteSpan> SliceFront(Immutable<ByteSpan> lhs, Bytes size) noexcept
    {
        return SliceFront(lhs, ToInt(size));
    }

    inline constexpr Tuple<RWByteSpan, RWByteSpan> SliceFront(Immutable<RWByteSpan> lhs, Bytes size) noexcept
    {
        return SliceFront(lhs, ToInt(size));
    }

    inline constexpr Tuple<ByteSpan, ByteSpan> SliceBack(Immutable<ByteSpan> lhs, Bytes size) noexcept
    {
        return SliceBack(lhs, ToInt(size));
    }

    inline constexpr Tuple<RWByteSpan, RWByteSpan> SliceBack(Immutable<RWByteSpan> lhs, Bytes size) noexcept
    {
        return SliceBack(lhs, ToInt(size));
    }

    // Conversion.
    // ==========

    template <typename TElement>
    inline Span<Const<TElement>> ToSpan(Immutable<ByteSpan> rhs) noexcept
    {
        auto begin = FromTypeless<TElement>(Begin(rhs));
        auto end = FromTypeless<TElement>(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline Span<TElement> ToRWSpan(Immutable<RWByteSpan> rhs) noexcept
    {
        auto begin = FromTypeless<TElement>(Begin(rhs));
        auto end = FromTypeless<TElement>(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline ByteSpan ToByteSpan(Immutable<Span<TElement>> rhs) noexcept
    {
        auto begin = ToBytePtr(Begin(rhs));
        auto end = ToBytePtr(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline RWByteSpan ToRWByteSpan(Immutable<Span<TElement>> rhs) noexcept
    {
        auto begin = ToBytePtr(Begin(rhs));
        auto end = ToBytePtr(End(rhs));

        return { begin, end };
    }

}
