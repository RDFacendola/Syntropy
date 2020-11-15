
/// \file reverse_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for adapters used to iterate a range in reverse order.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/experimental/core/foundation/range.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    /// \brief Adapter class used to reverse bidirectional ranges.
    /// \author Raffaele D. Facendola - November 2020.
    template <Concepts::BidirectionalRangeT TRange>
    class ReverseRange
    {
        template <Concepts::BidirectionalRangeT TRange>
        friend constexpr Bool IsEmpty(const ReverseRange<TRange>& range) noexcept;

        template <Concepts::BidirectionalRangeT TRange>
        friend constexpr decltype(auto) Front(const ReverseRange<TRange>& range) noexcept;

        template <Concepts::BidirectionalRangeT TRange>
        friend constexpr decltype(auto) Back(const ReverseRange<TRange>& range) noexcept;

        template <Concepts::BidirectionalRangeT TRange>
        friend constexpr ReverseRange<TRange> PopFront(const ReverseRange<TRange>& range, Int count) noexcept;

        template <Concepts::BidirectionalRangeT TRange>
        friend constexpr ReverseRange<TRange> PopBack(const ReverseRange<TRange>& range, Int count) noexcept;

        template <Concepts::BidirectionalRangeT TRange>
        friend constexpr TRange Reverse(const ReverseRange<TRange>& range) noexcept;

    public:

        /// \brief Create a new reverse range.
        constexpr ReverseRange(const TRange& range) noexcept;

    private:

        /// \brief Underlying range.
        TRange range_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // ReverseRange.
    // =============

    /// \brief Check whether a reverse range is empty.
    /// \return Returns true if the range is empty, returns false otherwise.
    template <Concepts::BidirectionalRangeT TRange>
    constexpr Bool IsEmpty(const ReverseRange<TRange>& range) noexcept;

    /// \brief Access the first element in a range.
    /// \remarks Accessing the first element of an empty range results in undefined behavior.
    template <Concepts::BidirectionalRangeT TRange>
    constexpr decltype(auto) Front(const ReverseRange<TRange>& range) noexcept;

    /// \brief Access the last element in a range.
    /// \remarks Accessing the last element of an empty range results in undefined behavior.
    template <Concepts::BidirectionalRangeT TRange>
    constexpr decltype(auto) Back(const ReverseRange<TRange>& range) noexcept;

    /// \brief Discard the first count elements in a range and return the resulting subrange.
    /// \remarks If this method would cause the subrange to exceed the original range, the behavior of this method is undefined.
    template <Concepts::BidirectionalRangeT TRange>
    constexpr ReverseRange<TRange> PopFront(const ReverseRange<TRange>& range, Int count = 1) noexcept;

    /// \brief Discard the last count elements in a range and return the resulting subrange.
    /// \remarks If this method would cause the subrange to exceed the original range, the behavior of this method is undefined.
    template <Concepts::BidirectionalRangeT TRange>
    constexpr ReverseRange<TRange> PopBack(const ReverseRange<TRange>& range, Int count = 1) noexcept;

    // Utilities.
    // ==========

    /// \brief Reverse a bidirectional range.
    template <Concepts::BidirectionalRangeT TRange>
    constexpr ReverseRange<TRange> Reverse(const TRange& range) noexcept;

    /// \brief Reverse a reversed range.
    template <Concepts::BidirectionalRangeT TRange>
    constexpr TRange Reverse(const ReverseRange<TRange>& range) noexcept;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ReverseRange.
    // ============

    template <Concepts::BidirectionalRangeT TRange>
    constexpr ReverseRange<TRange>::ReverseRange(const TRange& range) noexcept
        : range_(range)
    {

    }

    // Non-member functions.
    // =====================

    // ReverseRange.

    template <Concepts::BidirectionalRangeT TRange>
    constexpr Bool IsEmpty(const ReverseRange<TRange>& range) noexcept
    {
        return IsEmpty(range.range_);
    }

    template <Concepts::BidirectionalRangeT TRange>
    constexpr decltype(auto) Front(const ReverseRange<TRange>& range) noexcept
    {
        return Back(range.range_);
    }

    template <Concepts::BidirectionalRangeT TRange>
    constexpr decltype(auto) Back(const ReverseRange<TRange>& range) noexcept
    {
        return Front(range.range_);
    }

    template <Concepts::BidirectionalRangeT TRange>
    constexpr ReverseRange<TRange> PopFront(const ReverseRange<TRange>& range, Int count) noexcept
    {
        return PopBack(range.range_, count);
    }

    template <Concepts::BidirectionalRangeT TRange>
    constexpr ReverseRange<TRange> PopBack(const ReverseRange<TRange>& range, Int count) noexcept
    {
        return PopFront(range.range_, count);
    }

    // Utilities.

    template <Concepts::BidirectionalRangeT TRange>
    constexpr ReverseRange<TRange> Reverse(const TRange& range) noexcept
    {
        return range;
    }

    template <Concepts::BidirectionalRangeT TRange>
    constexpr TRange Reverse(const ReverseRange<TRange>& range) noexcept
    {
        return range.range_;
    }
}

// ===========================================================================