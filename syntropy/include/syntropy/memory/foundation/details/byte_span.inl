
/// \file byte_span.inl
///
/// \author Raffaele D. Facendola - July 2020.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    [[nodiscard]] inline ByteSpan
    Memory
    ::Align(Immutable<ByteSpan> lhs, Alignment alignment) noexcept
    {
        auto begin = Align(Ranges::Data(lhs), alignment);
        auto end = Ranges::Data(lhs) + Ranges::Count(lhs);

        return { Math::Min(begin, end), end };
    }

    [[nodiscard]] inline RWByteSpan
    Memory
    ::Align(Immutable<RWByteSpan> lhs, Alignment alignment) noexcept
    {
        return ToReadWrite(Align(ToReadOnly(lhs), alignment));
    }

    [[nodiscard]] inline ByteSpan
    Memory
    ::Floor(Immutable<ByteSpan> lhs, Bytes size) noexcept
    {
        auto floor_size = Math::Floor(Ranges::Count(lhs), ToInt(size));

        return Ranges::Front(lhs, floor_size);
    }

    [[nodiscard]] inline RWByteSpan
    Memory
    ::Floor(Immutable<RWByteSpan> lhs, Bytes size) noexcept
    {
        return ToReadWrite(Floor(ToReadOnly(lhs), size));
    }

    template <typename TObject>
    [[nodiscard]] inline ByteSpan
    Memory
    ::BytesOf(Immutable<TObject> rhs) noexcept
    {
        auto data = ToBytePtr(PtrOf(rhs));
        auto size = SizeOf<TObject>();

        return { data, size };
    }

    template <typename TObject>
    [[nodiscard]] inline RWByteSpan
    Memory
    ::BytesOf(Mutable<TObject> rhs) noexcept
    {
        return ToReadWrite(BytesOf(ToImmutable(rhs)));
    }

    template <typename TObject>
    [[nodiscard]] inline Immutable<TObject>
    Memory
    ::FromBytesOf(Immutable<ByteSpan> rhs) noexcept
    {
        return *FromTypelessPtr<TObject>(Ranges::Data(rhs));
    }

    template <typename TObject>
    [[nodiscard]] inline Mutable<TObject>
    Memory
    ::FromBytesOf(Immutable<RWByteSpan> rhs) noexcept
    {
        return *FromTypelessPtr<TObject>(Ranges::Data(rhs));
    }

    template <ContiguousRange TRange>
    [[nodiscard]] inline auto
    Memory
    ::RangeBytesOf(Immutable<TRange> rhs) noexcept
    {
        using ElementType = RangeElementTypeOf<TRange>;

        auto data = ToBytePtr(Ranges::Data(rhs));
        auto size = SizeOf<ElementType>() * Ranges::Count(rhs);

        return MakeByteSpan( data, size );
    }

    [[nodiscard]] inline Immutable<ByteSpan>
    Memory
    ::RangeBytesOf(Immutable<ByteSpan> rhs) noexcept
    {
        return rhs;
    }

    [[nodiscard]] inline Immutable<RWByteSpan>
    Memory
    ::RangeBytesOf(Immutable<RWByteSpan> rhs) noexcept
    {
        return rhs;
    }

    template <ContiguousRange TRange>
    [[nodiscard]] inline TRange
    Memory
    ::FromRangeBytesOf(Immutable<ByteSpan> rhs) noexcept
    {
        if constexpr (Templates::IsSame<TRange, ByteSpan>)
        {
            return rhs;
        }
        else
        {
            using ElementType = RangeElementTypeOf<TRange>;

            auto data = FromTypelessPtr<ElementType>(Ranges::Data(rhs));
            auto count = Ranges::Count(rhs) / ToInt(SizeOf<ElementType>());

            return TRange{ data, count };
        }
    }

    template <ContiguousRange TRange>
    [[nodiscard]] inline TRange
    Memory
    ::FromRangeBytesOf(Immutable<RWByteSpan> rhs) noexcept
    {
        return ToReadWrite(FromRangeBytesOf<TRange>(ToReadOnly(rhs)));
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

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
        auto size = VSize * Memory::SizeOf<TType>();

        return MakeByteSpan(data, size);
    }

}

// ===========================================================================
