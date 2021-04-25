
/// \file forward_range.inl
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/core/toolset/swap.h"
#include "syntropy/core/comparisons/compare.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // ForwardRange.
    // =============

    template <ForwardRange TRange, typename TFunction>
    constexpr void
    ForEach(Immutable<TRange> range, TFunction function) noexcept
    {
        auto right = Ranges::ViewOf(range);

        for (;!Ranges::IsEmpty(right);)
        {
            function(Ranges::Front(right));

            right = Ranges::PopFront(right);
        }
    }

    // Copy \ Move.
    // ============

    template <ForwardRange TRange, ForwardRange URange>
    constexpr Int
    PartialCopy(Immutable<TRange> destination,
                Immutable<URange> source,
                ForwardRangeTag) noexcept
    {
        auto left = Ranges::ViewOf(destination);
        auto right = Ranges::ViewOf(source);
        auto count = ToInt(0);

        for (; !Ranges::IsEmpty(right) && !Ranges::IsEmpty(left); ++count)
        {
            Ranges::Front(left) = Ranges::Front(right);

            left = Ranges::PopFront(left);
            right = Ranges::PopFront(right);
        }

        return count;
    }

    template <ForwardRange TRange, ForwardRange URange>
    constexpr Int
    PartialMove(Immutable<TRange> destination,
                Immutable<URange> source,
                ForwardRangeTag) noexcept
    {
        auto left = Ranges::ViewOf(destination);
        auto right = Ranges::ViewOf(source);
        auto count = ToInt(0);

        for (; !Ranges::IsEmpty(right) && !Ranges::IsEmpty(left); ++count)
        {
            Ranges::Front(left) = Move(Ranges::Front(right));

            left = Ranges::PopFront(left);
            right = Ranges::PopFront(right);
        }

        return count;
    }

    // Swap.
    // =====

    template <ForwardRange TRange>
    constexpr Int
    PartialSwap(Immutable<TRange> lhs,
                Immutable<TRange> rhs,
                ForwardRangeTag) noexcept
    {
        auto left = Ranges::ViewOf(lhs);
        auto right = Ranges::ViewOf(rhs);
        auto count = ToInt(0);

        for (; !Ranges::IsEmpty(left) && !Ranges::IsEmpty(right); ++count)
        {
            Toolset::Swap(Ranges::Front(left), Ranges::Front(right));

            left = Ranges::PopFront(left);
            right = Ranges::PopFront(right);
        }

        return count;
    }

    // Comparison.
    // ===========

    template <ForwardRange TRange, ForwardRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs, ForwardRangeTag)
    noexcept
    {
        auto left = Ranges::ViewOf(lhs);
        auto right = Ranges::ViewOf(rhs);

        for(; !Ranges::IsEmpty(left) && !Ranges::IsEmpty(right);)
        {
            if(!Comparisons::AreEqual(Ranges::Front(left),
                                     Ranges::Front(right)))
            {
                return false;
            }

            left = Ranges::PopFront(left);
            right = Ranges::PopFront(right);
        }

        return Ranges::IsEmpty(left) && Ranges::IsEmpty(right);
    }

    template <ForwardRange TRange, ForwardRange URange>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRange> lhs,
                  Immutable<URange> rhs,
                  ForwardRangeTag) noexcept
    {
        auto left = Ranges::ViewOf(lhs);
        auto right = Ranges::ViewOf(rhs);

        for(; !Ranges::IsEmpty(left) && !Ranges::IsEmpty(right);)
        {
            if(!Comparisons::AreEquivalent(Ranges::Front(left),
                                          Ranges::Front(right)))
            {
                return false;
            }

            left = Ranges::PopFront(left);
            right = Ranges::PopFront(right);
        }

        return Ranges::IsEmpty(left) && Ranges::IsEmpty(right);
    }

    template <ForwardRange TRange, ForwardRange URange>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRange> lhs, Immutable<URange> rhs, ForwardRangeTag)
    noexcept
    {
        auto left = Ranges::ViewOf(lhs);
        auto right = Ranges::ViewOf(rhs);

        // Compare until either is exhausted.

        for(; !Ranges::IsEmpty(left) && !Ranges::IsEmpty(right);)
        {
            auto compare = Comparisons::Compare(Ranges::Front(left),
                                               Ranges::Front(right));

            if(compare != Ordering::kEquivalent)
            {
                return compare;
            }

            left = Ranges::PopFront(left);
            right = Ranges::PopFront(right);
        }

        auto left_empty = Ranges::IsEmpty(left);
        auto right_empty = Ranges::IsEmpty(right);

        if(left_empty && !right_empty)
        {
            return Ordering::kLess;
        }

        if(right_empty && !left_empty)
        {
            return Ordering::kGreater;
        }

        return Ordering::kEquivalent;
    }

}

// ===========================================================================
