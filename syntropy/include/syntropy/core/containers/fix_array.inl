
/// \file fix_array.inl
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BASE FIX ARRAY                                                       */
    /************************************************************************/

    template <typename TType, Int VSize, typename TTraits>
    template <typename... TTypes, typename>
    constexpr BaseFixArray<TType, VSize, TTraits>:
    :BaseFixArray(Forwarding<TTypes>... elements) noexcept
        : elements_{ Forward<TTypes>(elements)... }
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr  BaseFixArray<TType, VSize, TTraits>
    ::BaseFixArray(Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
        : BaseFixArray(UnwindTag{},
                       rhs,
                       Syntropy::Templates::MakeSequence<VSize>{})
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr  BaseFixArray<TType, VSize, TTraits>
    ::BaseFixArray(Movable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
        : BaseFixArray(UnwindTag{},
                       Move(rhs),
                       Syntropy::Templates::MakeSequence<VSize>{})
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr Mutable<BaseFixArray<TType, VSize, TTraits>>
    BaseFixArray<TType, VSize, TTraits>
    ::operator=(Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
    {
        Ranges::Copy(RangeOf(rhs), RangeOf(*this));

        return *this;
    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr Mutable<BaseFixArray<TType, VSize, TTraits>>
    BaseFixArray<TType, VSize, TTraits>
    ::operator=(Movable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
    {
        Ranges::Move(RangeOf(rhs), RangeOf(*this));

        return *this;
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr
    typename BaseFixArray<TType, VSize, TTraits>::TReference
    BaseFixArray<TType, VSize, TTraits>::operator[](Int index) noexcept
    {
        return elements_[index];
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immutable<TType>BaseFixArray<TType, VSize, TTraits>
    ::operator[](Int index) const noexcept
    {
        return elements_[index];
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Ptr<TType> BaseFixArray<TType, VSize, TTraits>
    ::GetData() const noexcept
    {
        return (VSize > 0) ? PtrOf(elements_[0]) : nullptr;
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr RWPtr<TType> BaseFixArray<TType, VSize, TTraits>
    ::GetData() noexcept
    {
        return (VSize > 0) ? PtrOf(elements_[0]) : nullptr;
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Int BaseFixArray<TType, VSize, TTraits>
    ::GetSize() const noexcept
    {
        return VSize;
    }

    template <typename TType, Int VSize, typename TTraits>
    template<typename TFixArray, Int... VIndexes>
    constexpr BaseFixArray<TType, VSize, TTraits>
    ::BaseFixArray(UnwindTag,
                   Forwarding<TFixArray> other,
                   Syntropy::Templates::Sequence<VIndexes...>) noexcept
        : BaseFixArray(Get<VIndexes>(Forward<TFixArray>(other))...)
    {

    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // N-Tuple.
    // ========

    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immutable<TType>
    Get(Immutable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept
    {
        static_assert((VIndex >= 0) && (VIndex < VSize),
                      "Index out-of-range.");

        return ToImmutable(tuple[VIndex]);
    }

    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Mutable<TType>
    Get(Mutable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept
    {
        static_assert((VIndex >= 0) && (VIndex < VSize),
                      "Index out-of-range.");

        return ToMutable(tuple[VIndex]);
    }

    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immovable<TType>
    Get(Immovable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept
    {
        static_assert((VIndex >= 0) && (VIndex < VSize),
                      "Index out-of-range.");

        return ToImmovable(tuple[VIndex]);
    }

    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Movable<TType>
    Get(Movable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept
    {
        static_assert((VIndex >= 0) && (VIndex < VSize),
                      "Index out-of-range.");

        return Move(tuple[VIndex]);
    }

    // Comparison.
    // ===========

    template <typename TType, typename TTraits,
              typename UType, typename UTraits, Int VSize>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs,
               Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
    {
        return Ranges::AreEquivalent(RangeOf(lhs), RangeOf(rhs));
    }

    template <typename TType, typename TTraits,
              typename UType, typename UTraits, Int VSize>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs,
                Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
    {
        return Ranges::Compare(RangeOf(lhs), RangeOf(rhs));
    }

    // Utilities.
    // ==========

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr RWSpan<TType>
    RangeOf(Mutable<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept
    {
        return MakeSpan(rhs.GetData(), rhs.GetSize());
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Span<TType>
    RangeOf(Immutable<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept
    {
        return MakeSpan(rhs.GetData(), rhs.GetSize());
    }
}

// ===========================================================================
