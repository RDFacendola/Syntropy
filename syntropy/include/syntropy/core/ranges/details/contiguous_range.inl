
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
    AreEqual(Immutable<TRangeView> lhs,
             Immutable<URangeView> rhs) noexcept
    {
        using Details::RouteCount;
        using Details::RouteData;
        using Details::RouteIsEmpty;

        if(PtrOf(lhs) == PtrOf(rhs))
        {
            return true;
        }

        if(RouteIsEmpty(lhs) && RouteIsEmpty(rhs))
        {
            return true;
        }

        if(RouteCount(lhs) != RouteCount(rhs))
        {
            return false;
        }

        if(RouteData(lhs) == RouteData(rhs))
        {
            return true;
        }

        return false;
    }

    template <Concepts::ContiguousRangeView TRangeView,
              Concepts::ContiguousRangeView URangeView>
    [[nodiscard]] constexpr Bool
    Intersect(Immutable<TRangeView> lhs,
              Immutable<URangeView> rhs) noexcept
    {
        using Details::RouteCount;
        using Details::RouteData;
        using Details::RouteIsEmpty;

        if(RouteIsEmpty(lhs) || RouteIsEmpty(rhs))
        {
            return true;    // Empty ranges intersects with everything.
        }

        if(RouteData(lhs) > RouteData(rhs) + RouteCount(rhs))
        {
            return false;
        }

        if(RouteData(rhs) > RouteData(lhs) + RouteCount(lhs))
        {
            return false;
        }

        return true;
    }

    template <Concepts::ContiguousRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    ViewOf(Immutable<TRangeView> range_view) noexcept
    {
        return range_view;
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
        using Details::RouteData;

        return *(RouteData(range_view) + index);
    }

    template <Concepts::BaseContiguousRangeView TRangeView>
    template <typename TIndex, typename TCount>
    [[nodiscard]] inline TRangeView Slice<TRangeView>
    ::operator()(Immutable<TRangeView> range_view,
                 Immutable<TIndex> index,
                 Immutable<TCount> count) const noexcept
    {
        using Details::RouteData;

        return { RouteData(range_view) + index, count };
    };
}

// ===========================================================================
