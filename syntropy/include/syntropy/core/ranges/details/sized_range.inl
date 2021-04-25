
/// \file sized_range.inl
///
/// \author Raffaele D. Facendola - November 2020.
/// \author Raffaele D. Facendola - January 2021.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    template <SizedRange TRange, SizedRange URange>
    [[nodiscard]] constexpr Bool
    Ranges
    ::AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs, SizedRangeTag)
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
    Ranges
    ::AreEquivalent(Immutable<TRange> lhs,
                    Immutable<URange> rhs,
                    SizedRangeTag)
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
