
/// \file range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definition for ranges, homogeneous collection of
///        elements that can be visited in some order.
///
/// Ranges specifications based on the awesome
///     https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

#include "details/range.details.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* FUNCTIONS                                                            */
    /************************************************************************/

    // Range.
    // ======

    /// \brief Get a view to a range' elements.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    ViewOf(Forwarding<TRange> range) noexcept
        -> decltype(Details::RouteViewOf(Forward<TRange>(range)));

    /// \brief Check whether a range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<TRange> range) noexcept;

    /// \brief Get the number of elements in a range.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Count(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteCount(Ranges::ViewOf(range)));

    /// \brief Access the first element in a range.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Front(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteFront(Ranges::ViewOf(range)));

    /// \brief Access the last element in a range.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Back(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteBack(Ranges::ViewOf(range)));

    /// \brief Discard the first element in a range and return a range to the
    ///        remaining elements.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    PopFront(Immutable<TRange> range) noexcept
        -> decltype(Details::RoutePopFront(Ranges::ViewOf(range)));

    /// \brief Discard the last element in a range and return a range to the
    ///        remaining elements.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    PopBack(Immutable<TRange> range) noexcept
        -> decltype(Details::RoutePopBack(Ranges::ViewOf(range)));

    /// \brief Access range element by index.
    ///
    /// \remarks Undefined behavior if range boundaries are exceeded.
    template <typename TRange, typename TIndex>
    [[nodiscard]] constexpr auto
    At(Immutable<TRange> range, Immutable<TIndex> index) noexcept
        -> decltype(Details::RouteAt(Ranges::ViewOf(range), index));

    /// \brief Select a subrange of elements.
    ///
    /// \remarks Undefined behavior if range boundaries are exceeded.
    template <typename TRange, typename TCardinality>
    [[nodiscard]] constexpr auto
    Select(Immutable<TRange> range,
           Immutable<TCardinality> offset,
           Immutable<TCardinality> count) noexcept
        -> decltype(Details::RouteSelect(Ranges::ViewOf(range),
                                         offset,
                                         count));

    /// \brief Access a range elements' storage.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Data(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteData(Ranges::ViewOf(range)));

    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    /// \brief Concept for a collection of elements that can be visited in some
    ///        order.
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept Range = requires(TRange range)
    {
        /// \brief Get a view to a range' elements.
        { Ranges::ViewOf(range) };
    };

    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Type of a range's view.
    template <typename TRange>
    using RangeViewTypeOf = Templates::UnqualifiedOf<decltype(
        Ranges::ViewOf(Templates::Declval<TRange>()))>;

    /// \brief Type of a range's cardinality.
    template <typename TRange>
    using RangeCardinalityTypeOf = Templates::UnqualifiedOf<decltype(
        Ranges::Count(Templates::Declval<TRange>()))>;

    /// \brief Type of a range elements.
    template <typename TRange>
    using RangeElementTypeOf = Templates::UnqualifiedOf<decltype(
        Ranges::Front(Templates::Declval<TRange>()))>;

}

// ===========================================================================

#include "details/range.inl"

// ===========================================================================
