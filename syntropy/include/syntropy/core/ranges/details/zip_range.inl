
/// \file zip_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* ZIP RANGE VIEW                                                       */
    /************************************************************************/

    template <Concepts::ForwardRangeView... TRangeViews>
    constexpr ZipRange<TRangeViews...>
    ::ZipRange(Immutable<TRangeViews>... range_views) noexcept
        : range_views_{ range_views... }
    {

    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRangeViews...>
    ::GetFront() const noexcept
    {
        auto zip_front = [](Immutable<TRangeViews>... range_views)
        {
            return Tuples::MakeTuple(RouteFront(range_views)...);
        };

        return Tuples::Apply(zip_front, range_views_);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr auto ZipRange<TRangeViews...>
    ::PopFront() const noexcept
    {
        auto zip_pop_front = [](Immutable<TRangeViews>... range_views)
        {
            return ZipRange<TRangeViews...>{ RoutePopFront(range_views)... };
        };

        return Tuples::Apply(zip_pop_front, range_views_);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr Bool ZipRange<TRangeViews...>
    ::IsEmpty() const noexcept
    {
        auto zip_is_empty = [](Immutable<TRangeViews>... range_views)
        {
            return (sizeof...(TRangeViews) == 0) || (RouteIsEmpty(range_views) || ...);
        };

        return Tuples::Apply(zip_is_empty, range_views_);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr auto ZipRange<TRangeViews...>
    ::GetCount() const noexcept
    {
        auto zip_min_count = [](Immutable<TRangeViews>... range_views)
        {
            return Math::Min(RouteCount(range_views)...);
        };

        return Tuples::Apply(zip_min_count, range_views_);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRangeViews...>
    ::GetBack() const noexcept
    {
        auto zip_back = [](Immutable<TRangeViews>... range_views)
        {
            return Tuples::MakeTuple( RouteBack(range_views)... );
        };

        return Tuples::Apply(zip_back, range_views_);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr auto ZipRange<TRangeViews...>
    ::PopBack() const noexcept
    {
        auto zip_pop_back = [](Immutable<TRangeViews>... range_views)
        {
            return ZipRange<TRangeViews...>{ RoutePopBack(range_views)... };
        };

        return Tuples::Apply(zip_pop_back, range_views_);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    template <typename TIndex>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRangeViews...>
    ::At(Immutable<TIndex> index) const noexcept
    {
        auto zip_select = [index](Immutable<TRangeViews>... range_views)
        {
            return Tuples::MakeTuple( RouteAt(range_views, index)... );
        };

        return Tuples::Apply(zip_select, range_views_);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    template <typename TIndex, typename TCount>
    [[nodiscard]] constexpr auto ZipRange<TRangeViews...>
    ::Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept
    {
        auto zip_select = [index, count](Immutable<TRangeViews>... range_views)
        {
            return ZipRange<TRangeViews...>{ RouteSlice(range_views, index, count)... };
        };

        return Tuples::Apply(zip_select, range_views_);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr auto ZipRange<TRangeViews...>
    ::GetData() const noexcept
    {
        auto zip_data = [](Immutable<TRangeViews>... range_views)
        {
            return Tuples::MakeTuple( RouteData(range_views)... );
        };

        return Tuples::Apply(zip_data, range_views_);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr ZipRange<TRangeViews...>
    MakeZipRange(Immutable<TRangeViews>... range_views) noexcept
    {
        return { range_views... };
    }

    template <Syntropy::Tuples::Concepts::NTuple TTuple>
    [[nodiscard]] constexpr auto
    MakeZipRangeFromTuple(Immutable<TTuple> range_views) noexcept
    {
        auto make_zip_range = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                return ZipRange(Tuples::Get<TIndex>(range_views)...);
            };

        return make_zip_range(
            Syntropy::Tuples::Templates::TupleSequenceFor<decltype(range_views)>{});
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<ZipRange<TRangeViews...>> range_view) noexcept
    {
        return range_view.range_views_;
    }

    template <Concepts::ForwardRangeView TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range_view) noexcept
    {
        return Tuples::MakeTuple(range_view);
    }

    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr ZipRange<TRangeViews...>
    Zip(Immutable<TRangeViews>... range_views) noexcept
    {
        return MakeZipRangeFromTuple(Tuples::Concatenate(Unzip(range_views)...));
    }

    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<ZipRange<TRangeViews...>> range_view) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range_view));
    }

    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<ZipRange<TRangeViews...>> range_view) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range_view));
    }

    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<ZipRange<TRangeViews...>> range_view) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range_view));
    }

    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<ZipRange<TRangeViews...>> range_view) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range_view));
    }

}

// ===========================================================================
