
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

    // Random access range.
    // ====================

    template <Concepts::RandomAccessRange TRange, typename TIndex>
    [[nodiscard]] constexpr decltype(auto)
    At(Immutable<TRange> range, Immutable<TIndex> index) noexcept
    {
        return Details::RouteAt(range, index);
    }

    template <Concepts::RandomAccessRange TRange,
              typename TIndex,
              typename TCount>
    [[nodiscard]] constexpr TRange
    Slice(Immutable<TRange> range,
          Immutable<TIndex> index,
          Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(range, index, count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr TRange
    Front(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(
            range,
            Templates::RangeCountType<TRange>{ 0 }, count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr TRange
    Back(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(
            range,
            Details::RouteCount(range) - count, count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr TRange
    PopFront(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(
            range,
            count,
            Details::RouteCount(range) - count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr TRange
    PopBack(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(
            range,
            Templates::RangeCountType<TRange>{ 0 },
            Details::RouteCount(range) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range) noexcept
    {
        return Tuples::MakeTuple(
            Details::RouteFront(range),
            Details::RoutePopFront(range));
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range) noexcept
    {
        return Tuples::MakeTuple(
            Details::RouteBack(range),
            Details::RoutePopBack(range));
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Tuples::MakeTuple(
            Details::RouteFront(range, count),
            Details::RoutePopFront(range, count));
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Tuples::MakeTuple(
            Details::RouteBack(range, count),
            Details::RoutePopBack(range, count));
    }

}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE EXTENSIONS                                       */
    /************************************************************************/

    template <Concepts::BaseRandomAccessRange TRange>
    [[nodiscard]] inline decltype(auto) Front<TRange>
    ::operator()(Immutable<TRange> range) const noexcept
    {
        using TCount = Templates::RangeCountType<TRange>;

        return Details::RouteAt(range, TCount{ 0 });
    }

    template <Concepts::BaseRandomAccessRange TRange>
    [[nodiscard]] inline
    TRange PopFront<TRange>
    ::operator()(Immutable<TRange> range) const noexcept
    {
        using TCount = Templates::RangeCountType<TRange>;

        return Details::RouteSlice(range,
                                   TCount{ 1 },
                                   Details::RouteCount(range) - TCount{ 1 });
    };

    template <Concepts::BaseRandomAccessRange TRange>
    [[nodiscard]]
    inline decltype(auto) Back<TRange>
    ::operator()(Immutable<TRange> range) const noexcept
    {
        using TCount = Templates::RangeCountType<TRange>;

        return Details::RouteAt(range,
                                Details::RouteCount(range) - TCount{ 1 });
    };

    template <Concepts::BaseRandomAccessRange TRange>
    [[nodiscard]]
    inline TRange PopBack<TRange>
    ::operator()(Immutable<TRange> range) const noexcept
    {
        using TCount = Templates::RangeCountType<TRange>;

        return Details::RouteSlice(range,
                                   TCount{ 0 },
                                   Details::RouteCount(range) - TCount{ 1 });
    }
}

// ===========================================================================
