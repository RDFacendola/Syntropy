
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

#include "syntropy/core/concepts/range.h"
#include "syntropy/core/concepts/forward_range.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* SIZED RANGE INTERFACE                                                */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose elements can be visited sequentially and whose size can be computed in constant time.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept SizedRangeInterface = requires()
        {
            /// \brief Trait used to determine the reference type of an element inside the range.
            typename Templates::ElementReferenceTypeTraits<TRange>::Type;

            /// \brief Trait used to determine the type of the cardinality of the range.
            typename Templates::ElementCountTypeTraits<TRange>::Type;
        }
        && RangeCardinality<Templates::ElementCount<TRange>>
        && requires(Immutable<TRange> range)
        {
            /// \brief Access the first element in the range.
            { Front(range) } -> Syntropy::Concepts::SameAs<Templates::ElementReference<TRange>>;

            /// \brief Discard the first element in the range.
            { PopFront(range) } -> Syntropy::Concepts::ConvertibleTo<TRange>;

            /// \brief Get the number of elements in the range.
            { Count(range) } -> Syntropy::Concepts::SameAs<Templates::ElementCount<TRange>>;
        };

    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    /// \brief Range whose elements can be visited sequentially and whose size can be computed in constant time.
    /// \author Raffaele D. Facendola - January 2021.
    template <typename TRange>
    concept SizedRange = SizedRangeInterface<TRange> && ForwardRange<TRange>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Sized range interface.
    // ======================

    /// \brief Check whether a range is empty.
    template <Concepts::SizedRangeInterface TRange>
    [[nodiscard]] constexpr Bool IsEmpty(Immutable<TRange> range) noexcept;

    // Sized range.
    // ==============

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Compare two ranges lexicographically.
    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Ordering Compare(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Sized range interface.

    template <Concepts::SizedRangeInterface TRange>
    [[nodiscard]] constexpr Bool IsEmpty(Immutable<TRange> range) noexcept
    {
        return Count(range) == Templates::ElementCount<TRange>{ 0 };
    }

    // Sized range.

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        // Some ranges may provide a more efficient implementation than testing for equality.

        return AreEquivalent(lhs, rhs);
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