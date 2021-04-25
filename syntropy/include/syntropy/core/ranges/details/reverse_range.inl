
/// \file reverse_range.inl
///
/// \author Raffaele D. Facendola - November 2020.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    template <Ranges::BidirectionalRange TRange>
    template <Ranges::BidirectionalRange URange>
    constexpr ReverseRange<TRange>
    ::ReverseRange(Immutable<URange> range) noexcept
        : range_(Ranges::ViewOf(range))
    {

    }

    template <Ranges::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::GetFront() const noexcept
    {
        return Ranges::Back(range_);
    }

    template <Ranges::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::PopFront() const noexcept
    {
        return ReverseRange{ Ranges::PopBack(range_) };
    }

    template <Ranges::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::GetCount() const noexcept
    {
        return Ranges::Count(range_);
    }

    template <Ranges::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::GetBack() const noexcept
    {
        return Ranges::Front(range_);
    }

    template <Ranges::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::PopBack() const noexcept
    {
        return ReverseRange{ Ranges::PopFront(range_) };
    }

    template <Ranges::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::At(Int index) const noexcept
    {
        return Ranges::At(range_,
                          Ranges::Count(range_) - index - ToInt(1));
    }

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <BidirectionalRange TRange>
    [[nodiscard]] constexpr auto
    Reverse(Immutable<TRange> range) noexcept
    {
        return ReverseRange{ Ranges::ViewOf(range) };
    }

    template <BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    Reverse(Immutable<ReverseRange<TRange>> range) noexcept
    {
        return range.range_;
    }
}

// ===========================================================================
