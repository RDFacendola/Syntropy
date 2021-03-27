
/// \file random_access_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // RandomAccessRange.
    // ==================

    template <RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    Front(Immutable<TRange> range, Immutable<TCardinality> count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              RangeCardinalityTypeOf<TRange>{ 0 },
                              count);
    }

    template <RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    Back(Immutable<TRange> range, Immutable<TCardinality> count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              Ranges::Count(range) - count,
                              count);
    }

    template <RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    PopFront(Immutable<TRange> range, Immutable<TCardinality> count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              count,
                              Ranges::Count(range) - count);
    }

    template <RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    PopBack(Immutable<TRange> range, Immutable<TCardinality> count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              RangeCardinalityTypeOf<TRange>{ 0 },
                              Ranges::Count(range) - count);
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range) noexcept
    {
        return MakeTuple(Ranges::Front(range), Ranges::PopFront(range));
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range) noexcept
    {
        return MakeTuple(Ranges::Back(range), Ranges::PopBack(range));
    }

    template <RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range,
               Immutable<TCardinality> count) noexcept
    {
        return MakeTuple(Ranges::Front(range, count),
                         Ranges::PopFront(range, count));
    }

    template <RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range,
              Immutable<TCardinality> count) noexcept
    {
        return MakeTuple(Ranges::Back(range, count),
                         Ranges::PopBack(range, count));
    }

}

// ===========================================================================
