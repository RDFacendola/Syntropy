
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

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    /// \brief Models a view on a range that can be visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ForwardRangeT = requires(TRange& range)
    {
        /// \brief Access the first element in a range.
        /// \remarks Accessing the first element of an empty range results in undefined behavior.
        Front(range);

        /// \brief Discard the first count elements in a range and return the resulting subrange.
        /// \remarks If this method would cause the subrange to exceed the original range, the behavior of this method is undefined.
        { PopFront(range) } -> Concepts::ConvertibleTo<TRange>;

        /// \brief Check whether a range is empty.
        /// \return Returns true if the range is empty, returns false otherwise.
        { IsEmpty(range) } -> Concepts::SameAs<Bool>;
    };

    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    /// \brief Models a range whose size can be computed efficiently in constant time.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept SizedRangeT = ForwardRangeT<TRange> && requires(TRange & range)
    {
        /// \brief Get the number of elements in the range.
        { Count(range) } -> Concepts::SameAs<Int>;
    };

    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    /// \brief Models a view on a range that can be visited in both directions.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BidirectionalRangeT = ForwardRangeT<TRange> && requires(TRange & range)
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
    concept RandomAccessRangeT = BidirectionalRangeT<TRange> && requires(TRange& range, Int offset, Int count)
    {
        /// \brief Obtain a sub-range given an offset and a number of elements.
        /// \remarks Exceeding range boundaries results in undefined behavior.
        { Select(range, offset, count) } -> Concepts::ConvertibleTo<TRange>;
    };

    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Models a view on a range whose elements are allocated contiguously.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ContiguousRangeT = RandomAccessRangeT<TRange> && requires(TRange & range)
    {
        /// \brief Access raw range data.
        Data(range);
    };

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Apply a function to each element in the range.
    template <Concepts::ForwardRangeT TRange, typename TFunction>
    constexpr void ForEach(const TRange& range, TFunction function) noexcept;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <Concepts::ForwardRangeT TRange, typename TFunction>
    constexpr void ForEach(const TRange& range, TFunction function) noexcept
    {
        for (auto rest = range; !IsEmpty(rest); rest = PopFront(rest))
        {
            function(Front(rest));
        }
    }

}

// ===========================================================================