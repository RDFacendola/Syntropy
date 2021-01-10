
/// \file random_access_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges that can be accessed in random order.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/concepts/bidirectional_range.h"

#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE INTERFACE                                        */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose element can be visited in any order.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept RandomAccessRangeInterface = requires()
        {
            /// \brief Trait used to determine the reference type of an element inside the range.
            typename Templates::ElementReferenceTypeTraits<TRange>::Type;

            /// \brief Trait used to determine the type of the cardinality of the range.
            typename Templates::ElementCountTypeTraits<TRange>::Type;
        }
        && RangeCardinality<Templates::ElementCount<TRange>>
        && requires(Immutable<TRange> range)
        {
            /// \brief Get the number of elements in the range.
            { Count(range) } -> Syntropy::Concepts::SameAs<Templates::ElementCount<TRange>>;
        }
        && requires(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> offset, Immutable<Templates::ElementCount<TRange>> count)
        {
            /// \brief Access a sub-range.
            { Select(range, offset, count) } -> Syntropy::Concepts::ConvertibleTo<TRange>;
        }
        && requires(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> index)
        {
            /// \brief Access an element by index.
            { Select(range, index) } -> Syntropy::Concepts::SameAs<Templates::ElementReference<TRange>>;
        };


    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose element can be visited in any order.
    /// \author Raffaele D. Facendola - January 2021.
    template <typename TRange>
    concept RandomAccessRange = RandomAccessRangeInterface<TRange> && BidirectionalRange<TRange> && SizedRange<TRange>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Random access range interface.
    // ==============================

    /// \brief Access the first element in a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeInterface TRange>
    [[nodiscard]] constexpr decltype(auto) Front(Immutable<TRange> range) noexcept;

    /// \brief Access the last element in a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeInterface TRange>
    [[nodiscard]] constexpr decltype(auto) Back(Immutable<TRange> range) noexcept;

    /// \brief Discard the first elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeInterface TRange>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range) noexcept;

    /// \brief Discard the last elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeInterface TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range) noexcept;

    // Random access range.
    // ====================

    /// \brief Select the first elements in a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange Front(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept;

    /// \brief Select the last elements in a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange Back(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept;

    /// \brief Discard the first elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept;

    /// \brief Discard the last elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept;

    /// \brief Slice a range returning the first element and a subrange to the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto SliceFront(Immutable<TRange> range) noexcept;

    /// \brief Slice a range returning the last element and a subrange to the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto SliceBack(Immutable<TRange> range) noexcept;

    /// \brief Slice a range returning a subrange to the first count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceFront(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept;

    /// \brief Slice a range returning a subrange to the last count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceBack(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Random access range interface.

    template <Concepts::RandomAccessRangeInterface TRange>
    [[nodiscard]] constexpr decltype(auto) Front(Immutable<TRange> range) noexcept
    {
        return Select(range, Templates::ElementCount<TRange>{ 0 });
    }

    template <Concepts::RandomAccessRangeInterface TRange>
    [[nodiscard]] constexpr decltype(auto) Back(Immutable<TRange> range) noexcept
    {
        return Select(range, Count(range) - Templates::ElementCount<TRange>{ 1 });
    }

    template <Concepts::RandomAccessRangeInterface TRange>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range) noexcept
    {
        return PopFront(range, Templates::ElementCount<TRange>{ 1 });
    }

    template <Concepts::RandomAccessRangeInterface TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range) noexcept
    {
        return PopBack(range, Templates::ElementCount<TRange>{ 1 });
    }

    // Random access range.

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange Front(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept
    {
        return Select(range, Templates::ElementCount<TRange>{ 0 }, count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange Back(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept
    {
        return Select(range, Count(range) - count, count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept
    {
        return Select(range, count, Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept
    {
        return Select(range, Templates::ElementCount<TRange>{ 0 }, Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto SliceFront(Immutable<TRange> range) noexcept
    {
        return MakeTuple(Front(range), PopFront(range));
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto SliceBack(Immutable<TRange> range) noexcept
    {
        return MakeTuple(Back(range), PopBack(range));
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceFront(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept
    {
        return { Front(range, count), PopFront(range, count) };
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceBack(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> count) noexcept
    {
        return { Back(range, count), PopBack(range, count) };
    }

}

// ===========================================================================