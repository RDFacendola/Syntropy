
/// \file contiguous_range.h
///
/// \brief This header is part of the Syntropy core module. It contains
///        definitions for ranges whose elements are allocated contiguously.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/core/foundation/tuple.h"

#include "syntropy/core/concepts/details/contiguous_range_details.h"

#include "syntropy/core/concepts/random_access_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose elements are allocated
    ///        contiguously.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BaseContiguousRange = requires(Immutable<TRange> range)
        {
            /// \brief Access range's element storage.
            { Details::RouteData(range) };

            /// \brief Get range's elements count.
            { Details::RouteCount(range) };
        };

    /// \brief Range whose elements are allocated contiguously.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ContiguousRange
        = BaseContiguousRange<TRange> && RandomAccessRange<TRange>;

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Contiguous range.
    // =================

    /// \brief Access range's element storage.
    ///
    /// \remarks Accessing an empty range's storage results in
    ///          undefined behavior.
    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] constexpr auto
    Data(Immutable<TRange> range) noexcept;

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::ContiguousRange TRange,
              Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* CONTIGUOUS RANGE EXTENSIONS                                          */
    /************************************************************************/

    /// \brief Access range's element storage.
    template <typename TType>
    struct Data;

    /// \brief Access range's element by index.
    ///
    /// \remarks Exceeding range boundaries results in undefined behavior.
    /// \remarks This extension adapts RandomAccessRange type such that all its
    ///          instances are also BidirectionalRanges and SizedRanges.
    template <Concepts::BaseContiguousRange TRange>
    struct At<TRange>
    {
        template <typename TIndex>
        [[nodiscard]] decltype(auto)
        operator()(Immutable<TRange> range,
                   Immutable<TIndex> index) const noexcept;
    };

    /// \brief Obtain a view to a sub-range.
    ///
    /// \remarks Exceeding range boundaries results in undefined behavior.
    /// \remarks This extension adapts RandomAccessRange type such that all its
    ///          instances are also BidirectionalRanges and SizedRanges.
    template <Concepts::BaseContiguousRange TRange>
    struct Slice<TRange>
    {
        template <typename TIndex, typename TCount>
        [[nodiscard]] TRange
        operator()(Immutable<TRange> range,
                   Immutable<TIndex> index,
                   Immutable<TCount> count) const noexcept;
    };
}

// ===========================================================================

#include "contiguous_range.inl"

// ===========================================================================
