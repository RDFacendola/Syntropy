
/// \file random_access_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* SLICE RANGE                                                          */
    /************************************************************************/

    template <Concepts::RandomAccessRangeView TRangeView>
    template <Concepts::RandomAccessRange TRange>
    SliceRange<TRangeView>
    ::SliceRange(Immutable<TRange> range,
                 Immutable<SliceRange::CountType> offset,
                 Immutable<SliceRange::CountType> count) noexcept
        : range_view(Ranges::ViewOf(range))
        , offset_(offset)
        , count_(count)
    {

    }

    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr CountType
    SliceRange<TRangeView>
    ::GetCount() const noexcept
    {
        return count_;
    }

    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto)
    SliceRange<TRangeView>
    ::At(Immutable<CountType> index) const noexcept
    {
        return Ranges::At(index + offset);
    }

    template <Concepts::RandomAccessRangeView TRangeView>
    template <typename TRange>
    [[nodiscard]] constexpr auto
    SliceRange<TRangeView>
    ::GetData() const noexcept
    {
        return Ranges::Data(range_view) + offset;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // RandomAccessRange.
    // =================

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    Slice(Immutable<TRange> range,
          Immutable<TCount> index,
          Immutable<TCount> count) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return SliceRange(range_view, index, count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    Front(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Ranges::Slice(range_view,
                             Templates::RangeCountType<TRange>{ 0 },
                             count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    Back(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Ranges::Slice(range_view,
                             Ranges::Count(range_view) - count,
                             count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    PopFront(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Ranges::Slice(range_view,
                             count,
                             Ranges::Count(range_view) - count);
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    PopBack(Immutable<TRange> range, Immutable<TCount> count) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Ranges::Slice(range_view,
                             Templates::RangeCountType<TRange>{ 0 },
                             Ranges::Count(range_view) - count);
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Tuples::MakeTuple(Ranges::Front(range_view),
                                 Ranges::PopFront(range_view));
    }

    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Tuples::MakeTuple(Ranges::Back(range_view),
                                 Ranges::PopBack(range_view));
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range,
               Immutable<TCount> count) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Tuples::MakeTuple(Ranges::Front(range_view, count),
                                 Ranges::PopFront(range_view, count));
    }

    template <Concepts::RandomAccessRange TRange, typename TCount>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range,
              Immutable<TCount> count) noexcept
    {
        auto range_view = Ranges::ViewOf(range);

        return Tuples::MakeTuple(Ranges::Back(range_view, count),
                                 Ranges::PopBack(range_view, count));
    }

    // RandomAccessRangeView.
    // ======================

    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    ViewOf(Immutable<TRangeView> range_view) noexcept
    {
        return range_view;
    }

}

// ===========================================================================
