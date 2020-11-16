
/// \file ranges.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges and basic ranges functionalities.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/support.h"
#include "syntropy/language/concepts/concepts.h"

#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    /// \brief Models a view on a range that can be visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ForwardRangeT = requires(const TRange& range)
    {
        /// \brief Access the first element in a range.
        /// \remarks Accessing the first element of an empty range results in undefined behavior.
        Front(range);

        /// \brief Discard the first count elements in a range and return the resulting subrange.
        /// \remarks If this method would cause the subrange to exceed the original range, the behavior of this method is undefined.
        { PopFront(range) } -> Concepts::ConvertibleTo<TRange>;

        /// \brief Check whether a range is empty.
        /// \return Returns true if the range is empty, returns false otherwise.
        { IsEmpty(range) } -> Concepts::Boolean;
    };

    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    /// \brief Models a view on a range that can be visited in both directions.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BidirectionalRangeT = ForwardRangeT<TRange>
        && requires(TRange & range)
        {
            /// \brief Access the last element in a range.
            /// \remarks Accessing the last element of an empty range results in undefined behavior.
            Back(range);

            /// \brief Discard the last count elements in a range and return the resulting subrange.
            /// \remarks If this method would cause the subrange to exceed the original range, the behavior of this method is undefined.
            { PopBack(range) } -> Concepts::ConvertibleTo<TRange>;
        };

    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Models a view on a range that can be visited in any (random) order.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept RandomAccessRangeT = BidirectionalRangeT<TRange>
        && requires(TRange& range)
        {
            /// \brief Get the number of elements in the range.
            { Count(range) } -> Concepts::Integral;
        }
        && requires(TRange& range, Int offset, Int count)
        {
            /// \brief Obtain a sub-range given an offset and a number of elements.
            /// \remarks Exceeding range boundaries results in undefined behavior.
            { Select(range, offset, count) } -> Concepts::ConvertibleTo<TRange>;
        }
        && requires(TRange& range, Int index)
        {
            /// \brief Access a range element by index.
            /// \remarks Exceeding range boundaries results in undefined behavior.
            { Select(range, index) };
        };

    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Models a view on a range whose elements are allocated contiguously.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ContiguousRangeT = RandomAccessRangeT<TRange>
        && requires(TRange & range)
        {
            /// \brief Access contiguous range data.
            /// \remarks If the range is empty the returned value is unspecified.
            Data(range);
        };

    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    /// \brief Models a range whose size can be computed in constant time.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept SizeRangeT = ForwardRangeT<TRange>
        && requires(TRange & range)
        {
            /// \brief Get the number of elements in the range.
            { Count(range) } -> Concepts::Integral;
        };

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Forward range.
    // ==============

    /// \brief Apply a function to each element in the range.
    template <Concepts::ForwardRangeT TRange, typename TFunction>
    constexpr void ForEach(const TRange& range, TFunction function) noexcept;

    // Random access range.
    // ====================

    /// \brief Obtain a subrange consisting of the first elements of a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeT TRange>
    constexpr TRange Front(const TRange& range, Int count) noexcept;

    /// \brief Obtain a subrange consisting of the last elements of a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeT TRange>
    constexpr TRange Back(const TRange& range, Int count) noexcept;

    /// \brief Discard the first elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeT TRange>
    constexpr TRange PopFront(const TRange& range, Int count) noexcept;

    /// \brief Discard the last elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeT TRange>
    constexpr TRange PopBack(const TRange& range, Int count) noexcept;

    /// \brief Slice a range returning the first element and a subrange to the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined behavior.
    template <Concepts::RandomAccessRangeT TRange>
    constexpr auto SliceFront(const TRange& range) noexcept;

    /// \brief Slice a range returning the last element and a subrange to the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined behavior.
    template <Concepts::RandomAccessRangeT TRange>
    constexpr auto SliceBack(const TRange& range) noexcept;

    /// \brief Slice a range returning a subrange to the first count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeT TRange>
    constexpr TupleT<TRange, TRange> SliceFront(const TRange& range, Int count) noexcept;

    /// \brief Slice a range returning a subrange to the last count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeT TRange>
    constexpr TupleT<TRange, TRange> SliceBack(const TRange& range, Int count) noexcept;

    // Contiguous range.
    // =================

    /// \brief Get an iterator to the first element in a range.
    template <Concepts::ContiguousRangeT TRange>
    constexpr auto* begin(const TRange& span) noexcept;

    /// \brief Get an iterator past the last element in a range.
    template <Concepts::ContiguousRangeT TRange>
    constexpr auto* end(const TRange& span) noexcept;

    /// \brief Get an iterator to the first element in a contiguous range.
    template <Concepts::ContiguousRangeT TRange>
    constexpr auto* Begin(const TRange& span) noexcept;

    /// \brief Get an iterator past the last element in a contiguous range.
    template <Concepts::ContiguousRangeT TRange>
    constexpr auto* End(const TRange& span) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Forward range.

    template <Concepts::ForwardRangeT TRange, typename TFunction>
    constexpr void ForEach(const TRange& range, TFunction function) noexcept
    {
        for (auto rest = range; !IsEmpty(rest); rest = PopFront(rest))
        {
            function(Front(rest));
        }
    }

    // Random access range.

    template <Concepts::RandomAccessRangeT TRange>
    constexpr TRange Front(const TRange& range, Int count) noexcept
    {
        return Select(range, 0, count);
    }

    template <Concepts::RandomAccessRangeT TRange>
    constexpr TRange Back(const TRange& range, Int count) noexcept
    {
        return Select(range, Count(range) - count, count);
    }

    template <Concepts::RandomAccessRangeT TRange>
    constexpr TRange PopFront(const TRange& range, Int count) noexcept
    {
        return Select(range, count, Count(range) - count);
    }

    template <Concepts::RandomAccessRangeT TRange>
    constexpr TRange PopBack(const TRange& range, Int count) noexcept
    {
        return Select(range, 0, Count(range) - count);
    }

    template <Concepts::RandomAccessRangeT TRange>
    constexpr auto SliceFront(const TRange& range) noexcept
    {
        return MakeTuple(Front(range), PopFront(range));
    }

    template <Concepts::RandomAccessRangeT TRange>
    constexpr auto SliceBack(const TRange& range) noexcept
    {
        return MakeTuple(Back(range), PopBack(range));
    }

    template <Concepts::RandomAccessRangeT TRange>
    constexpr TupleT<TRange, TRange> SliceFront(const TRange& range, Int count) noexcept
    {
        return { Front(range, count), PopFront(range, count) };
    }

    template <Concepts::RandomAccessRangeT TRange>
    constexpr TupleT<TRange, TRange> SliceBack(const TRange& range, Int count) noexcept
    {
        return { Back(range, count), PopBack(range, count) };
    }

    // Contiguous range.

    template <Concepts::ContiguousRangeT TRange>
    constexpr auto* begin(const TRange& range) noexcept
    {
        return Data(range);
    }

    template <Concepts::ContiguousRangeT TRange>
    constexpr auto* end(const TRange& range) noexcept
    {
        return Data(range) + Count(range);
    }

    template <Concepts::ContiguousRangeT TRange>
    constexpr auto* Begin(const TRange& range) noexcept
    {
        return Data(range);
    }

    template <Concepts::ContiguousRangeT TRange>
    constexpr auto* End(const TRange& range) noexcept
    {
        return Data(range) + Count(range);
    }

}

// ===========================================================================