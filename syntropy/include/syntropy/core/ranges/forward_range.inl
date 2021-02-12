
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
    Copy(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto source = lhs;
        auto destination = rhs;

        for (; !Details::RouteIsEmpty(source)
               && !Details::RouteIsEmpty(destination);)
        {
            Details::RouteFront(destination) = Details::RouteFront(source);

            source = Details::RoutePopFront(source);
            destination = Details::RoutePopFront(destination);
        }

        return { source , destination };
    }

    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange>
    Move(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto source = lhs;
        auto destination = rhs;

        for (; !Details::RouteIsEmpty(source)
               && !Details::RouteIsEmpty(destination);)
        {
            Details::RouteFront(destination)
                = Syntropy::Move(Details::RouteFront(source));

            source = Details::RoutePopFront(source);
            destination = Details::RoutePopFront(destination);
        }

        return { source , destination };
    }

    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange>
    Swap(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto source = lhs;
        auto destination = rhs;

        for (; !Details::RouteIsEmpty(source)
               && !Details::RouteIsEmpty(destination);)
        {
            Algorithm::Swap(Details::RouteFront(source),
                            Details::RouteFront(destination));

            source = Details::RoutePopFront(source);
            destination = Details::RoutePopFront(destination);
        }

        return { source , destination };
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
