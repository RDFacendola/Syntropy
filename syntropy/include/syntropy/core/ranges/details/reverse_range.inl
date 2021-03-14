
/// \file reverse_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* REVERSE RANGE VIEW                                                   */
    /************************************************************************/

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    template <Ranges::Concepts::BidirectionalRangeView URangeView>
    constexpr ReverseRange<TRangeView>
    ::ReverseRange(Immutable<URangeView> range_view) noexcept
        : range_view_(range_view)
    {

    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRangeView>
    ::GetFront() const noexcept
    {
        return RouteBack(range_view_);
    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr auto ReverseRange<TRangeView>
    ::PopFront() const noexcept
    {
        return ReverseRange<TRangeView>{ RoutePopBack(range_view_) };
    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr Bool ReverseRange<TRangeView>
    ::IsEmpty() const noexcept
    {
        return RouteIsEmpty(range_view_);
    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr auto ReverseRange<TRangeView>
    ::GetCount() const noexcept
    {
        return RouteCount(range_view_);
    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRangeView>
    ::GetBack() const noexcept
    {
        return RouteFront(range_view_);
    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr auto ReverseRange<TRangeView>
    ::PopBack() const noexcept
    {
        return ReverseRange<TRangeView>{ RoutePopFront(range_view_) };
    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    template <typename TIndex>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRangeView>
    ::At(Immutable<TIndex> index) const noexcept
    {
        return RouteAt(range_view_, RouteCount(range_view_) - index - TIndex{ 1 });
    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    template <typename TIndex, typename TCount>
    [[nodiscard]] constexpr auto ReverseRange<TRangeView>
    ::Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept
    {
        return ReverseRange<TRangeView>
        {
            RouteSlice(range_view_, RouteCount(range_view_) - index - count, count)
        };
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr ReverseRange<TRangeView>
    Reverse(Immutable<TRangeView> range_view) noexcept
    {
        return ReverseRange<TRangeView>(range_view);
    }

    template <Ranges::Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    Reverse(Immutable<ReverseRange<TRangeView>> range_view) noexcept
    {
        return range_view.range_view_;
    }

}

// ===========================================================================
