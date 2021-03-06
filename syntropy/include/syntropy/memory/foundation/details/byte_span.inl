
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

    template <typename TTraits>
    [[nodiscard]] inline BaseByteSpan<TTraits>
    Align(Immutable<BaseByteSpan<TTraits>> lhs,
          Immutable<Alignment> alignment) noexcept
    {
        auto begin = Align(lhs.GetData(), alignment);
        auto end = lhs.data_() + lhs.GetCount();

        return { Math::Min(begin, end), end };
    }

    template <typename TTraits>
    [[nodiscard]] inline BaseByteSpan<TTraits>
    Floor(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<Bytes> size) noexcept
    {
        auto floor_size = Math::Floor(lhs.GetCount(), size);

        return Ranges::Front(lhs, floor_size);
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
        auto data = ToBytePtr(PtrOf(rhs));
        auto size = SizeOf<TObject>();

        return { data, size };
    }

    template <typename TObject, typename TTraits>
    [[nodiscard]] inline Reference<TObject>
    FromBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return *FromTypelessPtr<TObject>(rhs.GetData());
    }

    template <Ranges::ContiguousRange TRange>
    [[nodiscard]] inline auto
    RangeBytesOf(Immutable<TRange> rhs) noexcept
    {
        using RangeElementType = typename Ranges::RangeElementTypeOf<TRange>;

        auto data = ToBytePtr(Ranges::Data(rhs));
        auto size = SizeOf<RangeElementType>() * Ranges::Count(rhs);

        return MakeByteSpan( data, size );
    }

    template <typename TTraits>
    [[nodiscard]] inline BaseByteSpan<TTraits>
    RangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return rhs;
    }

    template <Ranges::ContiguousRange TRange, typename TTraits>
    [[nodiscard]] inline TRange
    FromRangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        if constexpr (Templates::IsSame<TRange, BaseByteSpan<TTraits>>)
        {
            return rhs;
        }
        else
        {
            using RangeElementType = Ranges::RangeElementTypeOf<TRange>;

            auto data = FromTypelessPtr<RangeElementType>(rhs.GetData());
            auto size = rhs.GetCount() / SizeOf<RangeElementType>();

            return TRange{ data, size };
        }
    }

    // Utilities.
    // ==========

    [[nodiscard]] constexpr ByteSpan
    MakeByteSpan(BytePtr begin, Immutable<Bytes> size) noexcept
    {
        return { begin, size };
    }

    [[nodiscard]] constexpr ByteSpan
    MakeByteSpan(BytePtr begin, BytePtr end) noexcept
    {
        return { begin, end };
    }

    [[nodiscard]] constexpr RWByteSpan
    MakeByteSpan(RWBytePtr begin, Immutable<Bytes> size) noexcept
    {
        return { begin, size };
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
