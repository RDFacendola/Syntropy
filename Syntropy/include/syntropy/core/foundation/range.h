
/// \file ranges.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges and basic ranges functionalities.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Enable a type TRange to be used as a range.
    template <typename TRange>
    struct RangeEnableTypeTraits;

    /// \brief Exposes a member type Type equal to a reference to an element in a range TRange.
    template <typename TRange>
    struct RangeElementReferenceTypeTraits;

    /// \brief Exposes a member type Type equal to a pointer to an element in a range TRange.
    template <typename TRange>
    struct RangeElementPointerTypeTraits;

    /// \brief Exposes a member type Type equal to the type of the number of elements in a range TRange.
    template <typename TRange>
    struct RangeElementCountTypeTraits;

    /// \brief Type of a reference to an element in a range TRange.
    template <typename TRange>
    using RangeElementReference = typename RangeElementReferenceTypeTraits<TRange>::Type;

    /// \brief Type of a pointer to an element in a range TRange.
    template <typename TRange>
    using RangeElementPointer = typename RangeElementPointerTypeTraits<TRange>::Type;

    /// \brief Type of a number of elements in a range TRange.
    template <typename TRange>
    using RangeElementCount = typename RangeElementCountTypeTraits<TRange>::Type;
}

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    /// \brief Models a view on a range.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept Range = requires(Immutable<TRange> range)
    {
        typename Templates::RangeEnableTypeTraits<TRange>;
    };

    /************************************************************************/
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    /// \brief Models a view on a range that can be visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ForwardRange = true
        && requires(Immutable<TRange> range)
        {
            /// \brief Type of a reference to an element in a range TRange.
            typename Templates::RangeElementReference<TRange>;

            /// \brief TRange shall provide access to its front element.
            { Front(range) } -> SameAs<Templates::RangeElementReference<TRange>>;

            /// \brief TRange shall provide a way to consume it from the front.
            { PopFront(range) } -> ConvertibleTo<TRange>;

            /// \brief TRange shall provide a way to determine if it still has elements.
            { IsEmpty(range) } -> Boolean;
        };

    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    /// \brief Models a range whose size can be computed in constant time.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept SizedRange = ForwardRange<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Type of the number of elements in a range TRange.
            typename Templates::RangeElementCount<TRange>;

            /// \brief TRange shall provide a way to know its current size.
            { Count(range) } -> SameAs<Templates::RangeElementCount<TRange>>;

            /// \brief TRange shall provide a way to compare its size to other ranges'.
            { Count(range) } -> TotallyOrdered;
        };

    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    /// \brief Models a view on a range that can be visited in both directions.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BidirectionalRange = ForwardRange<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief TRange shall provide access to its rear element.
            { Back(range) } -> SameAs<Templates::RangeElementReference<TRange>>;

            /// \brief TRange shall provide a way to consume it from the back.
            { PopBack(range) } -> ConvertibleTo<TRange>;
        };

    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Models a view on a range that can be visited in any (random) order.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept RandomAccessRange = BidirectionalRange<TRange> && SizedRange<TRange>
        && requires(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> offset, Immutable<Templates::RangeElementCount<TRange>> count)
        {
            /// \brief TRange shall provide a way to obtain any sub-range using an offset and its size.
            { Select(range, offset, count) } -> ConvertibleTo<TRange>;
        }
        && requires(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> index)
        {
            /// \brief TRange shall provide a way to access any element by index (aka offset from the start).
            { Select(range, index) } -> SameAs<Templates::RangeElementReference<TRange>>;
        }
        && requires(Immutable<Templates::RangeElementCount<TRange>> lhs, Immutable<Templates::RangeElementCount<TRange>> rhs)
        {
            /// \brief Range element count type shall be closed under addition.
            { lhs + rhs } -> SameAs<Templates::RangeElementCount<TRange>>;

            /// \brief Range element count type shall be closed under subtraction.
            { lhs - rhs } -> SameAs<Templates::RangeElementCount<TRange>>;
        }
        && Templates::IsConstructible<Templates::RangeElementCount<TRange>, Int>;

    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Models a view on a range whose elements are allocated contiguously.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ContiguousRange = RandomAccessRange<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Type of a number of elements in a range TRange.
            typename Templates::RangeElementPointer<TRange>;

            /// \brief TRange shall provide a way to access the memory elements are contiguously stored on.
            { Data(range) } -> SameAs<Templates::RangeElementPointer<TRange>>;

        }
        && requires(Immutable<Templates::RangeElementPointer<TRange>> data, Immutable<Templates::RangeElementCount<TRange>> offset)
        {
            /// \brief It shall be possible to access range elements via a data pointer and a positive offset.
            { data + offset } -> SameAs<Templates::RangeElementPointer<TRange>>;

            /// \brief It shall be possible to access range elements via a data pointer and a negative offset.
            { data - offset } -> SameAs<Templates::RangeElementPointer<TRange>>;
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
    template <Concepts::ForwardRange TRange, typename TFunction>
    constexpr void ForEach(Immutable<TRange> range, TFunction function) noexcept;

    // Sized range.
    // ============

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    constexpr Bool AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    // Random access range.
    // ====================

    /// \brief Obtain a subrange consisting of the first elements of a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    constexpr TRange Front(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Obtain a subrange consisting of the last elements of a range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    constexpr TRange Back(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Discard the first elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    constexpr TRange PopFront(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Discard the last elements in a range and return the resulting subrange.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    constexpr TRange PopBack(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Slice a range returning the first element and a subrange to the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    constexpr auto SliceFront(Immutable<TRange> range) noexcept;

    /// \brief Slice a range returning the last element and a subrange to the remaining ones.
    /// \remarks Calling this method with an empty range results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    constexpr auto SliceBack(Immutable<TRange> range) noexcept;

    /// \brief Slice a range returning a subrange to the first count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    constexpr Tuple<TRange, TRange> SliceFront(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Slice a range returning a subrange to the last count elements and another subrange to the remaining ones.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    constexpr Tuple<TRange, TRange> SliceBack(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    // Contiguous range.
    // =================

    /// \brief Get an iterator to the first element in a range.
    template <Concepts::ContiguousRange TRange>
    constexpr auto* begin(Immutable<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a range.
    template <Concepts::ContiguousRange TRange>
    constexpr auto* end(Immutable<TRange> range) noexcept;

    /// \brief Get an iterator to the first element in a contiguous range.
    template <Concepts::ContiguousRange TRange>
    constexpr auto* Begin(Immutable<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a contiguous range.
    template <Concepts::ContiguousRange TRange>
    constexpr auto* End(Immutable<TRange> range) noexcept;

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::ContiguousRange TRange, Concepts::ContiguousRange URange>
    constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

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

    template <Concepts::ForwardRange TRange, typename TFunction>
    constexpr void ForEach(Immutable<TRange> range, TFunction function) noexcept
    {
        for (auto rest = range; !IsEmpty(rest); rest = PopFront(rest))
        {
            function(Front(rest));
        }
    }

    // Sized range.

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        // In some implementations comparing two instances for identity is much faster than comparing them for equality.
        // This function is left as a customization point for those implementations.

        return AreEquivalent(lhs, rhs);
    }

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    constexpr Bool AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        if (Count(lhs) == Count(rhs))
        {
            auto lhs_copy = lhs;
            auto rhs_copy = rhs;

            for (; (!IsEmpty(lhs_copy)) && (!IsEmpty(rhs_copy)) && (Front(lhs_copy) == Front(rhs_copy)); )
            {
                lhs_copy = PopFront(lhs_copy);
                rhs_copy = PopFront(rhs_copy);
            }

            return IsEmpty(lhs_copy);
        }

        return false;
    }

    // Random access range.

    template <Concepts::RandomAccessRange TRange>
    constexpr TRange Front(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return Select(range, Templates::RangeElementCount<TRange>{ 0 }, count);
    }

    template <Concepts::RandomAccessRange TRange>
    constexpr TRange Back(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return Select(range, Count(range) - count, count);
    }

    template <Concepts::RandomAccessRange TRange>
    constexpr TRange PopFront(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return Select(range, count, Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    constexpr TRange PopBack(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return Select(range, Templates::RangeElementCount<TRange>{ 0 }, Count(range) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    constexpr auto SliceFront(Immutable<TRange> range) noexcept
    {
        return MakeTuple(Front(range), PopFront(range));
    }

    template <Concepts::RandomAccessRange TRange>
    constexpr auto SliceBack(Immutable<TRange> range) noexcept
    {
        return MakeTuple(Back(range), PopBack(range));
    }

    template <Concepts::RandomAccessRange TRange>
    constexpr Tuple<TRange, TRange> SliceFront(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return { Front(range, count), PopFront(range, count) };
    }

    template <Concepts::RandomAccessRange TRange>
    constexpr Tuple<TRange, TRange> SliceBack(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return { Back(range, count), PopBack(range, count) };
    }

    // Contiguous range.

    template <Concepts::ContiguousRange TRange>
    constexpr auto* begin(Immutable<TRange> range) noexcept
    {
        return Data(range);
    }

    template <Concepts::ContiguousRange TRange>
    constexpr auto* end(Immutable<TRange> range) noexcept
    {
        return Data(range) + Count(range);
    }

    template <Concepts::ContiguousRange TRange>
    constexpr auto* Begin(Immutable<TRange> range) noexcept
    {
        return Data(range);
    }

    template <Concepts::ContiguousRange TRange>
    constexpr auto* End(Immutable<TRange> range) noexcept
    {
        return Data(range) + Count(range);
    }

    template <Concepts::ContiguousRange TRange, Concepts::ContiguousRange URange>
    constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        // Note that empty ranges are equal to each other empty range.

        return (Count(lhs) == Count(rhs)) && (IsEmpty(lhs) || (Data(lhs) == Data(rhs)));
    }

}

// ===========================================================================