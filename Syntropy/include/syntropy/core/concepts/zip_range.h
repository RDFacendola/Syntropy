
/// \file zip_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for adapters used to zip multiple ranges together.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/concepts/ntuple.h"

#include "syntropy/core/concepts/details/zip_range_details.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* ZIP RANGE                                                            */
    /************************************************************************/

    /// \brief Adapter class used to zip many ranges together.
    /// The new range has elements equal to the Tuple consisting of each element in each source range, tied together.
    template <Concepts::ForwardRange... TRanges>
    using ZipRange = Details::ZipRange<TRanges...>;

    /// \brief Create a new range by element-wise joining different ranges and flattening zip-ranges on the first level.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> Zip(Immutable<TRanges>... ranges) noexcept;

    /// \brief Unzip a range, producing a tuple containing the individual ranges.
    /// \remarks If the provided range is not a Zip-range, the results is a tuple with a single element.
    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr auto Unzip(Immutable<TRange> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) Get(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) Get(Mutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) Get(Immovable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) Get(Movable<ZipRange<TRanges...>> range) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ZipRange.
    // =========

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> Zip(Immutable<TRanges>... ranges) noexcept
    {
        return Details::Zip(ranges...);
    }

    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr auto Unzip(Immutable<TRange> range) noexcept
    {
        return Details::Unzip(range);
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) Get(Immutable<Details::ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) Get(Mutable<Details::ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) Get(Immovable<Details::ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) Get(Movable<Details::ZipRange<TRanges...>> range) noexcept
    {
        return Tuples::Get<VIndex>(Unzip(range));
    }

}

// ===========================================================================
