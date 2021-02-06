
/// \file zip_range.inl
///
/// \author Raffaele D. Facendola - Nov 2020

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ZipRange.
    // =========

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    Zip(Immutable<TRanges>... ranges) noexcept
    {
        return Details::Zip(ranges...);
    }

    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range) noexcept
    {
        return Details::Unzip(range);
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<Details::ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<Details::ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<Details::ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<Details::ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

}

// ===========================================================================
