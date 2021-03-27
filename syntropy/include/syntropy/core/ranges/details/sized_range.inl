
/// \file sized_range.inl
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // SizedRange.
    // ===========

    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        return (PtrOf(lhs) == PtrOf(rhs));
    }

    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        if (Ranges::AreEqual(lhs, rhs))
        {
            return true;
        }

        auto lhs_view = Ranges::ViewOf(lhs);
        auto rhs_view = Ranges::ViewOf(rhs);

        if (Ranges::Count(lhs_view) == Ranges::Count(rhs_view))
        {
            for (; (!Ranges::IsEmpty(lhs_view)) &&
                   (!Ranges::IsEmpty(rhs_view)) &&
                   (Ranges::Front(lhs_view) == Ranges::Front(rhs_view)); )
            {
                lhs_view = Ranges::PopFront(lhs_view);
                rhs_view = Ranges::PopFront(rhs_view);
            }

            return Ranges::IsEmpty(lhs_view);
        }

        return false;
    }

    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto lhs_view = Ranges::ViewOf(lhs);
        auto rhs_view = Ranges::ViewOf(rhs);

        for (; !Ranges::IsEmpty(lhs_view) && !Ranges::IsEmpty(rhs_view); )
        {
            auto compare =
                (Ranges::Front(lhs_view) <=> Ranges::Front(rhs_view));

            if (compare == Ordering::kLess)
            {
                return Ordering::kLess;
            }

            if (compare == Ordering::kGreater)
            {
                return Ordering::kGreater;
            }

            lhs_view = Ranges::PopFront(lhs_view);
            rhs_view = Ranges::PopFront(rhs_view);
        }

        if (Ranges::IsEmpty(lhs_view) && Ranges::IsEmpty(rhs_view))
        {
            return Ordering::kEquivalent;
        }

        return Ranges::IsEmpty(lhs_view) ?
               Ordering::kLess :
               Ordering::kGreater;
    }

}

// ===========================================================================
