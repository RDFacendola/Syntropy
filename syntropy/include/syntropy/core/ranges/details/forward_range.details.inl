
/// \file forward_range.details.inl
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* RANGE VIEW ITERATOR                                                  */
    /************************************************************************/

    template <typename TRangeView>
    constexpr RangeViewIterator<TRangeView>
    ::RangeViewIterator(Immutable<TRangeView> range_view) noexcept
        : range_view_(range_view)
    {

    }

    template <typename TRangeView>
    [[nodiscard]] constexpr
    decltype(auto) RangeViewIterator<TRangeView>
    ::operator*() const noexcept
    {
        return RouteFront(range_view_);
    }

    template <typename TRangeView>
    constexpr Mutable<RangeViewIterator<TRangeView>>
    RangeViewIterator<TRangeView>
    ::operator++() noexcept
    {
        range_view_ = RoutePopFront(range_view_);

        return *this;
    }

    template <typename TRangeView>
    [[nodiscard]] constexpr Bool RangeViewIterator<TRangeView>
    ::operator==(Immutable<RangeViewIterator> other) const noexcept
    {
        SYNTROPY_ASSERT(RouteIsEmpty(other.range_view_));

        return RouteIsEmpty(range_view_);
    }

}

// ===========================================================================
