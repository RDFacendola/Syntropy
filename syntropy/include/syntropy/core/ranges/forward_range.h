
/// \file forward_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        sequentially.
///
/// Ranges specifications based on the awesome
///     https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/ranges/range.h"
#include "syntropy/core/tuples/tuple.h"

#include "syntropy/diagnostics/assert.h"

// ===========================================================================

#include "details/forward_range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* FORWARD RANGE VIEW                                                   */
    /************************************************************************/

    /// \brief Minimal interface for range views whose elements can be
    ///        visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept BaseForwardRangeView = requires(Immutable<TRangeView> range_view)
    {
        /// \brief Access range view's first element.
        { Details::RouteFront(range_view) };

        /// \brief Discard range view's first element and return the resulting
        ///        range_view.
        { Details::RoutePopFront(range_view) };

        /// \brief Check whether the range view is empty.
        { Details::RouteIsEmpty(range_view) };
    };

    /// \brief Range view whose elements can be visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept ForwardRangeView = BaseForwardRangeView<TRangeView>;
}

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Type of a range view's elements.
    template <typename TRangeView>
    using RangeViewElementTypeOf = decltype(
        Details::RouteFront(Syntropy::Templates::Declval<TRangeView>()));

    /// \brief Value type of a range's elements.
    template <typename TRangeView>
    using RangeViewElementValueTypeOf = Syntropy::Templates::QualifiedOf<
            RangeViewElementTypeOf<TRangeView>>;

    /// \brief Type of a range's elements.
    template <Ranges::Concepts::Range TRange>
    using RangeElementTypeOf = decltype(
        Details::RouteFront(
            ViewOf(Syntropy::Templates::Declval<TRange>())));

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

    // Forward range view.
    // ===================

    /// \brief Access range view's first element.
    /// \remarks Accessing the first element of an empty range view results
    ///          in undefined behavior.
    template <Concepts::ForwardRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto)
    Front(Immutable<TRangeView> range_view) noexcept;

    /// \brief Discard range view's first element and return the resulting
    ///        range view.
    /// \remarks If the provided range view is empty, the behavior of this
    ///          method is undefined.
    template <Concepts::ForwardRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    PopFront(Immutable<TRangeView> range_view) noexcept;

    /// \brief Check whether the range view is empty.
    template <Concepts::ForwardRangeView TRangeView>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<TRangeView> range_view) noexcept;

    /// \brief Apply a function to each element in the range view.
    template <Concepts::ForwardRangeView TRangeView, typename TFunction>
    constexpr void
    ForEach(Immutable<TRangeView> range_view, TFunction function) noexcept;

    /// \brief Advance both range views in lockstep, copy elements from the
    ///        source range view to the destination range view until either
    ///        is exhausted.
    /// \return Returns the range views to the elements that were not copied:
    ///         at least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Tuples::Tuple<TRangeView, URangeView>
    Copy(Immutable<TRangeView> destination,
         Immutable<URangeView> source) noexcept;

    /// \brief Advance both range views in lockstep, moving elements from the
    ///        source range view to the destination range view until either is
    ///        exhausted.
    /// \return Returns the range views to the elements that were not copied:
    ///         at least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Tuples::Tuple<TRangeView, URangeView>
    Move(Immutable<TRangeView> destination,
         Immutable<URangeView> source) noexcept;

    /// \brief Advance both range views in lockstep, swapping elements from
    ///        both until either lhs or rhs is exhausted.
    /// \return Returns the range views to the elements that were not swapped:
    ///         at least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRangeView TRangeView,
              Concepts::ForwardRangeView URangeView>
    constexpr Tuples::Tuple<TRangeView, URangeView>
    Swap(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range view.
    template <Ranges::Concepts::ForwardRangeView TRangeView>
    constexpr auto begin(Immutable<TRangeView> range_view) noexcept;

    /// \brief Get an iterator past the last element in a range view.
    template <Ranges::Concepts::ForwardRangeView TRangeView>
    constexpr auto end(Immutable<TRangeView> range_view) noexcept;
}

// ===========================================================================

#include "details/forward_range.inl"

// ===========================================================================
