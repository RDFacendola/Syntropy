
/// \file range.h
///
/// \brief This header is  part of the Syntropy core module.
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
    /* RANGE VIEW                                                           */
    /************************************************************************/

    /// \brief Interface for range views which are used to access individual
    ///        elements.
    template <typename TRangeView>
    concept RangeView = requires(Immutable<TRangeView> range_view)
    {
        /// \brief Access a view element via dereferencing operator.
        { *range_view };
    };

    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose element can be visited in
    ///        some order.
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept BaseRange = requires(Forwarding<TRange> range)
    {
        /// \brief Get a view to a range' elements.
        { Details::RouteViewOf(Forward<TRange>(range)) } -> RangeView;
    };

    /// \brief Collection of elements that can be visited in some order.
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept Range = BaseRange<TRange>;
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

    /// \brief Type of a ranges' elements.
    template <Ranges::Concepts::Range TRange>
    using RangeElementTypeOf = decltype(
        *Syntropy::Templates::Declval<TRange>());

    /// \brief Value type of a range's elements.
    template <Ranges::Concepts::Range TRange>
    using RangeElementValueTypeOf = Syntropy::Templates::QualifiedOf<
        RangeElementTypeOf<TRange>>;
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

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range.
    template <Ranges::Concepts::Range TRange>
    constexpr auto begin(Forwarding<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a range.
    template <Ranges::Concepts::Range TRange>
    constexpr auto end(Forwarding<TRange> range) noexcept;

}

// ===========================================================================

#include "details/range.inl"

// ===========================================================================
