
/// \file contiguous_range.inl
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // ContiguousRange.
    // ================

    template <ContiguousRange TRange, ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs, ContiguousRangeTag)
    noexcept
    {
        if(Ranges::Count(lhs) != Ranges::Count(rhs))
        {
            return false;
        }

        if(Ranges::IsEmpty(lhs) || (Ranges::Data(lhs) == Ranges::Data(rhs)))
        {
            return true;
        }

        return Ranges::AreEqual(lhs, rhs, ForwardRangeTag{});
    }

    template <ContiguousRange TRange, ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRange> lhs,
                  Immutable<URange> rhs,
                  ContiguousRangeTag) noexcept
    {
        if(Ranges::Count(lhs) != Ranges::Count(rhs))
        {
            return false;
        }

        if(Ranges::IsEmpty(lhs) || (Ranges::Data(lhs) == Ranges::Data(rhs)))
        {
            return true;
        }

        return Ranges::AreEquivalent(lhs, rhs, ForwardRangeTag{});
    }

    template <ContiguousRange TRange, ContiguousRange URange>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRange> lhs, Immutable<URange> rhs, ContiguousRangeTag)
    noexcept
    {
        if(Ranges::Count(lhs) == Ranges::Count(rhs))
        {
            auto is_empty = Ranges::IsEmpty(lhs);
            auto same_data = (Ranges::Data(lhs) == Ranges::Data(rhs));

            if(is_empty || same_data)
            {
                return Ordering::kEquivalent;
            }
        }

        return Ranges::Compare(lhs, rhs, ForwardRangeTag{});
    }

    template <ContiguousRange TRange, ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    Intersect(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto left_empty = Ranges::IsEmpty(lhs);
        auto right_empty = Ranges::IsEmpty(rhs);

        if(left_empty || right_empty)
        {
            return left_empty && right_empty;
        }

        if(auto rhs_end = Ranges::Data(rhs) + Ranges::Count(rhs);
           Ranges::Data(lhs) > rhs_end)
        {
            return false;
        }

        if(auto lhs_end = Ranges::Data(lhs) + Ranges::Count(rhs);
           Ranges::Data(rhs) > lhs_end)
        {
            return false;
        }

        return true;
    }

}

// ===========================================================================
