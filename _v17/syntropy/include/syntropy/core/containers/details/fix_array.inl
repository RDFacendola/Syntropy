
/// \file fix_array.inl
///
/// \author Raffaele D. Facendola - January 2021.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FIX ARRAY                                                            */
    /************************************************************************/

    template <typename TType, Int TCount>
    template <typename... TTypes>
    requires (sizeof...(TTypes) == TCount)
    constexpr FixArray<TType, TCount>
    ::FixArray(Forwarding<TTypes>... elements) noexcept
        : elements_{ Forward<TTypes>(elements)... }
    {

    }

    template <typename TType, Int TCount>
    template <typename UType>
    constexpr FixArray<TType, TCount>
    ::FixArray(Immutable<FixArray<UType, TCount>> rhs) noexcept
        : FixArray(ElementwiseTag{},
                   Templates::MakeSequence<TCount>{},
                   rhs)
    {

    }

    template <typename TType, Int TCount>
    template <typename UType>
    constexpr FixArray<TType, TCount>
    ::FixArray(Movable<FixArray<UType, TCount>> rhs) noexcept
        : FixArray(ElementwiseTag{},
                   Templates::MakeSequence<TCount>{},
                   Move(rhs))
    {

    }

    template <typename TType, Int TCount>
    template <typename UType>
    constexpr Mutable<FixArray<TType, TCount>>
    FixArray<TType, TCount>
    ::operator=(Immutable<FixArray<UType, TCount>> rhs) noexcept
    {
        Ranges::PartialCopy(*this, rhs);

        return *this;
    }

    template <typename TType, Int TCount>
    template <typename UType>
    constexpr Mutable<FixArray<TType, TCount>>
    FixArray<TType, TCount>
    ::operator=(Movable<FixArray<UType, TCount>> rhs) noexcept
    {
        Ranges::PartialMove(*this, Move(rhs));

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
    template<typename TFixArray, Int... TIndexes>
    constexpr FixArray<TType, TCount>
    ::FixArray(ElementwiseTag,
               Templates::Sequence<TIndexes...>,
               Forwarding<TFixArray> other) noexcept
        : FixArray(Get<TIndexes>(Forward<TFixArray>(other))...)
    {

    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Element access.
    // ===============

    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Immutable<TType>
    Get(Immutable<FixArray<TType, TCount>> fix_array) noexcept
    {
        return ToImmutable(fix_array[TIndex]);
    }

    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Mutable<TType>
    Get(Mutable<FixArray<TType, TCount>> fix_array) noexcept
    {
        return ToMutable(fix_array[TIndex]);
    }

    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Immovable<TType>
    Get(Immovable<FixArray<TType, TCount>> fix_array) noexcept
    {
        return ToImmovable(fix_array[TIndex]);
    }

    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Movable<TType>
    Get(Movable<FixArray<TType, TCount>> fix_array) noexcept
    {
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

    // Swap.
    // =====

    template <typename TType, typename UType, Int TCount>
    constexpr void
    Swap(Mutable<FixArray<TType, TCount>> lhs,
         Mutable<FixArray<UType, TCount>> rhs) noexcept
    {
        Ranges::PartialSwap(lhs, rhs);
    }

}

// ===========================================================================
