
/// \file span.inl
///
/// \author Raffaele D. Facendola - Jun 2020

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BASE SPAN                                                            */
    /************************************************************************/

    template <typename TType, typename TTraits>
    constexpr BaseSpan<TType, TTraits>
    ::BaseSpan(Null) noexcept
    {

    }

    template <typename TType, typename TTraits>
    constexpr BaseSpan<TType, TTraits>
    ::BaseSpan(Immutable<BaseSpan<TType, TTraits>::PointerType> begin,
               Immutable<BaseSpan<TType, TTraits>::CardinalityType> count)
                   noexcept
        : data_(begin)
        , count_(count)
    {

    }

    template <typename TType, typename TTraits>
    constexpr BaseSpan<TType, TTraits>
    ::BaseSpan(Immutable<BaseSpan<TType, TTraits>::PointerType> begin,
               Immutable<BaseSpan<TType, TTraits>::PointerType> end) noexcept
        : BaseSpan(begin, CardinalityType{ end - begin })
    {

    }

    template <typename TType, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr BaseSpan<TType, TTraits>
    ::BaseSpan(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept
        : data_(ToPtr<TType>(rhs.data_))
        , count_(rhs.count_)
    {

    }

    template <typename TType, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr Mutable<BaseSpan<TType, TTraits>> BaseSpan<TType, TTraits>
    ::operator=(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept
    {
        data_ = ToPtr<TType>(rhs.data_);
        count_ = rhs.count_;

        return *this;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr BaseSpan<TType, TTraits>
    ::operator Bool() const noexcept
    {
        return count_ > CardinalityType{ 0 };
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr
    typename BaseSpan<TType, TTraits>::ReferenceType
    BaseSpan<TType, TTraits>
    ::operator[](
        Immutable<typename BaseSpan<TType, TTraits>::CardinalityType> index)
    const noexcept
    {
        return data_[ToInt(index)];
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr
    typename BaseSpan<TType, TTraits>::PointerType BaseSpan<TType, TTraits>
    ::GetData() const noexcept
    {
        return data_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr
    Immutable<typename BaseSpan<TType, TTraits>::CardinalityType>
    BaseSpan<TType, TTraits>
    ::GetCount() const noexcept
    {
        return count_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr
    BaseSpan<TType, TTraits>
    BaseSpan<TType, TTraits>
    ::Select(Immutable<CardinalityType> offset,
             Immutable<CardinalityType> count) const noexcept
    {
        return { data_ + offset, count };
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // BaseSpan.
    // =========

    template <IsSpan TSpan, IsSpan USpan>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<TSpan> lhs, Immutable<USpan> rhs) noexcept
    {
        return Ranges::AreEquivalent(lhs, rhs);
    }

    template <IsSpan TSpan, IsSpan USpan>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<TSpan> lhs, Immutable<USpan> rhs) noexcept
    {
        return Ranges::Compare(lhs, rhs);
    }

    // Access.
    // =======

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr Span<TType>
    ToReadOnly(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept
    {
        return { ToReadOnly(rhs.GetData()), rhs.GetCount() };
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWSpan<TType>
    ToReadWrite(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept
    {
        return { ToReadWrite(rhs.GetData()), rhs.GetCount() };
    }

    // Utilities.
    // ==========

    template <typename TType, typename TCount>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Immutable<TCount> count) noexcept
    {
        return { begin, count };
    }

    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Ptr<TType> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType, typename TCount>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, Immutable<TCount> count) noexcept
    {
        return { begin, count };
    }

    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, RWPtr<TType> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType, Int VCount>
    [[nodiscard]] constexpr auto
    MakeSpan(TType(&rhs)[VCount]) noexcept
    {
        return MakeSpan(PtrOf(rhs[0]), VCount);
    }

}

// ===========================================================================
