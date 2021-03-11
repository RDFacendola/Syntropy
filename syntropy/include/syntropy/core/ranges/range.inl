
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
    {
        return Details::RouteViewOf(Forward<TRange>(range));
    }

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range.
    template <Ranges::Concepts::Range TRange>
    constexpr auto begin(Forwarding<TRange> range) noexcept
    {
        return begin(Ranges::Details::RouteViewOf(Forward<TRange>(range)));
    }

    /// \brief Get an iterator past the last element in a range.
    template <Ranges::Concepts::Range TRange>
    constexpr auto end(Forwarding<TRange> range) noexcept
    {
        return end(Ranges::Details::RouteViewOf(Forward<TRange>(range)));
    }
}

// ===========================================================================
