
/// \file random_access_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges whose elements can be visited in any order.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/core/foundation/tuple.h"

#include "syntropy/core/concepts/details/random_access_range_details.h"

#include "syntropy/core/concepts/bidirectional_range.h"
#include "syntropy/core/concepts/sized_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Range whose elements can be visited in any order.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BaseRandomAccessRange = requires(Immutable<TRange> range, Immutable<Templates::RangeCountType<TRange>> index, Immutable<Templates::RangeCountType<TRange>> count)
    {
        /// \brief Access range's element by index.
        { Details::RouteAt(range, index) };

        /// \brief Obtain a view to a sub-range.
        { Details::RouteSlice(range, index, count) };

        /// \brief Get range's elements count.
        { Details::RouteCount(range) };
    };

    /// \brief Range whose elements can be visited in any order.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept RandomAccessRange = BaseRandomAccessRange<TRange> && BidirectionalRange<TRange> && SizedRange<TRange>;

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
    template <Concepts::RandomAccessRange TRange, typename TIndex = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr Templates::RangeElementReferenceType<TRange> At(Immutable<TRange> range, Immutable<TIndex> index) noexcept;

    /// \brief Obtain a view to a sub-range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TIndex = Templates::RangeCountType<TRange>, typename TCount = TIndex>
    [[nodiscard]] constexpr TRange Slice(Immutable<TRange> range, Immutable<TIndex> index, Immutable<TCount> count) noexcept;

    /// \brief Take a number of elements from the range's front.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr TRange Front(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Take a number of elements from the range's back.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr TRange Back(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Discard the first elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Discard the last elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Slice a range returning the first element and a subrange to the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::RangeElementReferenceType<TRange>, TRange> SliceFront(Immutable<TRange> range) noexcept;

    /// \brief Slice a range returning the last element and a subrange to the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::RangeElementReferenceType<TRange>, TRange> SliceBack(Immutable<TRange> range) noexcept;

    /// \brief Slice a range returning a subrange to the first count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceFront(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Slice a range returning a subrange to the last count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange, typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceBack(Immutable<TRange> range, Immutable<TCount> count) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE EXTENSIONS                                       */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Access range's element by index.
    template <typename TType>
    struct At;

    /// \brief Obtain a view to a sub-range.
    template <typename TType>
    struct Slice;

    /// \brief Invokes a non-member function via ADL.
    template <Concepts::BaseRandomAccessRange TRange>
    struct Front<TRange>
    {
        [[nodiscard]] decltype(auto) operator()(Immutable<TRange> range) const noexcept;
    };

    /// \brief Invokes a non-member function via ADL.
    template <Concepts::BaseRandomAccessRange TRange>
    struct PopFront<TRange>
    {
        [[nodiscard]] TRange operator()(Immutable<TRange> range) const noexcept;
    };

    /// \brief Invokes a non-member function via ADL.
    template <Concepts::BaseRandomAccessRange TRange>
    struct Back<TRange>
    {
        [[nodiscard]] decltype(auto) operator()(Immutable<TRange> range) const noexcept;
    };

    /// \brief Invokes a non-member function via ADL.
    template <Concepts::BaseRandomAccessRange TRange>
    struct PopBack<TRange>
    {
        [[nodiscard]] TRange operator()(Immutable<TRange> range) const noexcept;
    };
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Random access range.

    template <Concepts::RandomAccessRange TRange, typename TIndex>
    [[nodiscard]] constexpr Templates::RangeElementReferenceType<TRange> At(Immutable<TRange> range, Immutable<TIndex> index) noexcept
    {
        return Details::RouteAt(range, index);
    }

    template <Concepts::RandomAccessRange TRange, typename TIndex, typename TCount>
    [[nodiscard]] constexpr TRange Slice(Immutable<TRange> range, Immutable<TIndex> index, Immutable<TCount> count) noexcept
    {
        return Details::RouteSlice(range, index, count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr TRange Front(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Ranges::Slice(range, Templates::RangeCountType<TRange>{ 0 }, count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr TRange Back(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Ranges::Slice(range, Ranges::Count(range) - count, count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Ranges::Slice(range, count, Ranges::Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return Ranges::Slice(range, Templates::RangeCountType<TRange>{ 0 }, Ranges::Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::RangeElementReferenceType<TRange>, TRange> SliceFront(Immutable<TRange> range) noexcept
    {
        return Tuples::MakeTuple(Ranges::Front(range), Ranges::PopFront(range));
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::RangeElementReferenceType<TRange>, TRange> SliceBack(Immutable<TRange> range) noexcept
    {
        return Tuples::MakeTuple(Ranges::Back(range), Ranges::PopBack(range));
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceFront(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return { Ranges::Front(range, count), Ranges::PopFront(range, count) };
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceBack(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        return { Ranges::Back(range, count), Ranges::PopBack(range, count) };
    }

}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Random access range extensions.
    // ===============================

    template <Concepts::BaseRandomAccessRange TRange>
    [[nodiscard]] inline decltype(auto) Front<TRange>::operator()(Immutable<TRange> range) const noexcept
    {
        using TCount = Templates::RangeCountType<TRange>;

        return Details::RouteAt(range, TCount{ 0 });
    }

    template <Concepts::BaseRandomAccessRange TRange>
    [[nodiscard]] inline TRange PopFront<TRange>::operator()(Immutable<TRange> range) const noexcept
    {
        using TCount = Templates::RangeCountType<TRange>;

        return Details::RouteSlice(range, TCount{ 1 }, Details::RouteCount(range) - TCount{ 1 });
    };

    template <Concepts::BaseRandomAccessRange TRange>
    [[nodiscard]] inline decltype(auto) Back<TRange>::operator()(Immutable<TRange> range) const noexcept
    {
        using TCount = Templates::RangeCountType<TRange>;

        return Details::RouteAt(range, Details::RouteCount(range) - TCount{ 1 });
    };

    template <Concepts::BaseRandomAccessRange TRange>
    [[nodiscard]] inline TRange PopBack<TRange>::operator()(Immutable<TRange> range) const noexcept
    {
        using TCount = Templates::RangeCountType<TRange>;

        return Details::RouteSlice(range, TCount{ 0 }, Details::RouteCount(range) - TCount{ 1 });
    }
}

// ===========================================================================