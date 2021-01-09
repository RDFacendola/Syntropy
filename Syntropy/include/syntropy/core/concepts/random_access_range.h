
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

#include "syntropy/core/concepts/range.h"
#include "syntropy/core/concepts/contiguous_range.h"

#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Range that can be visited in random order.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept RandomAccessRange = Range<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Get the number of elements in the range.
            { Count(range) } -> SameAs<Templates::RangeElementCount<TRange>>;
        }
        && requires(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> offset, Immutable<Templates::RangeElementCount<TRange>> count)
        {
            /// \brief Access a sub-range. Looks for a Select(range, offset, count) method using ADL.
            { Ranges::ADL::RequiresSelect(range, offset, count) } -> ConvertibleTo<TRange>;
        }
        && requires(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> index)
        {
            /// \brief Access an element by index. Looks for a Select(range, index) method using ADL.
            { Ranges::ADL::RequiresSelect(range, index) } -> SameAs<Templates::RangeElementReference<TRange>>;
        }
        && requires(Immutable<Templates::RangeElementCount<TRange>> lhs, Immutable<Templates::RangeElementCount<TRange>> rhs)
        {
            /// \brief Range element count type shall be closed under addition.
            { lhs + rhs } -> SameAs<Templates::RangeElementCount<TRange>>;

            /// \brief Range element count type shall be closed under subtraction.
            { lhs - rhs } -> SameAs<Templates::RangeElementCount<TRange>>;
        };
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Random access range.
    // ====================

    /// \brief Access the first element in a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr decltype(auto) Front(Immutable<TRange> range) noexcept;

    /// \brief Access the last element in a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr decltype(auto) Back(Immutable<TRange> range) noexcept;

    /// \brief Discard the first elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range) noexcept;

    /// \brief Discard the last elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range) noexcept;

    /// \brief Select the first elements in a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange Front(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Select the last elements in a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange Back(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Discard the first elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Discard the last elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

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
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceFront(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Slice a range returning a subrange to the last count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceBack(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges::ADL
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    using Ranges::Front;
    using Ranges::Back;
    using Ranges::PopFront;
    using Ranges::PopBack;

    /// \brief Detect a "Front" function using argument-dependent lookup.
    template <typename TRange>
    auto RequiresFront(Immutable<TRange> range) noexcept -> decltype(Front(range));

    /// \brief Detect a "Back" function using argument-dependent lookup.
    template <typename TRange>
    auto RequiresBack(Immutable<TRange> range) noexcept -> decltype(Back(range));

    /// \brief Detect a "PopFront" function using argument-dependent lookup.
    template <typename TRange>
    auto RequiresPopFront(Immutable<TRange> range) noexcept -> decltype(PopFront(range));

    /// \brief Detect a "PopBack" function using argument-dependent lookup.
    template <typename TRange>
    auto RequiresPopBack(Immutable<TRange> range) noexcept -> decltype(PopBack(range));
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

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr decltype(auto) Front(Immutable<TRange> range) noexcept
    {
        return Select(range, Templates::RangeElementCount<TRange>{ 0 });
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr decltype(auto) Back(Immutable<TRange> range) noexcept
    {
        return Select(range, Count(range) - Templates::RangeElementCount<TRange>{ 1 });
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range) noexcept
    {
        return PopFront(range, Templates::RangeElementCount<TRange>{ 1 });
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range) noexcept
    {
        return PopBack(range, Templates::RangeElementCount<TRange>{ 1 });
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange Front(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return Select(range, Templates::RangeElementCount<TRange>{ 0 }, count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange Back(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return Select(range, Count(range) - count, count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopFront(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return Select(range, count, Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return Select(range, Templates::RangeElementCount<TRange>{ 0 }, Count(range) - count);
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
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceFront(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return { Front(range, count), PopFront(range, count) };
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, TRange> SliceBack(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return { Back(range, count), PopBack(range, count) };
    }

}

// ===========================================================================