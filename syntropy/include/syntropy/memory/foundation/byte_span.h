
/// \file byte_span.h
///
/// \brief This header is part of Syntropy memory module.
///        It contains definitions for byte spans.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/support/swap.h"

#include "syntropy/core/ranges/contiguous_range.h"

#include "syntropy/math/math.h"

#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/alignment.h"
#include "syntropy/memory/foundation/byte.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE BYTE SPAN                                                       */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of bytes.
    /// \author Raffaele D. Facendola - December 2020.
    template <typename TTraits>
    class BaseByteSpan
    {
    public:

        /// \brief Pointer type.
        using TPointer = typename TTraits::TPointer;

        /// \brief Reference type.
        using TReference = typename TTraits::TReference;

        /// \brief Create an empty byte span.
        constexpr BaseByteSpan() noexcept = default;

        /// \brief Create an empty byte span.
        constexpr BaseByteSpan(Null) noexcept;

        /// \brief Create a byte span given a pointer to the first byte
        ///        and span size.
        constexpr
        BaseByteSpan(TPointer begin, Immutable<Bytes> size) noexcept;

        /// \brief Create a byte span given a pointer to both the first and
        ///        past the last byte.
        constexpr
        BaseByteSpan(TPointer begin, TPointer end) noexcept;

        /// \brief Converting copy constructor.
        template <typename UTraits>
        constexpr
        BaseByteSpan(Immutable<BaseByteSpan<UTraits>> rhs) noexcept;

        /// \brief Default destructor.
        ~BaseByteSpan() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UTraits>
        constexpr Mutable<BaseByteSpan>
        operator=(Immutable<BaseByteSpan<UTraits>> rhs) noexcept;

        ///  \brief Check whether the byte span is non-empty.
        [[nodiscard]] constexpr explicit
        operator Bool() const noexcept;

        /// \brief Access a byte by offset.
        ///
        /// If the provided offset is not within the byte span the behavior
        /// of this method is undefined.
        [[nodiscard]] constexpr TReference
        operator[](Immutable<Bytes> offset) const noexcept;

        /// \brief Access the underlying storage.
        [[nodiscard]] constexpr TPointer
        GetData() const noexcept;

        /// \brief Get the number of elements in the span.
        [[nodiscard]] constexpr Bytes
        GetCount() const noexcept;

    private:

        /// \brief Pointer to the first element.
        TPointer data_{ nullptr };

        /// \brief Number of bytes in the span.
        Bytes size_{ 0 };

    };

    /************************************************************************/
    /* BYTE SPAN                                                            */
    /************************************************************************/

    /// \brief Tag for read-only byte spans.
    struct ByteSpanTraits
    {
        /// \brief Pointer type.
        using TPointer = BytePtr;

        /// \brief Reference type.
        using TReference = Immutable<Byte>;
    };

    /// \brief Represents a span of read-only bytes.
    using ByteSpan = BaseByteSpan<ByteSpanTraits>;

    /************************************************************************/
    /* RW BYTE SPAN                                                         */
    /************************************************************************/

    /// \brief Tag for read-write byte spans.
    struct RWByteSpanTraits
    {
        /// \brief Pointer type.
        using TPointer = RWBytePtr;

        /// \brief Reference type.
        using TReference = Mutable<Byte>;
    };

    /// \brief Represents a span of read-write bytes.
    using RWByteSpan = BaseByteSpan<RWByteSpanTraits>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // BaseByteSpan.
    // =============

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<BaseByteSpan<TTraits>> lhs,
               Immutable<BaseByteSpan<UTraits>> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<BaseByteSpan<TTraits>> lhs,
                Immutable<BaseByteSpan<UTraits>> rhs) noexcept;

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
    template <Ranges::Concepts::ContiguousRange TRange>
    [[nodiscard]] auto
    RangeBytesOf(Immutable<TRange> rhs) noexcept;

    /// \brief Get the object representation of bytes in the contiguous
    ///        range rhs.
    /// \remarks The range-byte representation of a BaseByteSpan is the
    ///          span itself.
    template <typename TTraits>
    [[nodiscard]] BaseByteSpan<TTraits>
    RangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    /// \brief Get a contiguous range of strongly-typed elements from its
    ///        range object representation.
    /// \remarks If rhs is not exactly a range TRange, accessing the returned
    ///          value results in undefined behavior.
    template <Ranges::Concepts::ContiguousRange TRange, typename TTraits>
    [[nodiscard]] TRange
    FromRangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a read-only byte span.
    [[nodiscard]] ByteSpan
    ToReadOnly(Immutable<ByteSpan> rhs) noexcept;

    /// \brief Convert rhs to a read-write byte span.
    /// \remarks If the original memory location is not read-writable,
    ///          accessing the returned values results in undefined behavior.
    [[nodiscard]] RWByteSpan
    ToReadWrite(Immutable<ByteSpan> rhs) noexcept;

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
