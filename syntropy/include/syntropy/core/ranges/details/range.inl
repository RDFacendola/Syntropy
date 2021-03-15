
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
    [[nodiscard]] constexpr Templates::RangeViewTypeOf<TRange>
    ViewOf(Forwarding<TRange> range) noexcept
    {
        using Details::RouteViewOf;

        return RouteViewOf(Forward<TRange>(range));
    }
}

// ===========================================================================
