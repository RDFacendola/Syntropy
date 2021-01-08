
/// \file contiguous_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges whose elements are allocated contiguously.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/concepts/range.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Range whose elements are allocated contiguously.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ContiguousRange = Range<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Get the number of elements in the range.
            { Count(range) } -> SameAs<Templates::RangeElementCount<TRange>>;

            /// \brief Access the memory the elements are contiguously allocated on.
            { Data(range) } -> SameAs<Templates::RangeElementPointer<TRange>>;
        }
        && requires(Immutable<Templates::RangeElementPointer<TRange>> data, Immutable<Templates::RangeElementCount<TRange>> offset)
        {
            /// \brief Range elements should be accessible via a data pointer and a positive offset.
            { data + offset } -> SameAs<Templates::RangeElementPointer<TRange>>;

            /// \brief Range elements should be accessible via a data pointer and a negative offset.
            { data - offset } -> SameAs<Templates::RangeElementPointer<TRange>>;
        };
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Contiguous range.
    // =================

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::ContiguousRange TRange, Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Access a sub-range given an offset from the first element and the number of elements.
    /// \brief Exceeding range boundaries results in undefined behavior.
    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] constexpr decltype(auto) Select(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> offset, Immutable<Templates::RangeElementCount<TRange>> count) noexcept;

    /// \brief Get an iterator past the last element in a contiguous range.
    /// \brief Exceeding range boundaries results in undefined behavior.
    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] constexpr decltype(auto) Select(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> index) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Contiguous range.

    template <Concepts::ContiguousRange TRange, Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        // Note that empty ranges are equal to every other empty range.

        return (Count(lhs) == Count(rhs)) && (IsEmpty(lhs) || (Data(lhs) == Data(rhs)));
    }

    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] constexpr decltype(auto) Select(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> offset, Immutable<Templates::RangeElementCount<TRange>> count) noexcept
    {
        return TRange{ Data(range) + offset, count };
    }

    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] constexpr decltype(auto) Select(Immutable<TRange> range, Immutable<Templates::RangeElementCount<TRange>> index) noexcept
    {
        return *(Data(range) + index);
    }

}

// ===========================================================================