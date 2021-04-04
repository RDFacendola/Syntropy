
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
    AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto left = Ranges::ViewOf(lhs);
        auto right = Ranges::ViewOf(rhs);

        if (Ranges::Count(left) == Ranges::Count(right))
        {
            for(; !Ranges::IsEmpty(left);)
            {
                if(!Algorithms::AreEqual(left, right))
                {
                    return false;
                }

                left = Ranges::PopFront(left);
                right = Ranges::PopFront(right);
            }

            return true;
        }

        return false;
    }

}

// ===========================================================================
