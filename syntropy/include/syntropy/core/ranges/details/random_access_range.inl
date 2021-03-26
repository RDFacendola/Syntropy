
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

    template <Concepts::RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    Front(Immutable<TRange> range, Immutable<TCardinality> count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                             Templates::RangeCardinalityTypeOf<TRange>{ 0 },
                             count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    Back(Immutable<TRange> range, Immutable<TCardinality> count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              Ranges::Count(range) - count,
                              count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    PopFront(Immutable<TRange> range, Immutable<TCardinality> count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              count,
                              Ranges::Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    PopBack(Immutable<TRange> range, Immutable<TCardinality> count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              Templates::RangeCardinalityTypeOf<TRange>{ 0 },
                              Ranges::Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range) noexcept
    {
        return Records::MakeTuple(Ranges::Front(range),
                                 Ranges::PopFront(range));
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range) noexcept
    {
        return Records::MakeTuple(Ranges::Back(range),
                                 Ranges::PopBack(range));
    }

    template <Concepts::RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range,
               Immutable<TCardinality> count) noexcept
    {
        return Records::MakeTuple(Ranges::Front(range, count),
                                 Ranges::PopFront(range, count));
    }

    template <Concepts::RandomAccessRange TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range,
              Immutable<TCardinality> count) noexcept
    {
        return Records::MakeTuple(Ranges::Back(range, count),
                                 Ranges::PopBack(range, count));
    }

}

// ===========================================================================
