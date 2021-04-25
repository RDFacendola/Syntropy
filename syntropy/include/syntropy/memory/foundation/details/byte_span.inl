
/// \file byte_span.inl
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Alignment.
    // ==========

    [[nodiscard]] inline ByteSpan
    Align(Immutable<ByteSpan> lhs, Alignment alignment) noexcept
    {
        auto begin = Align(Ranges::Data(lhs), alignment);
        auto end = Ranges::Data(lhs) + Ranges::Count(lhs);

        return { Math::Min(begin, end), end };
    }

    [[nodiscard]] inline RWByteSpan
    Align(Immutable<RWByteSpan> lhs, Alignment alignment) noexcept
    {
        return ToReadWrite(Align(ToReadOnly(lhs), alignment));
    }

    [[nodiscard]] inline ByteSpan
    Floor(Immutable<ByteSpan> lhs, Bytes size) noexcept
    {
        auto floor_size = Math::Floor(Ranges::Count(lhs), ToInt(size));

        return Ranges::Front(lhs, floor_size);
    }

    [[nodiscard]] inline RWByteSpan
    Floor(Immutable<RWByteSpan> lhs, Bytes size) noexcept
    {
        return ToReadWrite(Floor(ToReadOnly(lhs), size));
    }

    // Conversions.
    // ============

    template <typename TObject>
    [[nodiscard]] inline ByteSpan
    BytesOf(Immutable<TObject> rhs) noexcept
    {
        auto data = ToBytePtr(PtrOf(rhs));
        auto size = SizeOf<TObject>();

        return { data, size };
    }

    template <typename TObject>
    [[nodiscard]] inline RWByteSpan
    BytesOf(Mutable<TObject> rhs) noexcept
    {
        return ToReadWrite(BytesOf(ToImmutable(rhs)));
    }

    template <typename TObject>
    [[nodiscard]] inline Immutable<TObject>
    FromBytesOf(Immutable<ByteSpan> rhs) noexcept
    {
        return *FromTypelessPtr<TObject>(Ranges::Data(rhs));
    }

    template <typename TObject>
    [[nodiscard]] inline Mutable<TObject>
    FromBytesOf(Immutable<RWByteSpan> rhs) noexcept
    {
        return *FromTypelessPtr<TObject>(Ranges::Data(rhs));
    }

    template <Ranges::ContiguousRange TRange>
    [[nodiscard]] inline auto
    RangeBytesOf(Immutable<TRange> rhs) noexcept
    {
        using ElementType = typename Ranges::RangeElementTypeOf<TRange>;

        auto data = ToBytePtr(Ranges::Data(rhs));
        auto size = SizeOf<ElementType>() * Ranges::Count(rhs);

        return MakeByteSpan( data, size );
    }

    [[nodiscard]] inline Immutable<ByteSpan>
    RangeBytesOf(Immutable<ByteSpan> rhs) noexcept
    {
        return rhs;
    }

    [[nodiscard]] inline Immutable<RWByteSpan>
    RangeBytesOf(Immutable<RWByteSpan> rhs) noexcept
    {
        return rhs;
    }

    template <Ranges::ContiguousRange TRange>
    [[nodiscard]] inline TRange
    FromRangeBytesOf(Immutable<ByteSpan> rhs) noexcept
    {
        if constexpr (Templates::IsSame<TRange, ByteSpan>)
        {
            return rhs;
        }
        else
        {
            using ElementType = Ranges::RangeElementTypeOf<TRange>;

            auto data = FromTypelessPtr<ElementType>(Ranges::Data(rhs));
            auto count = Ranges::Count(rhs) / ToInt(SizeOf<ElementType>());

            return TRange{ data, count };
        }
    }

    template <Ranges::ContiguousRange TRange>
    [[nodiscard]] inline TRange
    FromRangeBytesOf(Immutable<RWByteSpan> rhs) noexcept
    {
        return ToReadWrite(FromRangeBytesOf<TRange>(ToReadOnly(rhs)));
    }

    // Utilities.
    // ==========

    [[nodiscard]] constexpr ByteSpan
    MakeByteSpan(BytePtr begin, Bytes size) noexcept
    {
        return { begin, ToInt(size) };
    }

    [[nodiscard]] constexpr ByteSpan
    MakeByteSpan(BytePtr begin, BytePtr end) noexcept
    {
        return { begin, end };
    }

    [[nodiscard]] constexpr RWByteSpan
    MakeByteSpan(RWBytePtr begin, Bytes size) noexcept
    {
        return { begin, ToInt(size) };
    }

    [[nodiscard]] constexpr RWByteSpan
    MakeByteSpan(RWBytePtr begin, RWBytePtr end) noexcept
    {
        return { begin, end };
    }

    template <typename TType, Int VSize>
    [[nodiscard]] constexpr auto
    MakeByteSpan(TType(&rhs)[VSize]) noexcept
    {
        auto data = ToBytePtr(PtrOf(rhs[0]));
        auto size = VSize * SizeOf<TType>();

        return MakeByteSpan(data, size);
    }

}

// ===========================================================================
