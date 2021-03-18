
/// \file bidirectional_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE VIEW EXTENSIONS                                  */
    /************************************************************************/

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto)
    Back(Immutable<TRangeView> range_view) noexcept
    {
        using Details::RouteBack;

        return RouteBack(range_view);
    }

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    PopBack(Immutable<TRangeView> range_view) noexcept
    {
        using Details::RoutePopBack;

        return RoutePopBack(range_view);
    }

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    ViewOf(Immutable<TRangeView> range_view) noexcept
    {
        return range_view;
    }

}

// ===========================================================================
