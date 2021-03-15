
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
        using Details::RouteFront;

        return RouteFront(range_view);
    }

    template <Concepts::ForwardRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    PopFront(Immutable<TRangeView> range_view) noexcept
    {
        using Details::RoutePopFront;

        return RoutePopFront(range_view);
    }

    template <Concepts::ForwardRangeView TRangeView>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<TRangeView> range_view) noexcept
    {
        using Details::RouteIsEmpty;

        return RouteIsEmpty(range_view);
    }

    template <Concepts::ForwardRangeView TRangeView,
              typename TFunction>
    constexpr void
    ForEach(Immutable<TRangeView> range_view,
            TFunction function) noexcept
    {
        using Details::RouteFront;
        using Details::RoutePopFront;
        using Details::RouteIsEmpty;

        for (auto rest = range_view;
             !RouteIsEmpty(rest);
             rest = RoutePopFront(rest))
        {
            function(RouteFront(rest));
        }
    }

    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Int
    Copy(Immutable<TRangeView> destination,
         Immutable<URangeView> source) noexcept
    {
        using Details::RouteFront;
        using Details::RoutePopFront;
        using Details::RouteIsEmpty;

        auto source_copy = source;
        auto destination_copy = destination;
        auto count = ToInt(0);

        for (; !RouteIsEmpty(source_copy) &&
               !RouteIsEmpty(destination_copy);
               ++count)
        {
            RouteFront(destination_copy) = RouteFront(source_copy);

            source_copy = RoutePopFront(source_copy);
            destination_copy = RoutePopFront(destination_copy);
        }

        return count;
    }

    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Int
    Move(Immutable<TRangeView> destination,
         Immutable<URangeView> source) noexcept
    {
        using Details::RouteFront;
        using Details::RoutePopFront;
        using Details::RouteIsEmpty;

        auto source_copy = source;
        auto destination_copy = destination;
        auto count = ToInt(0);

        for (; !RouteIsEmpty(source_copy) &&
               !RouteIsEmpty(destination_copy);
               ++count)
        {
            RouteFront(destination_copy)
                = Syntropy::Move(RouteFront(source_copy));

            source_copy = RoutePopFront(source_copy);
            destination_copy = RoutePopFront(destination_copy);
        }

        return count;
    }

    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Int
    Swap(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept
    {
        using Details::RouteFront;
        using Details::RoutePopFront;
        using Details::RouteIsEmpty;

        auto left = lhs;
        auto right = rhs;
        auto count = ToInt(0);

        for (; !RouteIsEmpty(left) && !RouteIsEmpty(right); ++count)
        {
            Algorithm::Swap(RouteFront(left), RouteFront(right));

            left = RoutePopFront(left);
            right = RoutePopFront(right);
        }

        return count;
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
