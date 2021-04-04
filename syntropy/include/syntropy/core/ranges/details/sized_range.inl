
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

    // Comparison.
    // ===========

    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs, SizedRangeTag)
    noexcept
    {
        if(Ranges::Count(lhs) != Ranges::Count(rhs))
        {
            return false;
        }

        return Ranges::AreEqual(lhs, rhs, ForwardRangeTag{});
    }

    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs, SizedRangeTag)
    noexcept
    {
        if(Ranges::Count(lhs) != Ranges::Count(rhs))
        {
            return false;
        }

        return Ranges::AreEquivalent(lhs, rhs, ForwardRangeTag{});
    }

}

// ===========================================================================
