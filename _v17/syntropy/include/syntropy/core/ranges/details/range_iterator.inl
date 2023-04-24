
/// \file range_iterator.inl
///
/// \author Raffaele D. Facendola - January 2021.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE VIEW ITERATOR                                                  */
    /************************************************************************/

    template <ForwardRange TRangeView>
    template <ForwardRange TRange>
    constexpr
    RangeIterator<TRangeView>
    ::RangeIterator(Immutable<TRange> range) noexcept
        : range_view_(Ranges::ViewOf(range))
    {

    }

    template <ForwardRange TRangeView>
    [[nodiscard]] constexpr decltype(auto)
    RangeIterator<TRangeView>
    ::operator*() const noexcept
    {
        return Ranges::Front(range_view_);
    }

    template <ForwardRange TRangeView>
    constexpr Mutable<RangeIterator<TRangeView>>
    RangeIterator<TRangeView>
    ::operator++() noexcept
    {
        auto range_view = Ranges::PopFront(range_view_);

        range_view_ = range_view;

        return *this;
    }

    template <ForwardRange TRangeView>
    [[nodiscard]] constexpr Bool RangeIterator<TRangeView>
    ::operator==(Immutable<RangeIterator> other) const noexcept
    {
        return Ranges::IsEmpty(range_view_);
    }

    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range view.
    template <Ranges::ForwardRange TRange>
    constexpr auto begin(Immutable<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return RangeIterator{ range_view };
    }

    /// \brief Get an iterator past the last element in a range view.
    template <Ranges::ForwardRange TRange>
    constexpr auto end(Immutable<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        using RangeViewType = Ranges::RangeViewTypeOf<TRange>;

         return Ranges::RangeIterator<RangeViewType>{};
    }
}

// ===========================================================================
