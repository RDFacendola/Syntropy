
/// \file zip_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* ZIP RANGE                                                            */
    /************************************************************************/

    template <Concepts::ForwardRange... TRanges>
    constexpr ZipRange<TRanges...>
    ::ZipRange(Immutable<TRanges>... ranges) noexcept
        : ranges_{ ranges... }
    {

    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRanges...>
    ::GetFront() const noexcept
    {
        auto zip_front = [](Immutable<TRanges>... ranges)
        {
            return Tuples::MakeTuple(RouteFront(ranges)...);
        };

        return Tuples::Apply(zip_front, ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::PopFront() const noexcept
    {
        auto zip_pop_front = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ RoutePopFront(ranges)... };
        };

        return Tuples::Apply(zip_pop_front, ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Bool ZipRange<TRanges...>
    ::IsEmpty() const noexcept
    {
        auto zip_is_empty = [](Immutable<TRanges>... ranges)
        {
            return (sizeof...(TRanges) == 0) || (RouteIsEmpty(ranges) || ...);
        };

        return Tuples::Apply(zip_is_empty, ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::GetCount() const noexcept
    {
        auto zip_min_count = [](Immutable<TRanges>... ranges)
        {
            return Math::Min(RouteCount(ranges)...);
        };

        return Tuples::Apply(zip_min_count, ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRanges...>
    ::GetBack() const noexcept
    {
        auto zip_back = [](Immutable<TRanges>... ranges)
        {
            return Tuples::MakeTuple( RouteBack(ranges)... );
        };

        return Tuples::Apply(zip_back, ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::PopBack() const noexcept
    {
        auto zip_pop_back = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ RoutePopBack(ranges)... };
        };

        return Tuples::Apply(zip_pop_back, ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    template <typename TIndex>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRanges...>
    ::At(Immutable<TIndex> index) const noexcept
    {
        auto zip_select = [index](Immutable<TRanges>... ranges)
        {
            return Tuples::MakeTuple( RouteAt(ranges, index)... );
        };

        return Tuples::Apply(zip_select, ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    template <typename TIndex, typename TCount>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept
    {
        auto zip_select = [index, count](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ RouteSlice(ranges, index, count)... };
        };

        return Tuples::Apply(zip_select, ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::GetData() const noexcept
    {
        auto zip_data = [](Immutable<TRanges>... ranges)
        {
            return Tuples::MakeTuple( RouteData(ranges)... );
        };

        return Tuples::Apply(zip_data, ranges_);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    MakeZipRange(Immutable<TRanges>... ranges) noexcept
    {
        return { ranges... };
    }

    template <Syntropy::Tuples::Concepts::NTuple TTuple>
    [[nodiscard]] constexpr auto
    MakeZipRangeFromTuple(Immutable<TTuple> ranges) noexcept
    {
        auto make_zip_range = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                return ZipRange(Tuples::Get<TIndex>(ranges)...);
            };

        return make_zip_range(
            Syntropy::Tuples::Templates::TupleSequenceFor<decltype(ranges)>{});
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        return range.ranges_;
    }

    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range) noexcept
    {
        return Tuples::MakeTuple(range);
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    Zip(Immutable<TRanges>... ranges) noexcept
    {
        return MakeZipRangeFromTuple(Tuples::Concatenate(Unzip(ranges)...));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

}

// ===========================================================================
