
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
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Range.
    // ======

    /// \brief Get a view to a range' elements.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    ViewOf(Forwarding<TRange> range) noexcept
        -> decltype(Details::RouteViewOf(Forward<TRange>(range)));

    /// \brief Check whether a range view is empty.
    template <typename TRange>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<TRange> range) noexcept;

    /// \brief Get the number of elements in a range view.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Count(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteCount(Ranges::ViewOf(range)));

    /// \brief Access the first element in a range view.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Front(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteFront(Ranges::ViewOf(range)));

    /// \brief Access the last element in a range view.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Back(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteBack(Ranges::ViewOf(range)));

    /// \brief Discard the first element in a range view and return a view
    ///        to the remaining elements.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    PopFront(Immutable<TRange> range) noexcept
        -> decltype(Details::RoutePopFront(Ranges::ViewOf(range)));

    /// \brief Discard the last element in a range view and return a view to
    ///        the remaining elements.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    PopBack(Immutable<TRange> range) noexcept
        -> decltype(Details::RoutePopBack(Ranges::ViewOf(range)));

    /// \brief Access range view's element by index.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <typename TRange, typename TIndex>
    [[nodiscard]] constexpr auto
    At(Immutable<TRange> range, Immutable<TIndex> index) noexcept
        -> decltype(Details::RouteAt(Ranges::ViewOf(range), index));

    /// \brief Access a range elements' storage.
    ///
    /// \remarks Calling this method on an empty range results in undefined
    ///          behavior.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Data(Immutable<TRange> range) noexcept
        -> decltype(Details::RouteData(Ranges::ViewOf(range)));

}

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
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

    /// \brief Type of a range's cardinality.
    template <typename TRangeView>
    using RangeCountType = Syntropy::Templates::UnqualifiedOf<decltype(
        Details::RouteCount(
            Ranges::ViewOf(
                Syntropy::Templates::Declval<TRangeView>())))>;

    /// \brief Type of a range elements.
    template <typename TRangeView>
    using RangeElementTypeOf = decltype(
        Details::RouteFront(
            Ranges::ViewOf(
                Syntropy::Templates::Declval<TRangeView>())));

    /// \brief Value type of a range's elements.
    template <typename TRangeView>
    using RangeElementValueTypeOf = Syntropy::Templates::QualifiedOf
        <RangeElementTypeOf<TRangeView>>;
}

// ===========================================================================

#include "details/range.inl"

// ===========================================================================
