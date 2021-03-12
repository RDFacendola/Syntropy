
/// \file forward_range_.details.inl
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* RANGE ITERATOR                                                       */
    /************************************************************************/

    template <typename TRange>
    constexpr RangeIterator<TRange>
    ::RangeIterator(Immutable<TRange> range) noexcept
        : range_(range)
    {

    }

    template <typename TRange>
    [[nodiscard]] constexpr
    decltype(auto) RangeIterator<TRange>
    ::operator*() const noexcept
    {
        return RouteFront(range_);
    }

    template <typename TRange>
    constexpr Mutable<RangeIterator<TRange>> RangeIterator<TRange>
    ::operator++() noexcept
    {
        range_ = RoutePopFront(range_);

        return *this;
    }

    template <typename TRange>
    [[nodiscard]] constexpr Bool RangeIterator<TRange>
    ::operator==(Immutable<RangeIterator> other) const noexcept
    {
        SYNTROPY_ASSERT(RouteIsEmpty(other.range_));

        return RouteIsEmpty(range_);
    }

}

// ===========================================================================
