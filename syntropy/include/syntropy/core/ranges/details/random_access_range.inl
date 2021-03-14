
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

    // Random access range_view view.
    // =========================

    template <Concepts::RandomAccessRangeView TRangeView, typename TIndex>
    [[nodiscard]] constexpr decltype(auto)
    At(Immutable<TRangeView> range_view, Immutable<TIndex> index) noexcept
    {
        return Details::RouteAt(range_view, index);
    }

    template <Concepts::RandomAccessRangeView TRangeView,
              typename TIndex,
              typename TCount>
    [[nodiscard]] constexpr TRangeView
    Slice(Immutable<TRangeView> range_view,
          Immutable<TIndex> index,
          Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(range_view, index, count);
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr TRangeView
    Front(Immutable<TRangeView> range_view, Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(
            range_view,
            Templates::RangeViewCountType<TRangeView>{ 0 }, count);
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr TRangeView
    Back(Immutable<TRangeView> range_view, Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(
            range_view,
            Details::RouteCount(range_view) - count, count);
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr TRangeView
    PopFront(Immutable<TRangeView> range_view,
             Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(
            range_view,
            count,
            Details::RouteCount(range_view) - count);
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr TRangeView
    PopBack(Immutable<TRangeView> range_view, Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(
            range_view,
            Templates::RangeViewCountType<TRangeView>{ 0 },
            Details::RouteCount(range_view) - count);
    }

    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRangeView> range_view) noexcept
    {
        return Tuples::MakeTuple(
            Details::RouteFront(range_view),
            Details::RoutePopFront(range_view));
    }

    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRangeView> range_view) noexcept
    {
        return Tuples::MakeTuple(
            Details::RouteBack(range_view),
            Details::RoutePopBack(range_view));
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRangeView> range_view,
               Immutable<TCount> count) noexcept
    {
        return Tuples::MakeTuple(
            Details::RouteFront(range_view, count),
            Details::RoutePopFront(range_view, count));
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRangeView> range_view,
              Immutable<TCount> count) noexcept
    {
        return Tuples::MakeTuple(
            Details::RouteBack(range_view, count),
            Details::RoutePopBack(range_view, count));
    }

}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE EXTENSIONS                                       */
    /************************************************************************/

    template <Concepts::BaseRandomAccessRangeView TRangeView>
    [[nodiscard]] inline decltype(auto) Front<TRangeView>
    ::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        using TCount = Templates::RangeViewCountType<TRangeView>;

        return Details::RouteAt(range_view, TCount{ 0 });
    }

    template <Concepts::BaseRandomAccessRangeView TRangeView>
    [[nodiscard]] inline
    TRangeView PopFront<TRangeView>
    ::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        using TCount = Templates::RangeViewCountType<TRangeView>;

        auto index = TCount{ 1 };
        auto count = Details::RouteCount(range_view) - index;

        return Details::RouteSlice(range_view, index, count);
    };

    template <Concepts::BaseRandomAccessRangeView TRangeView>
    [[nodiscard]]
    inline decltype(auto) Back<TRangeView>
    ::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        using TCount = Templates::RangeViewCountType<TRangeView>;

        auto count = Details::RouteCount(range_view) - TCount{ 1 };

        return Details::RouteAt(range_view, count);
    };

    template <Concepts::BaseRandomAccessRangeView TRangeView>
    [[nodiscard]]
    inline TRangeView PopBack<TRangeView>
    ::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        using TCount = Templates::RangeViewCountType<TRangeView>;

        auto index = TCount{ 0 };
        auto count = Details::RouteCount(range_view) - TCount{ 1 };

        return Details::RouteSlice(range_view, index, count);
    }
}

// ===========================================================================
