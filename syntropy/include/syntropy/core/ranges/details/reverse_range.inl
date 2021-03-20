
/// \file reverse_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* REVERSE RANGE VIEW                                                   */
    /************************************************************************/

    template <Concepts::BidirectionalRangeView TRangeView>
    template <Concepts::BidirectionalRange TRange>
    constexpr ReverseRange<TRangeView>
    ::ReverseRange(Immutable<TRange> range) noexcept
        : range_view_(Ranges::ViewOf(range))
    {

    }

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRangeView>
    ::GetFront() const noexcept
    {
        return Ranges::Back(range_view_);
    }

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr auto ReverseRange<TRangeView>
    ::PopFront() const noexcept
    {
        return ReverseRange{ Ranges::PopBack(range_view_) };
    }

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr auto ReverseRange<TRangeView>
    ::GetCount() const noexcept
    {
        return Ranges::Count(range_view_);
    }

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRangeView>
    ::GetBack() const noexcept
    {
        return Ranges::Front(range_view_);
    }

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr auto ReverseRange<TRangeView>
    ::PopBack() const noexcept
    {
        return ReverseRange{ Ranges::PopFront(range_view_) };
    }

    template <Concepts::BidirectionalRangeView TRangeView>
    template <typename TIndex>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRangeView>
    ::At(Immutable<TIndex> index) const noexcept
    {
        return Ranges::At(range_view_,
                          Ranges::Count(range_view_) - index - TIndex{ 1 });
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

    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    Reverse(Immutable<ReverseRange<TRangeView>> range_view) noexcept
    {
        return range_view.range_view_;
    }

}

// ===========================================================================
