
/// \file range_iterator.inl
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* RANGE VIEW ITERATOR                                                  */
    /************************************************************************/

    template <Concepts::ForwardRange TRangeView>
    template <Concepts::ForwardRange TRange>
    constexpr
    RangeIterator<TRangeView>
    ::RangeIterator(Immutable<TRange> range) noexcept
        : range_view_(Ranges::ViewOf(range))
    {

    }

    template <Concepts::ForwardRange TRangeView>
    [[nodiscard]] constexpr decltype(auto)
    RangeIterator<TRangeView>
    ::operator*() const noexcept
    {
        return Ranges::Front(range_view_);
    }

    template <Concepts::ForwardRange TRangeView>
    constexpr Mutable<RangeIterator<TRangeView>>
    RangeIterator<TRangeView>
    ::operator++() noexcept
    {
        // PopFront returns a RangeView<Blah>, whereas range_view_ is
        // a BaseSpan!

        auto range_view = Ranges::PopFront(range_view_);

        range_view_ = range_view;

        return *this;
    }

    template <Concepts::ForwardRange TRangeView>
    [[nodiscard]] constexpr Bool RangeIterator<TRangeView>
    ::operator==(Immutable<RangeIterator> other) const noexcept
    {
        return Ranges::IsEmpty(range_view_);
    }

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range view.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto begin(Immutable<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Ranges::RangeIterator{ range_view };
    }

    /// \brief Get an iterator past the last element in a range view.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto end(Immutable<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        using RangeViewType = Ranges::Templates::RangeViewTypeOf<TRange>;

        // return Ranges::RangeIterator<RangeViewType>{};
        return Ranges::RangeIterator{ range_view };
    }
}

// ===========================================================================
