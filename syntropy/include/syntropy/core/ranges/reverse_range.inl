
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
    template <Ranges::Concepts::BidirectionalRange URange>
    constexpr ReverseRange<TRange>
    ::ReverseRange(Immutable<URange> range) noexcept
        : range_(range)
    {

    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::GetFront() const noexcept
    {
        return RouteBack(range_);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::PopFront() const noexcept
    {
        return ReverseRange<TRange>{ RoutePopBack(range_) };
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr Bool ReverseRange<TRange>
    ::IsEmpty() const noexcept
    {
        return RouteIsEmpty(range_);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::GetCount() const noexcept
    {
        return RouteCount(range_);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::GetBack() const noexcept
    {
        return RouteFront(range_);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::PopBack() const noexcept
    {
        return ReverseRange<TRange>{ RoutePopFront(range_) };
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    template <typename TIndex>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::At(Immutable<TIndex> index) const noexcept
    {
        return RouteAt(range_, RouteCount(range_) - index - TIndex{ 1 });
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    template <typename TIndex, typename TCount>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept
    {
        return ReverseRange<TRange>
        {
            RouteSlice(range_, RouteCount(range_) - index - count, count)
        };
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr ReverseRange<TRange>
    Reverse(Immutable<TRange> range) noexcept
    {
        return ReverseRange<TRange>(range);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    Reverse(Immutable<ReverseRange<TRange>> range) noexcept
    {
        return range.range_;
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    auto Reverse(Immutable<TRange> range) noexcept
    {
        return Details::Reverse(range);
    }
}

// ===========================================================================
