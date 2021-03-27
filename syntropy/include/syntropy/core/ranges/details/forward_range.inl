
/// \file forward_range.inl
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/core/algorithms/swap.h"

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
        for (auto range_view = Ranges::ViewOf(range);
             !Ranges::IsEmpty(range_view);
             range_view = Ranges::PopFront(range_view))
        {
            function(Ranges::Front(range_view));
        }
    }

    template <ForwardRange TRange, ForwardRange URange>
    constexpr RangeCardinalityTypeOf<URange>
    Copy(Immutable<TRange> destination, Immutable<URange> source) noexcept
    {
        auto source_view = Ranges::ViewOf(source);
        auto destination_view = Ranges::ViewOf(destination);
        auto count = RangeCardinalityTypeOf<URange>{ 0 };

        for (; !Ranges::IsEmpty(source_view) &&
               !Ranges::IsEmpty(destination_view);
               ++count)
        {
            Ranges::Front(destination_view) = Ranges::Front(source_view);

            source_view = Ranges::PopFront(source_view);
            destination_view = Ranges::PopFront(destination_view);
        }

        return count;
    }

    template <ForwardRange TRange, ForwardRange URange>
    constexpr RangeCardinalityTypeOf<URange>
    Move(Immutable<TRange> destination, Immutable<URange> source) noexcept
    {
        auto source_view = Ranges::ViewOf(source);
        auto destination_view = Ranges::ViewOf(destination);
        auto count = RangeCardinalityTypeOf<URange>{ 0 };

        for (; !Ranges::IsEmpty(source_view) &&
               !Ranges::IsEmpty(destination_view);
               ++count)
        {
            Ranges::Front(destination_view)
                = Syntropy::Move(Ranges::Front(source_view));

            source_view = Ranges::PopFront(source_view);
            destination_view = Ranges::PopFront(destination_view);
        }

        return count;
    }

    template <ForwardRange TRange>
    constexpr RangeCardinalityTypeOf<TRange>
    Swap(Immutable<TRange> lhs, Immutable<TRange> rhs) noexcept
    {
        auto lhs_view = Ranges::ViewOf(lhs);
        auto rhs_view = Ranges::ViewOf(rhs);
        auto count = RangeCardinalityTypeOf<TRange>{ 0 };

        for (; !Ranges::IsEmpty(lhs_view) &&
               !Ranges::IsEmpty(rhs_view);
               ++count)
        {
            Algorithms::Swap(Ranges::Front(lhs_view),
                            Ranges::Front(rhs_view));

            lhs_view = Ranges::PopFront(lhs_view);
            rhs_view = Ranges::PopFront(rhs_view);
        }

        return count;
    }

}

// ===========================================================================
