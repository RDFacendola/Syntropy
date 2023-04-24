
/// \file random_access_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        in any order.
///
/// Ranges specifications based on the awesome
///     https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - November 2020.
/// \author Raffaele D. Facendola - January 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/language/support/tuple.h"
#include "syntropy/core/ranges/bidirectional_range.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Concept for ranges whose elements can be visited in any order.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept RandomAccessRange = BidirectionalRange<TRange>
        && requires(Immutable<TRange> range, Int index)
        {
            /// \brief Access range element by index.
            { Ranges::At(range, index) };
        }
        && requires(
            Immutable<TRange> range, Int offset, Int count)
        {
            /// \brief Select a subrange of elements.
            { Ranges::Select(range, offset, count) };
        };

    /// \brief Tag type associated to random-access ranges.
    struct RandomAccessRangeTag{};

    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    namespace Ranges
    {
        /// \brief Take a number of elements from the range front.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <RandomAccessRange TRange>
        [[nodiscard]] constexpr auto
        Front(Immutable<TRange> range, Int count) noexcept;

        /// \brief Take a number of elements from the range back.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <RandomAccessRange TRange>
        [[nodiscard]] constexpr auto
        Back(Immutable<TRange> range, Int count) noexcept;

        /// \brief Discard the first elements in a range and return the
        ///        resulting subrange.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <RandomAccessRange TRange>
        [[nodiscard]] constexpr auto
        PopFront(Immutable<TRange> range, Int count) noexcept;

        /// \brief Discard the last elements in a range and return the
        ///        resulting subrange.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <RandomAccessRange TRange>
        [[nodiscard]] constexpr auto
        PopBack(Immutable<TRange> range, Int count) noexcept;

        /// \brief Slice a range returning the first element and a subrange to
        ///        the remaining ones.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <RandomAccessRange TRange>
        [[nodiscard]] constexpr auto
        SliceFront(Immutable<TRange> range) noexcept;

        /// \brief Slice a range returning the last element and a subrange to
        ///        the remaining ones.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <RandomAccessRange TRange>
        [[nodiscard]] constexpr auto
        SliceBack(Immutable<TRange> range) noexcept;

        /// \brief Slice a range returning a subrange to the first count
        ///        elements and another subrange to the remaining ones.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <RandomAccessRange TRange>
        [[nodiscard]] constexpr auto
        SliceFront(Immutable<TRange> range, Int count) noexcept;

        /// \brief Slice a range returning a subrange to the first count
        ///        elements and another subrange to the remaining ones.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <RandomAccessRange TRange>
        [[nodiscard]] constexpr auto
        SliceBack(Immutable<TRange> range, Int count) noexcept;

    }

}

// ===========================================================================

#include "details/random_access_range.inl"

// ===========================================================================
