
/// \file random_access_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        in any order.
///
/// Ranges specifications based on the awesome
///     https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/ranges/bidirectional_range.h"
#include "syntropy/core/ranges/sized_range.h"
#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

#include "random_access_range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose elements can be visited
    ///        in any order.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BaseRandomAccessRange = requires(Immutable<TRange> range)
    {
        /// \brief Get range's elements count.
        { Details::RouteCount(range) };
    }
    && requires(Immutable<TRange> range,
                Immutable<Templates::RangeCountType<TRange>> index,
                Immutable<Templates::RangeCountType<TRange>> count)
    {
        /// \brief Access range's element by index.
        { Details::RouteAt(range, index) };

        /// \brief Obtain a view to a sub-range.
        { Details::RouteSlice(range, index, count) };
    };

    /// \brief Range whose elements can be visited in any order.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept RandomAccessRange
         = BaseRandomAccessRange<TRange>
        && BidirectionalRange<TRange>
        && SizedRange<TRange>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Random access range.
    // ====================

    /// \brief Access range's element by index.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TIndex
        = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr decltype(auto)
    At(Immutable<TRange> range, Immutable<TIndex> index) noexcept;

    /// \brief Obtain a view to a sub-range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TIndex
        = Templates::RangeCountType<TRange>, typename TCount = TIndex>
    [[nodiscard]] constexpr TRange
    Slice(Immutable<TRange> range,
          Immutable<TIndex> index,
          Immutable<TCount> count) noexcept;

    /// \brief Take a number of elements from the range's front.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount
        = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr TRange
    Front(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Take a number of elements from the range's back.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount
        = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr TRange
    Back(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Discard the first elements in a range and return the resulting
    ///        subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount
        = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr TRange
    PopFront(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Discard the last elements in a range and return the resulting
    ///        subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount
        = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr TRange
    PopBack(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Slice a range returning the first element and a subrange to
    ///        the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined
    ///          behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range) noexcept;

    /// \brief Slice a range returning the last element and a subrange to the
    ///        remaining ones.
    /// \remarks Calling this method with an empty range results in undefined
    ///          behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range) noexcept;

    /// \brief Slice a range returning a subrange to the first count elements
    ///        and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount
        = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Slice a range returning a subrange to the last count elements
    ///        and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount
        = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range, Immutable<TCount> count) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE EXTENSIONS                                       */
    /************************************************************************/

    /// \brief Access range's first element.
    /// \remarks Accessing the first element of an empty range results in
    ///          undefined behavior.
    /// \remarks This extension adapts RandomAccessRange type such that all
    ///          its instances are also BidirectionalRanges and SizedRanges.
    template <Concepts::BaseRandomAccessRange TRange>
    struct Front<TRange>
    {
        [[nodiscard]] decltype(auto)
        operator()(Immutable<TRange> range) const noexcept;
    };

    /// \brief Discard range's first element and return the resulting range.
    /// \remarks If the provided range is empty, the behavior of this method
    ///          is undefined.
    /// \remarks This extension adapts RandomAccessRange type such that all
    ///          its instances are also BidirectionalRanges and SizedRanges.
    template <Concepts::BaseRandomAccessRange TRange>
    struct PopFront<TRange>
    {
        [[nodiscard]] TRange
        operator()(Immutable<TRange> range) const noexcept;
    };

    /// \brief Access range's last element.
    /// \remarks Accessing the last element of an empty range results in
    ///          undefined behavior.
    /// \remarks This extension adapts RandomAccessRange type such that all
    ///          its instances are also BidirectionalRanges and SizedRanges.
    template <Concepts::BaseRandomAccessRange TRange>
    struct Back<TRange>
    {
        [[nodiscard]] decltype(auto)
        operator()(Immutable<TRange> range) const noexcept;
    };

    /// \brief Discard range's last element and return the resulting range.
    /// \remarks If the provided range is empty, the behavior of this method
    ///          is undefined.
    /// \remarks This extension adapts RandomAccessRange type such that all
    ///          its instances are also BidirectionalRanges and SizedRanges.
    template <Concepts::BaseRandomAccessRange TRange>
    struct PopBack<TRange>
    {
        [[nodiscard]] TRange
        operator()(Immutable<TRange> range) const noexcept;
    };
}

// ===========================================================================

#include "random_access_range.inl"

// ===========================================================================
