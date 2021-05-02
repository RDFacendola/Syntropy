
/// \file forward_range.inl
///
/// \author Raffaele D. Facendola - November 2020.
/// \author Raffaele D. Facendola - January 2021.

// ===========================================================================

#include "syntropy/language/support/swap.h"
#include "syntropy/core/comparisons/compare.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    template <ForwardRange TRange, typename TFunction>
    constexpr void
    Ranges
    ::ForEach(Immutable<TRange> range, TFunction function) noexcept
    {
        auto right = Ranges::ViewOf(range);

        for (;!Ranges::IsEmpty(right);)
        {
            function(Ranges::Front(right));

            right = Ranges::PopFront(right);
        }
    }

    template <ForwardRange TRange, ForwardRange URange>
    constexpr Int
    Ranges
    ::PartialCopy(Immutable<TRange> destination,
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
    Ranges
    ::PartialMove(Immutable<TRange> destination,
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

    template <ForwardRange TRange>
    constexpr Int
    Ranges
    ::PartialSwap(Immutable<TRange> lhs,
                Immutable<TRange> rhs,
                ForwardRangeTag) noexcept
    {
        auto left = Ranges::ViewOf(lhs);
        auto right = Ranges::ViewOf(rhs);
        auto count = ToInt(0);

        for (; !Ranges::IsEmpty(left) && !Ranges::IsEmpty(right); ++count)
        {
            Support::Swap(Ranges::Front(left), Ranges::Front(right));

            left = Ranges::PopFront(left);
            right = Ranges::PopFront(right);
        }

        return count;
    }

    template <ForwardRange TRange, ForwardRange URange>
    [[nodiscard]] constexpr Bool
    Ranges
    ::AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs, ForwardRangeTag)
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
    Ranges
    ::AreEquivalent(Immutable<TRange> lhs,
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
    Ranges
    ::Compare(Immutable<TRange> lhs, Immutable<URange> rhs, ForwardRangeTag)
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
