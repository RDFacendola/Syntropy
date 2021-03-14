
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
        return Details::RouteCount(range_view);
    }

    template <Concepts::SizedRangeView TRangeView, Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept
    {
        return (PtrOf(lhs) == PtrOf(rhs));
    }

    template <Concepts::SizedRangeView TRangeView, Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept
    {
        if (AreEqual(lhs, rhs))
        {
            return true;
        }

        if (Details::RouteCount(lhs) == Details::RouteCount(rhs))
        {
            auto lhs_copy = lhs;
            auto rhs_copy = rhs;

            for (;   (!Details::RouteIsEmpty(lhs_copy))
                  && (!Details::RouteIsEmpty(rhs_copy))
                  && (Details::RouteFront(lhs_copy)
                    == Details::RouteFront(rhs_copy)); )
            {
                lhs_copy = Details::RoutePopFront(lhs_copy);
                rhs_copy = Details::RoutePopFront(rhs_copy);
            }

            return Details::RouteIsEmpty(lhs_copy);
        }

        return false;
    }

    template <Concepts::SizedRangeView TRangeView, Concepts::SizedRangeView URangeView>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRangeView> lhs, Immutable<URangeView> rhs) noexcept
    {
        auto lhs_copy = lhs;
        auto rhs_copy = rhs;

        for (;    !Details::RouteIsEmpty(lhs_copy)
               && !Details::RouteIsEmpty(rhs_copy); )
        {
            auto compare
                 = (Details::RouteFront(lhs_copy)
                <=> Details::RouteFront(rhs_copy));

            if (compare == Ordering::kLess)
            {
                return Ordering::kLess;
            }

            if (compare == Ordering::kGreater)
            {
                return Ordering::kGreater;
            }

            lhs_copy = Details::RoutePopFront(lhs_copy);
            rhs_copy = Details::RoutePopFront(rhs_copy);
        }

        if (Details::RouteIsEmpty(lhs_copy) && Details::RouteIsEmpty(rhs_copy))
        {
            return Ordering::kEquivalent;
        }

        return Details::RouteIsEmpty(lhs_copy) ?
            Ordering::kLess :
            Ordering::kGreater;
    }
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* SIZED RANGE EXTENSIONS                                               */
    /************************************************************************/

    template <Concepts::BaseSizedRangeView TRangeView>
    Bool IsEmpty<TRangeView>::operator()(Immutable<TRangeView> range_view) const noexcept
    {
        return
            Details::RouteCount(range_view) == Templates::RangeCountType<TRangeView>{};
    }

}

// ===========================================================================
