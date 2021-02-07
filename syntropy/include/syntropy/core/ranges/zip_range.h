
/// \file zip_range.h
///
/// \brief his header is part of the Syntropy core module. It contains
/// definitions for adapters used to zip multiple ranges together.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/tuples/ntuple.h"
#include "syntropy/core/ranges/forward_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* ZIP RANGE                                                            */
    /************************************************************************/

    /// \brief Adapter class used to zip many ranges together.
    ///
    /// The new range has elements equal to the Tuple consisting of each
    /// element in each source range, tied together.
    /// \author Raffaele D. Facendola - November 2020.
    /// \author Raffaele D. Facendola - January 2021.
    template <Concepts::ForwardRange... TRanges>
    class ZipRange
    {
        template <Concepts::ForwardRange... URanges>
        friend constexpr auto
        Unzip(Immutable<ZipRange<URanges...>> range) noexcept;

    public:

        /// \brief Create an empty zip-range.
        constexpr
        ZipRange() noexcept = default;

        /// \brief Default copy-constructor.
        constexpr
        ZipRange(Immutable<ZipRange> rhs) noexcept = default;

        /// \brief Create a new range by zipping together one or more ranges.
        constexpr
        ZipRange(Immutable<TRanges>... ranges) noexcept;

        /// \brief Default destructor.
        ~ZipRange() noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<ZipRange>
        operator=(Immutable<ZipRange> range) noexcept = default;

        /// \brief Access range's first element.
        ///
        /// \remarks Accessing the first element of an empty range results
        ///          in undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetFront() const noexcept;

        /// \brief Discard range's first element and return the resulting
        ///        range.
        ///
        /// \remarks If the provided range is empty, the behavior of this
        ///          method is undefined.
        [[nodiscard]] constexpr auto
        PopFront() const noexcept;

        /// \brief Check whether the range is empty.
        [[nodiscard]] constexpr Bool
        IsEmpty() const noexcept;

        /// \brief Get range's elements count.
        [[nodiscard]] constexpr auto
        GetCount() const noexcept;

        /// \brief Access range's last element.
        ///
        /// \remarks Accessing the last element of an empty range results in
        ///          undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetBack() const noexcept;

        /// \brief Discard range's last element and return the resulting range.
        ///
        /// \remarks If the provided range is empty, the behavior of this
        ///          method is undefined.
        [[nodiscard]] constexpr auto
        PopBack() const noexcept;

        /// \brief Access range's element by index.
        ///
        /// \remarks Exceeding range boundaries results in undefined behavior.
        template <typename TIndex>
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<TIndex> index) const noexcept;

        /// \brief Obtain a view to a sub-range.
        ///
        /// \remarks Exceeding range boundaries results in undefined behavior.
        template <typename TIndex, typename TCount>
        [[nodiscard]] constexpr auto
        Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept;

        /// \brief Access range's element storage.
        ///
        /// \remarks Accessing an empty range's storage results in
        ///          undefined behavior.
        [[nodiscard]] constexpr auto
        GetData() const noexcept;

    private:

        /// \brief Underlying ranges.
        Tuples::Tuple<TRanges...> ranges_;

    };

    /// \brief Deduction rule.
    template <Concepts::ForwardRange... TRanges>
    ZipRange(Immutable<TRanges>...) -> ZipRange<TRanges...>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new ZipRange by deducing templates types
    ///        from arguments.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    MakeZipRange(Immutable<TRanges>... ranges) noexcept;

    /// \brief Create a new ZipRange by deducing templates types from
    ///        arguments provided
    template <Syntropy::Tuples::Concepts::NTuple TTuple>
    [[nodiscard]] constexpr auto
    MakeZipRangeFromTuple(Immutable<TTuple> ranges) noexcept;

    /// \brief Access the individual ranges in a zip range.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Unzip a range, producing a tuple containing the individual
    ///        ranges.
    ///
    /// \remarks If the provided range is not a Zip-range, the results is a
    ///          tuple with a single element.
    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range) noexcept;

    /// \brief Create a new range by element-wise joining different ranges and
    ///        flattening zip-ranges on the first level.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    Zip(Immutable<TRanges>... ranges) noexcept;

    /// \brief
    /// Access the VIndex-th range in a zip-range.
    ///
    /// \remarks
    /// The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief
    /// Access the VIndex-th range in a zip-range.
    ///
    /// \remarks
    /// The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief
    /// Access the VIndex-th range in a zip-range.
    ///
    /// \remarks
    /// The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<ZipRange<TRanges...>> range) noexcept;

    /// \brief
    /// Access the VIndex-th range in a zip-range.
    ///
    /// \remarks
    /// The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<ZipRange<TRanges...>> range) noexcept;

}

// ===========================================================================

namespace Syntropy::Tuples::Templates
{
    /************************************************************************/
    /* NTUPLE TRAITS                                                        */
    /************************************************************************/

    /// \brief Partial template specialization for tuples.
    template <Int VIndex, Syntropy::Ranges::Concepts::ForwardRange... TRanges>
    struct ElementTypeTraits<VIndex,
                             Syntropy::Ranges::ZipRange<TRanges...>>
        : Syntropy::Templates::Alias<ElementType<VIndex,
                                     Tuples::Tuple<TRanges...>>> {};

    /// \brief Partial template specialization for tuples.
    template <Syntropy::Ranges::Concepts::ForwardRange... TRanges>
    struct RankTypeTraits<Syntropy::Ranges::ZipRange<TRanges...>>
        : Syntropy::Templates::IntConstant<sizeof...(TRanges)> {};
}

// ===========================================================================

#include "zip_range.inl"

// ===========================================================================
