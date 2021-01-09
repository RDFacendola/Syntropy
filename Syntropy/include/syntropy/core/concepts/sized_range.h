
/// \file sized_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges that can be scanned sequentially and have a well-known size.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/support/compare.h"

#include "syntropy/core/concepts/range.h"
#include "syntropy/core/concepts/random_access_range.h"

#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    /// \brief Range that can be visited sequentially and whose size can be computed in constant time.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept SizedRange = Range<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Get the number of elements in the range.
            { Count(range) } -> SameAs<Templates::RangeElementCount<TRange>>;

            /// \brief Access the first element in the range.
            { Ranges::ADL::RequiresFront(range) } -> SameAs<Templates::RangeElementReference<TRange>>;

            /// \brief Discard the first element in the range.
            { Ranges::ADL::RequiresPopFront(range) } -> ConvertibleTo<TRange>;
        };
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Sized range.
    // ==============

    /// \brief Check whether a range is empty.
    template <Concepts::SizedRange TRange>
    [[nodiscard]] constexpr Bool IsEmpty(Immutable<TRange> range) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Compare two ranges lexicographically.
    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Ordering Compare(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges::ADL
{
    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    using Ranges::IsEmpty;

    /// \brief Detect a "IsEmpty" function using argument-dependent lookup.
    template <typename TRange>
    auto RequiresIsEmpty(Immutable<TRange> range) noexcept -> decltype(IsEmpty(range));
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Sized range.

    template <Concepts::SizedRange TRange>
    [[nodiscard]] constexpr Bool IsEmpty(Immutable<TRange> range) noexcept
    {
        return Count(range) == Templates::RangeElementCount<TRange>{ 0 };
    }

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
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

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Ordering Compare(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto lhs_copy = lhs;
        auto rhs_copy = rhs;

        for (; !IsEmpty(lhs_copy) && !IsEmpty(rhs_copy); )
        {
            auto compare = (Front(lhs_copy) <=> Front(rhs_copy));

            if (compare == Ordering::kLess)
            {
                return Ordering::kLess;
            }

            if (compare == Ordering::kGreater)
            {
                return Ordering::kGreater;
            }

            lhs_copy = PopFront(lhs_copy);
            rhs_copy = PopFront(rhs_copy);
        }

        if (IsEmpty(lhs_copy) && IsEmpty(rhs_copy))
        {
            return Ordering::kEquivalent;
        }

        return IsEmpty(lhs_copy) ? Ordering::kLess : Ordering::kGreater;
    }
}

// ===========================================================================