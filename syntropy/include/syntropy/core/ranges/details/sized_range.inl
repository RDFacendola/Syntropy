
/// \file sized_range.inl
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Concepts::SizedRangeView TRangeView>
    [[nodiscard]] constexpr auto
    Count(Immutable<TRangeView> range_view) noexcept
    {
        using Details::RouteCount;

        return RouteCount(range_view);
    }

    template <Concepts::SizedRangeView TRangeView,
              Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRangeView> lhs,
             Immutable<URangeView> rhs) noexcept
    {
        return (PtrOf(lhs) == PtrOf(rhs));
    }

    template <Concepts::SizedRangeView TRangeView,
              Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRangeView> lhs,
                  Immutable<URangeView> rhs) noexcept
    {
        using Details::RouteCount;
        using Details::RouteFront;
        using Details::RouteIsEmpty;
        using Details::RoutePopFront;

        if (AreEqual(lhs, rhs))
        {
            return true;
        }

        if (RouteCount(lhs) == RouteCount(rhs))
        {
            auto lhs_copy = lhs;
            auto rhs_copy = rhs;

            for (;   (!RouteIsEmpty(lhs_copy)) && (!RouteIsEmpty(rhs_copy))
                  && (RouteFront(lhs_copy) == RouteFront(rhs_copy)); )
            {
                lhs_copy = RoutePopFront(lhs_copy);
                rhs_copy = RoutePopFront(rhs_copy);
            }

            return RouteIsEmpty(lhs_copy);
        }

        return false;
    }

    template <Concepts::SizedRangeView TRangeView,
              Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRangeView> lhs,
            Immutable<URangeView> rhs) noexcept
    {
        using Details::RouteCount;
        using Details::RouteFront;
        using Details::RouteIsEmpty;
        using Details::RoutePopFront;

        auto lhs_copy = lhs;
        auto rhs_copy = rhs;

        for (; !RouteIsEmpty(lhs_copy) && !RouteIsEmpty(rhs_copy); )
        {
            auto compare = (RouteFront(lhs_copy) <=> RouteFront(rhs_copy));

            if (compare == Ordering::kLess)
            {
                return Ordering::kLess;
            }

            if (compare == Ordering::kGreater)
            {
                return Ordering::kGreater;
            }

            lhs_copy = RoutePopFront(lhs_copy);
            rhs_copy = RoutePopFront(rhs_copy);
        }

        if (RouteIsEmpty(lhs_copy) && RouteIsEmpty(rhs_copy))
        {
            return Ordering::kEquivalent;
        }

        return RouteIsEmpty(lhs_copy) ? Ordering::kLess : Ordering::kGreater;
    }
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* SIZED RANGE EXTENSIONS                                               */
    /************************************************************************/

    template <Concepts::BaseSizedRangeView TRangeView>
    Bool IsEmpty<TRangeView>
    ::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        using Details::RouteCount;

        return RouteCount(range_view) ==
            Templates::RangeViewCountType<TRangeView>{};
    }

}

// ===========================================================================
