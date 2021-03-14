
/// \file range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definition for ranges, homogeneous collection of
///        elements that can be visited in some order.
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

#include "details/range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    /// \brief A collection of elements that can be visited in some order.
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept Range = requires(Forwarding<TRange> range)
    {
        /// \brief Get a view to a range' elements.
        { Details::RouteViewOf(Forward<TRange>(range)) };
    };
}

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Type of a range's view.
    template <Ranges::Concepts::Range TRange>
    using RangeViewTypeOf = decltype(
        Details::RouteViewOf(Syntropy::Templates::Declval<TRange>()));
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Range views.
    // ============

    /// \brief Get a view to a range' elements.
    template <Concepts::Range TRange>
    [[nodiscard]] constexpr Templates::RangeViewTypeOf<TRange>
    ViewOf(Forwarding<TRange> range) noexcept;

}

// ===========================================================================

#include "details/range.inl"

// ===========================================================================