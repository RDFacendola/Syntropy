
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
        using Details::RouteAt;

        return RouteAt(range_view, index);
    }

    template <Concepts::RandomAccessRangeView TRangeView,
              typename TIndex,
              typename TCount>
    [[nodiscard]] constexpr TRangeView
    Slice(Immutable<TRangeView> range_view,
          Immutable<TIndex> index,
          Immutable<TCount> count) noexcept
    {
        using Details::RouteSlice;

        return RouteSlice(range_view, index, count);
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr TRangeView
    Front(Immutable<TRangeView> range_view, Immutable<TCount> count) noexcept
    {
        using Details::RouteSlice;

        return RouteSlice(
            range_view,
            Templates::RangeViewCountType<TRangeView>{ 0 }, count);
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr TRangeView
    Back(Immutable<TRangeView> range_view, Immutable<TCount> count) noexcept
    {
        using Details::RouteSlice;
        using Details::RouteCount;

        return RouteSlice(
            range_view,
            RouteCount(range_view) - count, count);
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr TRangeView
    PopFront(Immutable<TRangeView> range_view,
             Immutable<TCount> count) noexcept
    {
        using Details::RouteSlice;
        using Details::RouteCount;

        return RouteSlice(
            range_view,
            count,
            RouteCount(range_view) - count);
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr TRangeView
    PopBack(Immutable<TRangeView> range_view, Immutable<TCount> count) noexcept
    {
        using Details::RouteSlice;
        using Details::RouteCount;

        return RouteSlice(
            range_view,
            Templates::RangeViewCountType<TRangeView>{ 0 },
            RouteCount(range_view) - count);
    }

    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRangeView> range_view) noexcept
    {
        using Details::RouteFront;
        using Details::RoutePopFront;

        return Tuples::MakeTuple(
            RouteFront(range_view),
            RoutePopFront(range_view));
    }

    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRangeView> range_view) noexcept
    {
        using Details::RouteBack;
        using Details::RoutePopBack;

        return Tuples::MakeTuple(
            RouteBack(range_view),
            RoutePopBack(range_view));
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRangeView> range_view,
               Immutable<TCount> count) noexcept
    {
        using Details::RouteFront;
        using Details::RoutePopFront;

        return Tuples::MakeTuple(
            RouteFront(range_view, count),
            RoutePopFront(range_view, count));
    }

    template <Concepts::RandomAccessRangeView TRangeView, typename TCount>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRangeView> range_view,
              Immutable<TCount> count) noexcept
    {
        using Details::RouteBack;
        using Details::RoutePopBack;

        return Tuples::MakeTuple(
            RouteBack(range_view, count),
            RoutePopBack(range_view, count));
    }

    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    ViewOf(Immutable<TRangeView> range_view) noexcept
    {
        return range_view;
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
        using Details::RouteAt;

        using TCount = Templates::RangeViewCountType<TRangeView>;

        return RouteAt(range_view, TCount{ 0 });
    }

    template <Concepts::BaseRandomAccessRangeView TRangeView>
    [[nodiscard]] inline
    TRangeView PopFront<TRangeView>
    ::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        using Details::RouteCount;
        using Details::RouteSlice;

        using TCount = Templates::RangeViewCountType<TRangeView>;

        auto index = TCount{ 1 };
        auto count = RouteCount(range_view) - index;

        return RouteSlice(range_view, index, count);
    };

    template <Concepts::BaseRandomAccessRangeView TRangeView>
    [[nodiscard]]
    inline decltype(auto) Back<TRangeView>
    ::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        using Details::RouteCount;
        using Details::RouteAt;

        using TCount = Templates::RangeViewCountType<TRangeView>;

        auto count = RouteCount(range_view) - TCount{ 1 };

        return RouteAt(range_view, count);
    };

    template <Concepts::BaseRandomAccessRangeView TRangeView>
    [[nodiscard]]
    inline TRangeView PopBack<TRangeView>
    ::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        using Details::RouteCount;
        using Details::RouteSlice;

        using TCount = Templates::RangeViewCountType<TRangeView>;

        auto index = TCount{ 0 };
        auto count = RouteCount(range_view) - TCount{ 1 };

        return RouteSlice(range_view, index, count);
    }
}

// ===========================================================================
