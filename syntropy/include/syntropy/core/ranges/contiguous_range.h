
/// \file contiguous_range.h
///
/// \brief This header is part of the Syntropy core module. It contains
///        definitions for ranges whose elements are allocated contiguously.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/ranges/random_access_range.h"
#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

#include "details/contiguous_range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* CONTIGUOUS RANGE VIEW                                                */
    /************************************************************************/

    /// \brief Minimal interface for views to ranges whose elements are
    ////       allocated contiguously.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept BaseContiguousRangeView =
        requires(Immutable<TRangeView> range_view)
        {
            /// \brief Access a range elements' storage.
            { Details::RouteData(range_view) };

            /// \brief Get the number of elements in a range view.
            { Details::RouteCount(range_view) };
        };

    /// \brief Range view whose elements are allocated contiguously.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept ContiguousRangeView = BaseContiguousRangeView<TRangeView>
        && RandomAccessRangeView<TRangeView>;

    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Container whose elements are allocated contiguously.
    ///
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept ContiguousRange = requires(Immutable<TRange> range)
    {
        { ViewOf(range) } -> ContiguousRangeView;
    };
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Contiguous range view.
    // ======================

    /// \brief Access a range elements' storage.
    ///
    /// \remarks Calling this method on an empty range results in undefined
    ///          behavior.
    template <Concepts::ContiguousRangeView TRangeView>
    [[nodiscard]] constexpr auto
    Data(Immutable<TRangeView> range) noexcept;

    /// \brief Check whether two range view elements are equal.
    template <Concepts::ContiguousRangeView TRangeView,
              Concepts::ContiguousRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRangeView> lhs,
             Immutable<URangeView> rhs) noexcept;

    /// \brief Check whether two contiguous range views intersects.
    template <Concepts::ContiguousRangeView TRangeView,
              Concepts::ContiguousRangeView URangeView>
    [[nodiscard]] constexpr Bool
    Intersect(Immutable<TRangeView> lhs,
              Immutable<URangeView> rhs) noexcept;

    /// \brief Idempotent range view.
    template <Concepts::ContiguousRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    ViewOf(Immutable<TRangeView> range_view) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* CONTIGUOUS RANGE VIEW EXTENSIONS                                     */
    /************************************************************************/

    /// \brief Access a range view element by index.
    ///
    /// \remarks Undefined behavior if range view boundaries are exceeded.
    /// \remarks This extension adapts contiguous range views to meet
    ///          random access range view concept.
    template <Concepts::BaseContiguousRangeView TRangeView>
    struct At<TRangeView>
    {
        template <typename TIndex>
        [[nodiscard]] decltype(auto)
        operator()(Immutable<TRangeView> range_view,
                   Immutable<TIndex> index) const noexcept;
    };

    /// \brief Obtain a sub-range-view.
    ///
    /// \remarks Undefined behavior if range view boundaries are exceeded.
    /// \remarks This extension adapts contiguous range views to meet
    ///          random access range view concept.
    template <Concepts::BaseContiguousRangeView TRangeView>
    struct Slice<TRangeView>
    {
        template <typename TIndex, typename TCount>
        [[nodiscard]] TRangeView
        operator()(Immutable<TRangeView> range_view,
                   Immutable<TIndex> index,
                   Immutable<TCount> count) const noexcept;
    };
}

// ===========================================================================

#include "details/contiguous_range.inl"

// ===========================================================================
