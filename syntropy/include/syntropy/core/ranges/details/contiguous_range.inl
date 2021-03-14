
/// \file contiguous_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Contiguous range view.
    // ======================

    template <Concepts::ContiguousRangeView TRangeView>
    [[nodiscard]] constexpr auto
    Data(Immutable<TRangeView> range_view) noexcept
    {
        return Details::RouteData(range_view);
    }

    template <Concepts::ContiguousRangeView TRangeView,
              Concepts::ContiguousRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept
    {
        auto same_instance
            = (PtrOf(lhs) == PtrOf(rhs));

        auto both_empty
            = (Details::RouteIsEmpty(lhs));

        auto same_data
            = (Details::RouteData(lhs) == Details::RouteData(rhs));

        auto same_count
            = (Details::RouteCount(lhs) == Details::RouteCount(rhs));

        return same_instance || (same_count && (both_empty || same_data));
    }

    template <Concepts::ContiguousRangeView TRangeView,
              Concepts::ContiguousRangeView URangeView>
    [[nodiscard]] constexpr Bool
    Intersect(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept
    {
        using TCount = decltype(Ranges::Count(lhs));

        auto lhs_count = Ranges::Count(lhs);
        auto rhs_count = Ranges::Count(rhs);
        auto lhs_data = Ranges::Data(lhs);
        auto rhs_data = Ranges::Data(rhs);

        return (lhs_count == TCount{ 0 })
            || (rhs_count == TCount{ 0 })
            || ((lhs_data < rhs_data + rhs_count)
             && (rhs_data < lhs_data + lhs_count));
    }

}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* CONTIGUOUS RANGE VIEW EXTENSIONS                                     */
    /************************************************************************/

    template <Concepts::BaseContiguousRangeView TRangeView>
    template <typename TIndex>
    [[nodiscard]] inline decltype(auto) At<TRangeView>
    ::operator()(Immutable<TRangeView> range_view,
                 Immutable<TIndex> index) const noexcept
    {
        return *(Details::RouteData(range_view) + index);
    }

    template <Concepts::BaseContiguousRangeView TRangeView>
    template <typename TIndex, typename TCount>
    [[nodiscard]] inline TRangeView Slice<TRangeView>
    ::operator()(Immutable<TRangeView> range_view,
                 Immutable<TIndex> index,
                 Immutable<TCount> count) const noexcept
    {
        return { Details::RouteData(range_view) + index, count };
    };
}

// ===========================================================================
