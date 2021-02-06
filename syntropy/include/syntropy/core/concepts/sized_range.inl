
/// \file sized_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Concepts::SizedRange TRange>
    [[nodiscard]] constexpr auto
    Count(Immutable<TRange> range) noexcept
    {
        return Details::RouteCount(range);
    }

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        return (PtrOf(lhs) == PtrOf(rhs));
    }

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
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

    template <Concepts::SizedRange TRange, Concepts::SizedRange URange>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
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

    template <Concepts::BaseSizedRange TRange>
    Bool IsEmpty<TRange>::operator()(Immutable<TRange> range) const noexcept
    {
        return
            Details::RouteCount(range) == Templates::RangeCountType<TRange>{};
    }
    
}

// ===========================================================================
