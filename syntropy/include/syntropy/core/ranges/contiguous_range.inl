
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

    // Contiguous range.
    // =================

    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] constexpr auto
    Data(Immutable<TRange> range) noexcept
    {
        return Details::RouteData(range);
    }

    template <Concepts::ContiguousRange TRange,
              Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
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

    template <Concepts::ContiguousRange TRange,
              Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    Intersect(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
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
    /* CONTIGUOUS RANGE EXTENSIONS                                          */
    /************************************************************************/

    template <Concepts::BaseContiguousRange TRange>
    template <typename TIndex>
    [[nodiscard]] inline decltype(auto) At<TRange>
    ::operator()(Immutable<TRange> range,
                 Immutable<TIndex> index) const noexcept
    {
        return *(Details::RouteData(range) + index);
    }

    template <Concepts::BaseContiguousRange TRange>
    template <typename TIndex, typename TCount>
    [[nodiscard]] inline TRange Slice<TRange>
    ::operator()(Immutable<TRange> range,
                 Immutable<TIndex> index,
                 Immutable<TCount> count) const noexcept
    {
        return { Details::RouteData(range) + index, count };
    };
}

// ===========================================================================
