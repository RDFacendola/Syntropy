
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
    ::BaseSpan(Immutable<BaseSpan<TType, TTraits>::TPointer> begin,
               Int size) noexcept
        : data_(begin)
        , count_(size)
    {

    }

    template <typename TType, typename TTraits>
    constexpr BaseSpan<TType, TTraits>
    ::BaseSpan(Immutable<BaseSpan<TType, TTraits>::TPointer> begin,
               Immutable<BaseSpan<TType, TTraits>::TPointer> end) noexcept
        : BaseSpan(begin, ToInt(end - begin))
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
        return count_ > ToInt(0);
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr
    typename BaseSpan<TType, TTraits>::TReference BaseSpan<TType, TTraits>
    ::operator[](Int index) const noexcept
    {
        return data_[index];
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr
    typename BaseSpan<TType, TTraits>::TPointer BaseSpan<TType, TTraits>
    ::GetData() const noexcept
    {
        return data_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr Int BaseSpan<TType, TTraits>
    ::GetCount() const noexcept
    {
        return count_;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // BaseSpan.
    // =========

    template <typename TType, typename TTraits,
              typename UType, typename UTraits>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<BaseSpan<TType, TTraits>> lhs,
               Immutable<BaseSpan<UType, UTraits>> rhs) noexcept
    {
        return Ranges::AreEquivalent(lhs, rhs);
    }

    template <typename TType, typename TTraits,
              typename UType, typename UTraits>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<BaseSpan<TType, TTraits>> lhs,
                Immutable<BaseSpan<UType, UTraits>> rhs) noexcept
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
    
    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Int size) noexcept
    {
        return { begin, size };
    }

    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Ptr<TType> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, Int size) noexcept
    {
        return { begin, size };
    }

    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, RWPtr<TType> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType, Int VSize>
    [[nodiscard]] constexpr auto
    MakeSpan(TType(&rhs)[VSize]) noexcept
    {
        return MakeSpan(PtrOf(rhs[0]), VSize);
    }

}

// ===========================================================================
