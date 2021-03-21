
/// \file range.inl
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Range.
    // =====

    template <Concepts::Range TRange>
    [[nodiscard]] constexpr auto
    ViewOf(Forwarding<TRange> range) noexcept
        -> decltype(Details::RouteViewOf(Forward<TRange>(range)))
    {
        return Details::RouteViewOf(Forward<TRange>(range));
    }

    template <typename TRange>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<TRange> range) noexcept
    {
        return Details::RouteIsEmpty(Ranges::ViewOf(range));
    }

    template <typename TRange>
    [[nodiscard]] constexpr auto
    Count(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteCount(Ranges::ViewOf(range)))
    {
        return Details::RouteCount(Ranges::ViewOf(range));
    }

    template <typename TRange>
    [[nodiscard]] constexpr auto
    Front(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteFront(Ranges::ViewOf(range)))
    {
        return Details::RouteFront(Ranges::ViewOf(range));
    }

    template <typename TRange>
    [[nodiscard]] constexpr auto
    Back(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteBack(Ranges::ViewOf(range)))
    {
        return Details::RouteBack(Ranges::ViewOf(range));
    }

    template <typename TRange>
    [[nodiscard]] constexpr auto
    PopFront(Immutable<TRange> range) noexcept
        -> decltype(Details::RoutePopFront(Ranges::ViewOf(range)))
    {
        return Details::RoutePopFront(Ranges::ViewOf(range));
    }

    template <typename TRange>
    [[nodiscard]] constexpr auto
    PopBack(Immutable<TRange> range) noexcept
        -> decltype(Details::RoutePopBack(Ranges::ViewOf(range)))
    {
        return Details::RoutePopBack(Ranges::ViewOf(range));
    }

    template <typename TRange, typename TIndex>
    [[nodiscard]] constexpr auto
    At(Immutable<TRange> range, Immutable<TIndex> index) noexcept
        -> decltype(Details::RouteAt(Ranges::ViewOf(range), index))
    {
        return Details::RouteAt(Ranges::ViewOf(range), index);
    }

    template <typename TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    Select(Immutable<TRange> range,
           Immutable<TCardinality> offset,
           Immutable<TCardinality> count) noexcept
       -> decltype(Details::RouteSelect(Ranges::ViewOf(range), offset, count))
    {
        return Details::RouteSelect(Ranges::ViewOf(range), offset, count);
    }
    
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Data(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteData(Ranges::ViewOf(range)))
    {
        return Details::RouteData(Ranges::ViewOf(range));
    }

}

// ===========================================================================
