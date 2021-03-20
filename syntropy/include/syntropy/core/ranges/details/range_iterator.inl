
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

    template <Concepts::ForwardRangeView TRangeView>
    template <Concepts::ForwardRange TRange>
    constexpr RangeViewIterator<TRangeView>
    ::RangeViewIterator(Immutable<TRange> range) noexcept
        : range_view_(Ranges::ViewOf(range))
    {

    }

    template <typename TRangeView>
    [[nodiscard]] constexpr
    decltype(auto) RangeViewIterator<TRangeView>
    ::operator*() const noexcept
    {
        return Ranges::Front(range_view_);
    }

    template <typename TRangeView>
    constexpr Mutable<RangeViewIterator<TRangeView>>
    RangeViewIterator<TRangeView>
    ::operator++() noexcept
    {
        range_view_ = Ranges::PopFront(range_view_);

        return *this;
    }

    template <typename TRangeView>
    [[nodiscard]] constexpr Bool RangeViewIterator<TRangeView>
    ::operator==(Immutable<RangeViewIterator> other) const noexcept
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

        return Ranges::RangeViewIterator{ range_view };
    }

    /// \brief Get an iterator past the last element in a range view.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto end(Immutable<TRange> range) noexcept
    {
        using RangeViewType = decltype(Ranges::ViewOf(range));

        return Ranges::RangeViewIterator<RangeViewType>{};
    }
}

// ===========================================================================
