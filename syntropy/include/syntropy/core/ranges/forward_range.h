
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

#include "syntropy/core/tuples/tuple.h"

#include "syntropy/diagnostics/assert.h"

// ===========================================================================

#include "details/forward_range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Range's element reference type.
    template <typename TRange>
    using RangeElementOf = decltype(
        Details::RouteFront(Syntropy::Templates::Declval<TRange>()));

    /// \brief Range's element value type.
    template <typename TRange>
    using RangeElementValueType = Syntropy::Templates::QualifiedOf<
        RangeElementOf<TRange>>;

}

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose elements can be
    ///        visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BaseForwardRange = requires(Immutable<TRange> range)
    {
        /// \brief Access range's first element.
        { Details::RouteFront(range) };

        /// \brief Discard range's first element and return the resulting
        ///        range.
        { Details::RoutePopFront(range) };

        /// \brief Check whether the range is empty.
        { Details::RouteIsEmpty(range) };
    };

    /// \brief Range whose elements can be visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ForwardRange = BaseForwardRange<TRange>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Forward range.
    // ==============

    /// \brief Access range's first element.
    /// \remarks Accessing the first element of an empty range results
    ///          in undefined behavior.
    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr decltype(auto)
    Front(Immutable<TRange> range) noexcept;

    /// \brief Discard range's first element and return the resulting range.
    /// \remarks If the provided range is empty, the behavior of this method
    ///          is undefined.
    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr TRange
    PopFront(Immutable<TRange> range) noexcept;

    /// \brief Check whether the range is empty.
    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<TRange> range) noexcept;

    /// \brief Apply a function to each element in the range.
    template <Concepts::ForwardRange TRange, typename TFunction>
    constexpr void
    ForEach(Immutable<TRange> range, TFunction function) noexcept;

    /// \brief Advance both ranges in lockstep, copy elements from the source
    ///        range to the destination range until either is exhausted.
    /// \return Returns the ranges to the elements that were not copied: at
    ///         least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange>
    Copy(Immutable<TRange> destination, Immutable<URange> source) noexcept;

    /// \brief Advance both ranges in lockstep, moving elements from the source
    ///        range to the destination range until either is exhausted.
    /// \return Returns the ranges to the elements that were not copied: at
    ///         least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange>
    Move(Immutable<TRange> destination, Immutable<URange> source) noexcept;

    /// \brief Advance both ranges in lockstep, swapping elements from both
    ///        until either lhs or rhs is exhausted.
    /// \return Returns the ranges to the elements that were not swapped: at
    ///         least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange>
    Swap(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto begin(Immutable<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a range.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto end(Immutable<TRange> range) noexcept;
}

// ===========================================================================

#include "details/forward_range.inl"

// ===========================================================================
