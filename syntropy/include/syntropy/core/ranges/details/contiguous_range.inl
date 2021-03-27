
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
        if(PtrOf(lhs) == PtrOf(rhs))
        {
            return true;
        }

        auto lhs_view = Ranges::ViewOf(lhs);
        auto rhs_view = Ranges::ViewOf(rhs);

        if(Ranges::IsEmpty(lhs_view) && Ranges::IsEmpty(rhs_view))
        {
            return true;
        }

        if(Ranges::Count(lhs_view) != Ranges::Count(rhs_view))
        {
            return false;
        }

        if(Ranges::Data(lhs_view) == Ranges::Data(rhs_view))
        {
            return true;
        }

        return false;
    }

    template <ContiguousRange TRange, ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    Intersect(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto lhs_view = Ranges::ViewOf(lhs);
        auto rhs_view = Ranges::ViewOf(rhs);

        if(Ranges::IsEmpty(lhs_view) || Ranges::IsEmpty(rhs_view))
        {
            return true;    // Empty ranges intersects with everything.
        }

        if(auto rhs_end = Ranges::Data(rhs_view) + Ranges::Count(rhs_view);
           Ranges::Data(lhs_view) > rhs_end)
        {
            return false;
        }

        if(auto lhs_end = Ranges::Data(lhs_view) + Ranges::Count(lhs_view);
           Ranges::Data(rhs_view) > lhs_end)
        {
            return false;
        }

        return true;
    }

}

// ===========================================================================
