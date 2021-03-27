
/// \file sized_range.h
///
/// \brief This header is part of the Syntropy core module. It contains
///        definitions for ranges whose elements can be visited sequentially
///        and whose size can be computed in constant time.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/ranges/forward_range.h"
#include "syntropy/core/algorithms/compare.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    /// \brief Concept for range whose elements can be visited sequentially and
    ///        whose size can be computed in constant time.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept SizedRange = ForwardRange<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Get range's elements count.
            { Ranges::Count(range) };
        };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // SizedRange.
    // ===========

    /// \brief Check whether two ranges are element-wise equal.
    ///
    /// \remarks Equality implies equivalence, therefore if this method returns
    ///          true AreEquivalent also returns true.
    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Check whether two ranges are element-wise equivalent.
    ///
    /// \remarks Equality implies equivalence but not the other way around!
    ///          If AreEqual returns false this method can either return true
    ///          or false.
    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Compare two range views lexicographically.
    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

}

// ===========================================================================

#include "details/sized_range.inl"

// ===========================================================================
