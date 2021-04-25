
/// \file byte_span.h
///
/// \brief This header is part of Syntropy memory module.
///        It contains definitions for byte spans.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

#include "syntropy/core/ranges/contiguous_range.h"
#include "syntropy/core/ranges/span.h"

#include "syntropy/math/math.h"

#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/alignment.h"
#include "syntropy/memory/foundation/byte.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BYTE SPAN                                                            */
    /************************************************************************/

    /// \brief A span of read-only bytes.
    using ByteSpan = Span<Byte>;

    /************************************************************************/
    /* RW BYTE SPAN                                                         */
    /************************************************************************/

    /// \brief A span of read-write bytes.
    using RWByteSpan = RWSpan<Byte>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Alignment.
    // ==========

    /// \brief Consume lhs from the front until its first byte is aligned to
    ///        rhs or lhs is exhausted.
    [[nodiscard]] ByteSpan
    Align(Immutable<ByteSpan> lhs, Alignment alignment) noexcept;

    /// \brief Consume lhs from the front until its first byte is aligned to
    ///        rhs or lhs is exhausted.
    [[nodiscard]] RWByteSpan
    Align(Immutable<RWByteSpan> lhs, Alignment alignment) noexcept;

    /// \brief Consume lhs from the back until its size is a multiple of size
    ///        or lhs is exhausted.
    [[nodiscard]] ByteSpan
    Floor(Immutable<ByteSpan> lhs, Bytes size) noexcept;

    /// \brief Consume lhs from the back until its size is a multiple of size
    ///        or lhs is exhausted.
    [[nodiscard]] RWByteSpan
    Floor(Immutable<RWByteSpan> lhs, Bytes size) noexcept;

    // Conversions.
    // ============

    /// \brief Get the read-only object representation of rhs.
    ///
    /// An object representation is the sequence of bytes starting from the
    /// object address.
    ///
    /// \remarks If rhs type is not exactly TObject, the behavior of this
    ///          method is undefined.
    template <typename TObject>
    [[nodiscard]] ByteSpan
    BytesOf(Immutable<TObject> rhs) noexcept;

    /// \brief Get the read-write object representation of rhs.
    ///
    /// An object representation is the sequence of bytes starting from
    /// the object address.
    /// If rhs type is not exactly TObject, the behavior of this method
    /// is undefined.
    template <typename TObject>
    [[nodiscard]] RWByteSpan
    BytesOf(Mutable<TObject> rhs) noexcept;

    /// \brief Get an object TObject from its object representation.
    /// \remarks If rhs is not exactly TObject, accessing the returned value
    ///          results in undefined behavior.
    template <typename TObject>
    [[nodiscard]] Immutable<TObject>
    FromBytesOf(Immutable<ByteSpan> rhs) noexcept;

    /// \brief Get an object TObject from its object representation.
    /// \remarks If rhs is not exactly TObject, accessing the returned value
    ///          results in undefined behavior.
    template <typename TObject>
    [[nodiscard]] Mutable<TObject>
    FromBytesOf(Immutable<RWByteSpan> rhs) noexcept;

    /// \brief Get the object representation of elements in a contiguous range.
    template <Ranges::ContiguousRange TRange>
    [[nodiscard]] auto
    RangeBytesOf(Immutable<TRange> rhs) noexcept;

    /// \brief Get the object representation of elements in a contiguous range.
    /// \remarks The range object representation of a ByteSpan is the span
    ///          itself.
    [[nodiscard]] Immutable<ByteSpan>
    RangeBytesOf(Immutable<ByteSpan> rhs) noexcept;

    /// \brief Get the object representation of elements in a contiguous range.
    /// \remarks The range object representation of a ByteSpan is the span
    ///          itself.
    [[nodiscard]] Immutable<RWByteSpan>
    RangeBytesOf(Immutable<RWByteSpan> rhs) noexcept;

    /// \brief Get a contiguous range of strongly-typed elements from its
    ///        range object representation.
    /// \remarks If rhs is not exactly a range TRange, accessing the returned
    ///          value results in undefined behavior.
    template <Ranges::ContiguousRange TRange>
    [[nodiscard]] TRange
    FromRangeBytesOf(Immutable<ByteSpan> rhs) noexcept;

    /// \brief Get a contiguous range of strongly-typed elements from its
    ///        range object representation.
    /// \remarks If rhs is not exactly a range TRange, accessing the returned
    ///          value results in undefined behavior.
    template <Ranges::ContiguousRange TRange>
    [[nodiscard]] TRange
    FromRangeBytesOf(Immutable<ByteSpan> rhs) noexcept;

    /// \brief Get a contiguous range of strongly-typed elements from its
    ///        range object representation.
    /// \remarks If rhs is not exactly a range TRange, accessing the returned
    ///          value results in undefined behavior.
    template <Ranges::ContiguousRange TRange>
    [[nodiscard]] TRange
    FromRangeBytesOf(Immutable<RWByteSpan> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr ByteSpan
    MakeByteSpan(BytePtr begin, Bytes size) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr ByteSpan
    MakeByteSpan(BytePtr begin, BytePtr end) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr RWByteSpan
    MakeByteSpan(RWBytePtr begin, Bytes size) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr RWByteSpan
    MakeByteSpan(RWBytePtr begin, RWBytePtr end) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    template <typename TType, Int VSize>
    [[nodiscard]] constexpr auto
    MakeByteSpan(TType(&rhs)[VSize]) noexcept;

}

// ===========================================================================

#include "details/byte_span.inl"

// ===========================================================================
