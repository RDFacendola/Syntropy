
/// \file forward_range.inl
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Forward range view.
    // ===================

    template <Concepts::ForwardRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto)
    Front(Immutable<TRangeView> range_view) noexcept
    {
        return Details::RouteFront(range_view);
    }

    template <Concepts::ForwardRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    PopFront(Immutable<TRangeView> range_view) noexcept
    {
        return Details::RoutePopFront(range_view);
    }

    template <Concepts::ForwardRangeView TRangeView>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<TRangeView> range_view) noexcept
    {
        return Details::RouteIsEmpty(range_view);
    }

    template <Concepts::ForwardRangeView TRangeView, typename TFunction>
    constexpr void
    ForEach(Immutable<TRangeView> range_view, TFunction function) noexcept
    {
        for (auto rest = range_view;
             !Details::RouteIsEmpty(rest);
             rest = Details::RoutePopFront(rest))
        {
            function(Details::RouteFront(rest));
        }
    }

    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Tuples::Tuple<TRangeView, URangeView>
    Copy(Immutable<TRangeView> destination,
         Immutable<URangeView> source) noexcept
    {
        auto source_copy = source;
        auto destination_copy = destination;

        for (; !Details::RouteIsEmpty(source_copy) &&
               !Details::RouteIsEmpty(destination_copy);)
        {
            Details::RouteFront(destination_copy)
                = Details::RouteFront(source_copy);

            source_copy = Details::RoutePopFront(source_copy);
            destination_copy = Details::RoutePopFront(destination_copy);
        }

        return { destination_copy, source_copy };
    }

    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Tuples::Tuple<TRangeView, URangeView>
    Move(Immutable<TRangeView> destination,
         Immutable<URangeView> source) noexcept
    {
        auto source_copy = source;
        auto destination_copy = destination;

        for (; !Details::RouteIsEmpty(source_copy) &&
               !Details::RouteIsEmpty(destination_copy);)
        {
            Details::RouteFront(destination_copy)
                = Syntropy::Move(Details::RouteFront(source_copy));

            source_copy = Details::RoutePopFront(source_copy);
            destination_copy = Details::RoutePopFront(destination_copy);
        }

        return { source , destination };
    }

    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Tuples::Tuple<TRangeView, URangeView>
    Swap(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept
    {
        auto left = lhs;
        auto right = rhs;

        for (; !Details::RouteIsEmpty(left) &&
               !Details::RouteIsEmpty(right);)
        {
            Algorithm::Swap(Details::RouteFront(left),
                            Details::RouteFront(right));

            left = Details::RoutePopFront(left);
            right = Details::RoutePopFront(right);
        }

        return { left , right };
    }

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range view.
    template <Ranges::Concepts::ForwardRangeView TRangeView>
    constexpr auto begin(Immutable<TRangeView> range_view) noexcept
    {
        return Ranges::Details::RangeViewIterator<TRangeView>{ range_view };
    }

    /// \brief Get an iterator past the last element in a range view.
    template <Ranges::Concepts::ForwardRangeView TRangeView>
    constexpr auto end(Immutable<TRangeView> range_view) noexcept
    {
        return Ranges::Details::RangeViewIterator<TRangeView>{};
    }
}

// ===========================================================================
