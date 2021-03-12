
/// \file bidirectional_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE EXTENSIONS                                       */
    /************************************************************************/

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto)
    Back(Immutable<TRange> range) noexcept
    {
        return Details::RouteBack(range);
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    PopBack(Immutable<TRange> range) noexcept
    {
        return Details::RoutePopBack(range);
    }
}

// ===========================================================================
