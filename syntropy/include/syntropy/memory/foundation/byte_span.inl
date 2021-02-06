
/// \file byte_span.inl
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE BYTE SPAN                                                       */
    /************************************************************************/

    template <typename TTraits>
    constexpr BaseByteSpan<TTraits>
    ::BaseByteSpan(Null) noexcept
    {

    }

    template <typename TTraits>
    constexpr BaseByteSpan<TTraits>
    ::BaseByteSpan(BaseByteSpan<TTraits>::TPointer begin,
                   Immutable<Bytes> size) noexcept
        : data_(begin)
        , size_(size)
    {

    }

    template <typename TTraits>
    constexpr BaseByteSpan<TTraits>
    ::BaseByteSpan(BaseByteSpan<TTraits>::TPointer begin,
                   BaseByteSpan<TTraits>::TPointer end) noexcept
        : BaseByteSpan(begin, ToBytes(end - begin))
    {

    }

    template <typename TTraits>
    template <typename UTraits>
    constexpr BaseByteSpan<TTraits>
    ::BaseByteSpan(Immutable<BaseByteSpan<UTraits>> rhs) noexcept
        : data_(rhs.data_)
        , size_(rhs.size_)
    {

    }

    template <typename TTraits>
    template <typename UTraits>
    constexpr Mutable<BaseByteSpan<TTraits>> BaseByteSpan<TTraits>
    ::operator=(Immutable<BaseByteSpan<UTraits>> rhs) noexcept
    {
        data_ = rhs.data_;
        size_ = rhs.size_;

        return *this;
    }

     template <typename TTraits>
     [[nodiscard]] constexpr BaseByteSpan<TTraits>
     ::operator Bool() const noexcept
     {
         return size_ > ToBytes(0);
     }

    template <typename TTraits>
    [[nodiscard]] constexpr typename BaseByteSpan<TTraits>::TReference
    BaseByteSpan<TTraits>
    ::operator[](Immutable<Bytes> offset) const noexcept
    {
        return data_[offset];
    }

    template <typename TTraits>
    [[nodiscard]] constexpr typename BaseByteSpan<TTraits>::TPointer
    BaseByteSpan<TTraits>
    ::GetData() const noexcept
    {
        return data_;
    }

    template <typename TTraits>
    [[nodiscard]] constexpr Bytes BaseByteSpan<TTraits>::
    GetCount() const noexcept
    {
        return size_;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // BaseByteSpan.
    // =============

    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<BaseByteSpan<TTraits>> lhs,
               Immutable<BaseByteSpan<UTraits>> rhs) noexcept
    {
        return Ranges::AreEquivalent(lhs, rhs);
    }

    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<BaseByteSpan<TTraits>> lhs,
                Immutable<BaseByteSpan<UTraits>> rhs) noexcept
    {
        using namespace Ranges;

        return Ranges::Compare(lhs, rhs);
    }

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

    template <Ranges::Concepts::ContiguousRange TRange>
    [[nodiscard]] inline auto
    RangeBytesOf(Immutable<TRange> rhs) noexcept
    {
        using TRangeElement
            = typename Ranges::Templates::RangeElementValueType<TRange>;

        auto data = ToBytePtr(Ranges::Data(rhs));
        auto size = SizeOf<TRangeElement>() * Ranges::Count(rhs);

        return MakeByteSpan( data, size );
    }

    template <typename TTraits>
    [[nodiscard]] inline BaseByteSpan<TTraits>
    RangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return rhs;
    }

    template <Ranges::Concepts::ContiguousRange TRange, typename TTraits>
    [[nodiscard]] inline TRange
    FromRangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        if constexpr (Concepts::SameAs<TRange, BaseByteSpan<TTraits>>)
        {
            return rhs;
        }
        else
        {
            using TRangeElement
                = Ranges::Templates::RangeElementValueType<TRange>;

            auto data = FromTypelessPtr<TRangeElement>(rhs.GetData());
            auto size = rhs.GetCount() / SizeOf<TRangeElement>();

            return TRange{ data, size };
        }
    }

    // Access.
    // =======

    [[nodiscard]] inline ByteSpan
    ToReadOnly(Immutable<ByteSpan> rhs) noexcept
    {
        return rhs;
    }

    [[nodiscard]] inline RWByteSpan
    ToReadWrite(Immutable<ByteSpan> rhs) noexcept
    {
        return { Syntropy::ToReadWrite(rhs.GetData()), rhs.GetCount() };
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
        return MakeByteSpan(ToBytePtr(PtrOf(rhs[0])), VSize * SizeOf<TType>());
    }

}

// ===========================================================================
