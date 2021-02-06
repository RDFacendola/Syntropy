
/// \file reverse_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    template <Ranges::Concepts::BidirectionalRange TRange>
    auto Reverse(Immutable<TRange> range) noexcept
    {
        return Details::Reverse(range);
    }
}

// ===========================================================================
