
/// \file zip_range.h
///
/// \brief This header is part of the Syntropy core module. It contains
///        definitions for adapters used to zip multiple ranges together.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/records/record.h"
#include "syntropy/core/records/tuple.h"
#include "syntropy/core/ranges/forward_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* ZIP RANGE                                                            */
    /************************************************************************/

    /// \brief Adapter class used to tie many ranges together.
    ///
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

        /// \brief Check whether the range is empty.
        [[nodiscard]] constexpr Bool
        IsEmpty() const noexcept;

        /// \brief Get the number of element.
        [[nodiscard]] constexpr auto
        GetCount() const noexcept;

        /// \brief Access the first element in the range.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr decltype(auto)
        GetFront() const noexcept;

        /// \brief Access the last element in the range.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr decltype(auto)
        GetBack() const noexcept;


        /// \brief Discard the first element in the range and return a range to
        ///        the remaining elements.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr auto
        PopFront() const noexcept;




        /// \brief Discard the last element in the range and return the range
        ///        to the remaining elements.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr auto
        PopBack() const noexcept;

        /// \brief Access a range element by index.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <typename TIndex>
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<TIndex> index) const noexcept;

        /// \brief Access the storage of all tied ranges.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr auto
        GetData() const noexcept;

    private:

        /// \brief Underlying ranges.
        Records::Tuple<TRanges...> ranges_;

    };

    /// \brief Deduction guides for ZipRange.
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
    ///        provided arguments.
    template <Syntropy::Records::Record TTuple>
    [[nodiscard]] constexpr auto
    MakeZipRangeFromTuple(Immutable<TTuple> ranges) noexcept;

    /// \brief Unzip a zip range, producing a tuple containing the
    ///        individual ranges.
    ///
    /// \remarks If the provided range is not a ZipRange, the results is a
    ///          tuple with a single element.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Unzip a zip range, producing a tuple containing the individual
    ///        ranges.
    ///
    /// \remarks If the provided range is not a ZipRange, the results is a
    ///          tuple with a single element.
    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range) noexcept;

    /// \brief Create a new ZipRange by element-wise joining different ranges,
    ///        flattening eventual ZipRanges on the first level.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    Zip(Immutable<TRanges>... ranges) noexcept;

    /// \brief Access a zip range by index.
    ///
    /// \remarks Ill-formed if the range rank is exceeded.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access a zip range by index.
    ///
    /// \remarks Ill-formed if the range rank is exceeded.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access a zip range by index.
    ///
    /// \remarks Ill-formed if the range rank is exceeded.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access a zip range by index.
    ///
    /// \remarks Ill-formed if the range rank is exceeded.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<ZipRange<TRanges...>> range) noexcept;

}

// ===========================================================================

namespace Syntropy::Records
{
    /************************************************************************/
    /* NTUPLE TRAITS                                                        */
    /************************************************************************/

    /// \brief Partial template specialization for tuples.
    template <Syntropy::Ranges::Concepts::ForwardRange... TRanges>
    struct RankTrait<Syntropy::Ranges::ZipRange<TRanges...>>
        : Syntropy::Templates::IntConstant<sizeof...(TRanges)> {};
}

// ===========================================================================

#include "details/zip_range.inl"

// ===========================================================================
