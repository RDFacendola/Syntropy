
/// \file zip_range.inl
///
/// \author Raffaele D. Facendola - November 2020.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* ZIP RANGE                                                            */
    /************************************************************************/

    template <Ranges::ForwardRange... TRanges>
    constexpr ZipRange<TRanges...>
    ::ZipRange(Immutable<TRanges>... ranges) noexcept
        : ranges_{ ranges... }
    {

    }

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRanges...>
    ::GetFront() const noexcept
    {
        auto zip_front = [](Immutable<TRanges>... ranges)
        {
            return MakeTuple(Front(ranges)...);
        };

        return Tuples::Apply(zip_front, ranges_);
    }

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::PopFront() const noexcept
    {
        auto zip_pop_front = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>
            {
                Ranges::PopFront(ranges)...
            };
        };

        return Tuples::Apply(zip_pop_front, ranges_);
    }

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr Bool ZipRange<TRanges...>
    ::IsEmpty() const noexcept
    {
        auto zip_is_empty = [](Immutable<TRanges>... ranges)
        {
            return (sizeof...(TRanges) == 0)
                || (Ranges::IsEmpty(ranges)
                || ...);
        };

        return Tuples::Apply(zip_is_empty, ranges_);
    }

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::GetCount() const noexcept
    {
        auto zip_min_count = [](Immutable<TRanges>... ranges)
        {
            return Math::Min(Count(ranges)...);
        };

        return Tuples::Apply(zip_min_count, ranges_);
    }

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRanges...>
    ::GetBack() const noexcept
    {
        auto zip_back = [](Immutable<TRanges>... ranges)
        {
            return MakeTuple(Back(ranges)...);
        };

        return Tuples::Apply(zip_back, ranges_);
    }

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::PopBack() const noexcept
    {
        auto zip_pop_back = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>
            {
                Ranges::PopBack(ranges)...
            };
        };

        return Tuples::Apply(zip_pop_back, ranges_);
    }

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRanges...>
    ::At(Int index) const noexcept
    {
        auto zip_select = [index](Immutable<TRanges>... ranges)
        {
            return MakeTuple(Ranges::At(ranges, index)...);
        };

        return Tuples::Apply(zip_select, ranges_);
    }

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::GetData() const noexcept
    {
        auto zip_data = [](Immutable<TRanges>... ranges)
        {
            return MakeTuple( Data(ranges)... );
        };

        return Tuples::Apply(zip_data, ranges_);
    }

    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    template <Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    Ranges
    ::MakeZipRange(Immutable<TRanges>... ranges) noexcept
    {
        return { ranges... };
    }

    template <Syntropy::Tuples::IsTuple TTuple>
    [[nodiscard]] constexpr auto
    Ranges
    ::MakeZipRangeFromTuple(Immutable<TTuple> ranges) noexcept
    {
        auto make_zip_range = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                return ZipRange(Tuples::Get<TIndex>(ranges)...);
            };

        using RangesType = decltype(ranges);

        return make_zip_range(
            Syntropy::Tuples::SequenceOf<RangesType>{});
    }

    template <ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    Ranges
    ::Zip(Immutable<TRanges>... ranges) noexcept
    {
        return MakeZipRangeFromTuple(
            Concatenate(Unzip(ranges)...));
    }

    template <ForwardRange... TRanges>
    [[nodiscard]] constexpr auto
    Ranges
    ::Unzip(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        return range.ranges_;
    }

    template <ForwardRange TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::Unzip(Immutable<TRange> range) noexcept
    {
        return MakeTuple(range);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Int VIndex, Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Ranges::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

}

// ===========================================================================
