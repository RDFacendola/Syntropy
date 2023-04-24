
/// \file random_access_range.inl
///
/// \author Raffaele D. Facendola - November 2020.
/// \author Raffaele D. Facendola - January 2021.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::Front(Immutable<TRange> range, Int count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              ToInt(0),
                              count);
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::Back(Immutable<TRange> range, Int count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              Ranges::Count(range) - count,
                              count);
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::PopFront(Immutable<TRange> range, Int count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              count,
                              Ranges::Count(range) - count);
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::PopBack(Immutable<TRange> range, Int count) noexcept
    {
        return Ranges::Select(Ranges::ViewOf(range),
                              ToInt(0),
                              Ranges::Count(range) - count);
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::SliceFront(Immutable<TRange> range) noexcept
    {
        return MakeTuple(Ranges::Front(range), Ranges::PopFront(range));
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::SliceBack(Immutable<TRange> range) noexcept
    {
        return MakeTuple(Ranges::Back(range), Ranges::PopBack(range));
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::SliceFront(Immutable<TRange> range, Int count) noexcept
    {
        return MakeTuple(Ranges::Front(range, count),
                         Ranges::PopFront(range, count));
    }

    template <RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::SliceBack(Immutable<TRange> range, Int count) noexcept
    {
        return MakeTuple(Ranges::Back(range, count),
                         Ranges::PopBack(range, count));
    }

}

// ===========================================================================
