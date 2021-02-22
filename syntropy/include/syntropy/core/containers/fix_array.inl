
/// \file fix_array.inl
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FIX ARRAY                                                            */
    /************************************************************************/

    template <typename TType, Int TCount>
    template <typename... TTypes, typename>
    constexpr FixArray<TType, TCount>
    ::FixArray(Forwarding<TTypes>... elements) noexcept
        : elements_{ Forward<TTypes>(elements)... }
    {

    }

    template <typename TType, Int TCount>
    template <typename UType>
    constexpr FixArray<TType, TCount>
    ::FixArray(Immutable<FixArray<UType, TCount>> rhs) noexcept
        : FixArray(UnwindTag{},
                   rhs,
                   Syntropy::Templates::MakeSequence<TCount>{})
    {

    }

    template <typename TType, Int TCount>
    template <typename UType>
    constexpr FixArray<TType, TCount>
    ::FixArray(Movable<FixArray<UType, TCount>> rhs) noexcept
        : FixArray(UnwindTag{},
                   Move(rhs),
                   Syntropy::Templates::MakeSequence<TCount>{})
    {

    }

    template <typename TType, Int TCount>
    template <typename UType>
    constexpr Mutable<FixArray<TType, TCount>>
    FixArray<TType, TCount>
    ::operator=(Immutable<FixArray<UType, TCount>> rhs) noexcept
    {
        Ranges::Copy(rhs, *this);

        return *this;
    }

    template <typename TType, Int TCount>
    template <typename UType>
    constexpr Mutable<FixArray<TType, TCount>>
    FixArray<TType, TCount>
    ::operator=(Movable<FixArray<UType, TCount>> rhs) noexcept
    {
        Ranges::Move(rhs, *this);

        return *this;
    }

    template <typename TType, Int TCount>
    constexpr FixArray<TType, TCount>
    ::operator Span<TType>() const noexcept
    {
        return MakeSpan(elements_);
    }

    template <typename TType, Int TCount>
    constexpr FixArray<TType, TCount>
    ::operator RWSpan<TType>() noexcept
    {
        return MakeSpan(elements_);
    }

    template <typename TType, Int TCount>
    [[nodiscard]] constexpr Mutable<TType>
    FixArray<TType, TCount>
    ::operator[](Int index) noexcept
    {
        return elements_[index];
    }

    template <typename TType, Int TCount>
    [[nodiscard]] constexpr Immutable<TType>
    FixArray<TType, TCount>
    ::operator[](Int index) const noexcept
    {
        return elements_[index];
    }

    template <typename TType, Int TCount>
    [[nodiscard]] constexpr Ptr<TType> FixArray<TType, TCount>
    ::GetData() const noexcept
    {
        return (TCount > 0) ? PtrOf(elements_[0]) : nullptr;
    }

    template <typename TType, Int TCount>
    [[nodiscard]] constexpr RWPtr<TType> FixArray<TType, TCount>
    ::GetData() noexcept
    {
        return (TCount > 0) ? PtrOf(elements_[0]) : nullptr;
    }

    template <typename TType, Int TCount>
    [[nodiscard]] constexpr Int FixArray<TType, TCount>
    ::GetCount() const noexcept
    {
        return TCount;
    }

    template <typename TType, Int TCount>
    template<typename TFixArray, Int... TIndexes>
    constexpr FixArray<TType, TCount>
    ::FixArray(UnwindTag,
               Forwarding<TFixArray> other,
               Syntropy::Templates::Sequence<TIndexes...>) noexcept
        : FixArray(Get<TIndexes>(Forward<TFixArray>(other))...)
    {

    }

    template <typename TType, Int TCount>
    constexpr void FixArray<TType, TCount>
    ::Swap(Movable<FixArray<TType, TCount>> rhs) noexcept
    {
        Ranges::Swap(rhs, *this);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // N-Tuple.
    // ========

    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Immutable<TType>
    Get(Immutable<FixArray<TType, TCount>> fix_array) noexcept
    {
        static_assert((TIndex >= 0) && (TIndex < TCount),
                      "Index out-of-range.");

        return ToImmutable(fix_array[TIndex]);
    }

    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Mutable<TType>
    Get(Mutable<FixArray<TType, TCount>> fix_array) noexcept
    {
        static_assert((TIndex >= 0) && (TIndex < TCount),
                      "Index out-of-range.");

        return ToMutable(fix_array[TIndex]);
    }

    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Immovable<TType>
    Get(Immovable<FixArray<TType, TCount>> fix_array) noexcept
    {
        static_assert((TIndex >= 0) && (TIndex < TCount),
                      "Index out-of-range.");

        return ToImmovable(fix_array[TIndex]);
    }

    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Movable<TType>
    Get(Movable<FixArray<TType, TCount>> fix_array) noexcept
    {
        static_assert((TIndex >= 0) && (TIndex < TCount),
                      "Index out-of-range.");

        return Move(fix_array[TIndex]);
    }

    // Comparison.
    // ===========

    template <typename TType, typename UType, Int TCount>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<FixArray<TType, TCount>> lhs,
               Immutable<FixArray<UType, TCount>> rhs) noexcept
    {
        return Ranges::AreEquivalent(lhs, rhs);
    }

    template <typename TType, typename UType, Int TCount>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<FixArray<TType, TCount>> lhs,
                Immutable<FixArray<UType, TCount>> rhs) noexcept
    {
        return Ranges::Compare(lhs, rhs);
    }

    // Ranges.
    // =======

    template <typename TType, Int TCount>
    [[nodiscard]] constexpr Span<TType>
    ViewOf(Immutable<FixArray<TType, TCount>> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType, Int TCount>
    [[nodiscard]] constexpr RWSpan<TType>
    ViewOf(Mutable<FixArray<TType, TCount>> rhs) noexcept
    {
        return rhs;
    }

}

// ===========================================================================
