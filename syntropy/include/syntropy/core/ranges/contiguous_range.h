
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

    /// \brief Minimal interface for range views whose elements are allocated
    ///        contiguously.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept BaseContiguousRangeView =
        requires(Immutable<TRangeView> range_view)
        {
            /// \brief Access range's element storage.
            { Details::RouteData(range_view) };

            /// \brief Get range's elements count.
            { Details::RouteCount(range_view) };
        };

    /// \brief Range view whose elements are allocated contiguously.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept ContiguousRangeView
        = BaseContiguousRangeView<TRangeView> && RandomAccessRangeView<TRangeView>;

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Contiguous range view.
    // ======================

    /// \brief Access range view's element storage.
    ///
    /// \remarks Accessing an empty range view's storage results in
    ///          undefined behavior.
    template <Concepts::ContiguousRangeView TRangeView>
    [[nodiscard]] constexpr auto
    Data(Immutable<TRangeView> range) noexcept;

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::ContiguousRangeView TRangeView,
              Concepts::ContiguousRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept;

    /// \brief Check whether two contiguous range views intersects.
    template <Concepts::ContiguousRangeView TRangeView,
              Concepts::ContiguousRangeView URangeView>
    [[nodiscard]] constexpr Bool
    Intersect(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* CONTIGUOUS RANGE VIEW EXTENSIONS                                     */
    /************************************************************************/

    /// \brief Access range view's element by index.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    /// \remarks This extension adapts RandomAccessRangeView type such that all
    ///          its instances are also BidirectionalRanges and SizedRanges.
    template <Concepts::BaseContiguousRangeView TRangeView>
    struct At<TRangeView>
    {
        template <typename TIndex>
        [[nodiscard]] decltype(auto)
        operator()(Immutable<TRangeView> range_view,
                   Immutable<TIndex> index) const noexcept;
    };

    /// \brief Obtain a range view to a sub-range-view.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    /// \remarks This extension adapts RandomAccessRange type such that all its
    ///          instances are also BidirectionalRanges and SizedRanges.
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
