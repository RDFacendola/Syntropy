
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
#include "syntropy/core/support/compare.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/ranges/forward_range.h"
#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

#include "details/sized_range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Type of a range's cardinality.
    template <typename TRangeView>
    using RangeViewCountType = Syntropy::Templates::UnqualifiedOf<decltype(
        Details::RouteCount(Syntropy::Templates::Declval<TRangeView>()))>;
}

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* SIZED RANGE VIEW                                                     */
    /************************************************************************/

    /// \brief Minimal interface for range views whose elements can be visited
    ///        sequentially and whose size can be computed in constant time.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept BaseSizedRangeView = requires(Immutable<TRangeView> range_view)
        {
            /// \brief Get range's elements count.
            { Details::RouteCount(range_view) };
        };

    /// \brief Range view whose elements can be visited sequentially and whose
    ///        size can be computed in constant time.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept SizedRangeView = BaseSizedRangeView<TRangeView>
        && ForwardRangeView<TRangeView>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get the number of elements in a range view.
    template <Concepts::SizedRangeView TRangeView>
    [[nodiscard]] constexpr auto
    Count(Immutable<TRangeView> range_view) noexcept;

    /// \brief Check whether elements in two range views are equal.
    ///
    /// \remarks Equality implies equivalence, therefore if this method returns
    ///          true AreEquivalent also returns true.
    template <Concepts::SizedRangeView TRangeView,
              Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRangeView> lhs,
             Immutable<URangeView> rhs) noexcept;

    /// \brief Check whether elements in two range views are equivalent.
    ///
    /// \remarks Equality implies equivalence but not the other way around!
    ///          If AreEqual returns false this method can either return true
    ///          or false.
    template <Concepts::SizedRangeView TRangeView,
              Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRangeView> lhs,
                  Immutable<URangeView> rhs) noexcept;

    /// \brief Compare two range views lexicographically.
    template <Concepts::SizedRangeView TRangeView,
              Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRangeView> lhs,
            Immutable<URangeView> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* SIZED RANGE EXTENSIONS                                               */
    /************************************************************************/

    /// \brief Check whether a range view is empty.
    ///
    /// \remarks This extension adapts SizedRangeView type such that all
    ///          its instances are also ForwardRangeViews.
    template <Concepts::BaseSizedRangeView TRangeView>
    struct IsEmpty<TRangeView>
    {
        Bool operator()(Immutable<TRangeView> range_view) const noexcept;
    };
}

// ===========================================================================

#include "details/sized_range.inl"

// ===========================================================================
