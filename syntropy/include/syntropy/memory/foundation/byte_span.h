
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

    /// \brief A contiguous sequence of bytes.
    template <typename TTraits>
    using BaseByteSpan = BaseSpan<Byte, TTraits>;

    /************************************************************************/
    /* BYTE SPAN                                                            */
    /************************************************************************/

    /// \brief Traits for read-only byte spans.
    struct ByteSpanTraits : BaseSpanTraits<BytePtr, Immutable<Byte>, Bytes>
    {

    };

    /// \brief Represents a span of read-only bytes.
    using ByteSpan = BaseByteSpan<ByteSpanTraits>;

    /************************************************************************/
    /* RW BYTE SPAN                                                         */
    /************************************************************************/

    /// \brief Traits for read-write byte spans.
    struct RWByteSpanTraits : BaseSpanTraits<RWBytePtr, Mutable<Byte>, Bytes>
    {

    };

    /// \brief Represents a span of read-write bytes.
    using RWByteSpan = BaseByteSpan<RWByteSpanTraits>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Alignment.
    // ==========

    /// \brief Consume lhs from the front until its first byte is aligned to
    ///        rhs or lhs is exhausted.
    template <typename TTraits>
    [[nodiscard]] BaseByteSpan<TTraits>
    Align(Immutable<BaseByteSpan<TTraits>> lhs,
          Immutable<Alignment> alignment) noexcept;

    /// \brief Consume lhs from the back until its size is a multiple of size
    ///        or lhs is exhausted.
    template <typename TTraits>
    [[nodiscard]] BaseByteSpan<TTraits>
    Floor(Immutable<BaseByteSpan<TTraits>> lhs,
          Immutable<Bytes> size) noexcept;

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
    template <typename TObject, typename TTraits>
    [[nodiscard]] Reference<TObject>
    FromBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    /// \brief Get the object representation of bytes in the contiguous
    ///        range rhs.
    template <Ranges::Concepts::ContiguousRangeView TRangeView>
    [[nodiscard]] auto
    RangeBytesOf(Immutable<TRangeView> rhs) noexcept;

    /// \brief Get the object representation of bytes in the contiguous
    ///        range rhs.
    /// \remarks The range-byte representation of a BaseByteSpan is the
    ///          span itself.
    template <typename TTraits>
    [[nodiscard]] BaseByteSpan<TTraits>
    RangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    /// \brief Get a contiguous range of strongly-typed elements from its
    ///        range object representation.
    /// \remarks If rhs is not exactly a range TRangeView, accessing the returned
    ///          value results in undefined behavior.
    template <Ranges::Concepts::ContiguousRangeView TRangeView, typename TTraits>
    [[nodiscard]] TRangeView
    FromRangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr ByteSpan
    MakeByteSpan(BytePtr begin, Immutable<Bytes> size) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr ByteSpan
    MakeByteSpan(BytePtr begin, BytePtr end) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr RWByteSpan
    MakeByteSpan(RWBytePtr begin, Immutable<Bytes> size) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr RWByteSpan
    MakeByteSpan(RWBytePtr begin, RWBytePtr end) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    template <typename TType, Int VSize>
    [[nodiscard]] constexpr auto
    MakeByteSpan(TType(&rhs)[VSize]) noexcept;

}

// ===========================================================================

#include "byte_span.inl"

// ===========================================================================
