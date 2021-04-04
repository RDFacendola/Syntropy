
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
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        if(Ranges::Count(lhs) != Ranges::Count(rhs))
        {
            return false;
        }

        if(Ranges::IsEmpty(lhs))
        {
            return true;
        }

        return (Ranges::Data(lhs) == Ranges::Data(rhs));
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
