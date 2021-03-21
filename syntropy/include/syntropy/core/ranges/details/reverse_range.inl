
/// \file reverse_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    template <Concepts::BidirectionalRange TRange>
    template <Concepts::BidirectionalRange URange>
    constexpr ReverseRange<TRange>
    ::ReverseRange(Immutable<URange> range) noexcept
        : range_(Ranges::ViewOf(range))
    {

    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::GetFront() const noexcept
    {
        return Ranges::Back(range_);
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::PopFront() const noexcept
    {
        return ReverseRange{ Ranges::PopBack(range_) };
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::GetCount() const noexcept
    {
        return Ranges::Count(range_);
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::GetBack() const noexcept
    {
        return Ranges::Front(range_);
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::PopBack() const noexcept
    {
        return ReverseRange{ Ranges::PopFront(range_) };
    }

    template <Concepts::BidirectionalRange TRange>
    template <typename TIndex>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::At(Immutable<TIndex> index) const noexcept
    {
        return Ranges::At(range_,
                          Ranges::Count(range_) - index - TIndex{ 1 });
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto
    Reverse(Immutable<TRange> range) noexcept
    {
        return ReverseRange{ Ranges::ViewOf(range) };
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    Reverse(Immutable<ReverseRange<TRange>> range) noexcept
    {
        return range.range_;
    }

}

// ===========================================================================
