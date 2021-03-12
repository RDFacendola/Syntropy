
/// \file forward_range.inl
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

    // Forward range.
    // ==============

    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr decltype(auto)
    Front(Immutable<TRange> range) noexcept
    {
        return Details::RouteFront(range);
    }

    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr TRange
    PopFront(Immutable<TRange> range) noexcept
    {
        return Details::RoutePopFront(range);
    }

    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<TRange> range) noexcept
    {
        return Details::RouteIsEmpty(range);
    }

    template <Concepts::ForwardRange TRange, typename TFunction>
    constexpr void
    ForEach(Immutable<TRange> range, TFunction function) noexcept
    {
        for (auto rest = range;
             !Details::RouteIsEmpty(rest);
             rest = Details::RoutePopFront(rest))
        {
            function(Details::RouteFront(rest));
        }
    }

    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange>
    Copy(Immutable<TRange> destination, Immutable<URange> source) noexcept
    {
        auto source_copy = source;
        auto destination_copy = destination;

        for (; !Details::RouteIsEmpty(source_copy)
               && !Details::RouteIsEmpty(destination_copy);)
        {
            Details::RouteFront(destination_copy)
                = Details::RouteFront(source_copy);

            source_copy = Details::RoutePopFront(source_copy);
            destination_copy = Details::RoutePopFront(destination_copy);
        }

        return { destination_copy, source_copy };
    }

    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange>
    Move(Immutable<TRange> destination, Immutable<URange> source) noexcept
    {
        auto source_copy = source;
        auto destination_copy = destination;

        for (; !Details::RouteIsEmpty(source_copy)
               && !Details::RouteIsEmpty(destination_copy);)
        {
            Details::RouteFront(destination_copy)
                = Syntropy::Move(Details::RouteFront(source_copy));

            source_copy = Details::RoutePopFront(source_copy);
            destination_copy = Details::RoutePopFront(destination_copy);
        }

        return { source , destination };
    }

    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange>
    Swap(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto left = lhs;
        auto right = rhs;

        for (; !Details::RouteIsEmpty(left)
               && !Details::RouteIsEmpty(right);)
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

    /// \brief Get an iterator to the first element in a range.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto begin(Immutable<TRange> range) noexcept
    {
        return Ranges::Details::RangeIterator<TRange>{ range };
    }

    /// \brief Get an iterator past the last element in a range.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto end(Immutable<TRange> range) noexcept
    {
        return Ranges::Details::RangeIterator<TRange>{};
    }
}

// ===========================================================================
