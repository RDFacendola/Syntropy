
/// \file sized_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges whose elements can be visited sequentially and whose size can be computed in constant time.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/core/foundation/tuple.h"

#include "syntropy/core/concepts/forward_range.h"

#include "syntropy/core/concepts/details/sized_range_details.h"

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Range's count type.
    template <typename TRange>
    using RangeCountType = Syntropy::Templates::RemoveConstReference<decltype(Details::RouteCount(Syntropy::Templates::Declval<TRange>()))>;

}

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    /// \brief Range whose elements can be visited sequentially and whose size can be computed in constant time.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept SizedRange = ForwardRange<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Get range's elements count.
            { Details::RouteCount(range) };
        };

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Sized range.
    // ============

    /// \brief Get range's elements count.
    template <Concepts::SizedRange TRange>
    [[nodiscard]] constexpr Templates::RangeCountType<TRange> Count(Immutable<TRange> range) noexcept;

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

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* SIZED RANGE EXTENSIONS                                               */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Get range's elements count.
    template <typename TType>
    struct Count;
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
    [[nodiscard]] constexpr Templates::RangeCountType<TRange> Count(Immutable<TRange> range) noexcept
    {
        return Details::RouteCount(range);
    }

    // Sized range.

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        // Some ranges may provide a more efficient implementation than testing for equality.

        return Ranges::AreEquivalent(lhs, rhs);
    }

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        if (Ranges::Count(lhs) == Ranges::Count(rhs))
        {
            auto lhs_copy = lhs;
            auto rhs_copy = rhs;

            for (; (!Ranges::IsEmpty(lhs_copy)) && (!Ranges::IsEmpty(rhs_copy)) && (Ranges::Front(lhs_copy) == Ranges::Front(rhs_copy)); )
            {
                lhs_copy = Ranges::PopFront(lhs_copy);
                rhs_copy = Ranges::PopFront(rhs_copy);
            }

            return Ranges::IsEmpty(lhs_copy);
        }

        return false;
    }

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Ordering Compare(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto lhs_copy = lhs;
        auto rhs_copy = rhs;

        for (; !Ranges::IsEmpty(lhs_copy) && !Ranges::IsEmpty(rhs_copy); )
        {
            auto compare = (Ranges::Front(lhs_copy) <=> Ranges::Front(rhs_copy));

            if (compare == Ordering::kLess)
            {
                return Ordering::kLess;
            }

            if (compare == Ordering::kGreater)
            {
                return Ordering::kGreater;
            }

            lhs_copy = Ranges::PopFront(lhs_copy);
            rhs_copy = Ranges::PopFront(rhs_copy);
        }

        if (Ranges::IsEmpty(lhs_copy) && Ranges::IsEmpty(rhs_copy))
        {
            return Ordering::kEquivalent;
        }

        return Ranges::IsEmpty(lhs_copy) ? Ordering::kLess : Ordering::kGreater;
    }
}

// ===========================================================================