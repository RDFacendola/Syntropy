
/// \file zip_range_details.h
///
/// \brief This header is part of the Syntropy core module.
/// It contains implementation details about zip-ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/concepts/forward_range.h"
#include "syntropy/core/concepts/sized_range.h"
#include "syntropy/core/concepts/bidirectional_range.h"
#include "syntropy/core/concepts/random_access_range.h"
#include "syntropy/core/concepts/contiguous_range.h"

#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Ranges::Details
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
    template <Ranges::Concepts::ForwardRange... TRanges>
    class ZipRange
    {
        template <Ranges::Concepts::ForwardRange... TRanges>
        friend constexpr auto
        Unzip(Immutable<ZipRange<TRanges...>> range) noexcept;

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
        template <typename URange = TRange>
        requires Ranges::Concepts::ContiguousRange<URange>
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

    // Utilities.
    // ==========

    /// \brief Create a new ZipRange by deducing templates types
    ///        from arguments.
    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    MakeZipRange(Immutable<TRanges>... ranges) noexcept;

    /// \brief Create a new ZipRange by deducing templates types from
    ///        arguments provided
    template <Syntropy::Tuples::Concepts::NTuple TTuple>
    [[nodiscard]] constexpr auto
    MakeZipRangeFromTuple(Immutable<TTuple> ranges) noexcept;

    /// \brief Access the individual ranges in a zip range.
    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access a range in a zip-range.
    /// This overload is provided for symmetry purposes.
    template <Ranges::Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range) noexcept;

    /// \brief Create a new range by element-wise joining different ranges
    ///        and flattening zip-ranges on the first level.
    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    Zip(Immutable<TRanges>... ranges) noexcept;
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
                             Syntropy::Ranges::Details::ZipRange<TRanges...>>
        : Syntropy::Templates::Alias<ElementType<VIndex,
                                     Tuples::Tuple<TRanges...>>> {};

    /// \brief Partial template specialization for tuples.
    template <Syntropy::Ranges::Concepts::ForwardRange... TRanges>
    struct RankTypeTraits<Syntropy::Ranges::Details::ZipRange<TRanges...>>
        : Syntropy::Templates::IntConstant<sizeof...(TRanges)> {};
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ZipRange.
    // =========

    template <Ranges::Concepts::ForwardRange... TRanges>
    constexpr ZipRange<TRanges...>
    ::ZipRange(Immutable<TRanges>... ranges) noexcept
        : ranges_{ ranges... }
    {

    }

    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRanges...>
    ::GetFront() const noexcept
    {
        auto zip_front = [](Immutable<TRanges>... ranges)
        {
            return Tuples::MakeTuple(RouteFront(ranges)...);
        };

        return Tuples::Apply(zip_front, ranges_);
    }

    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::PopFront() const noexcept
    {
        auto zip_pop_front = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ RoutePopFront(ranges)... };
        };

        return Tuples::Apply(zip_pop_front, ranges_);
    }

    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Bool ZipRange<TRanges...>
    ::IsEmpty() const noexcept
    {
        auto zip_is_empty = [](Immutable<TRanges>... ranges)
        {
            return (sizeof...(TRanges) == 0) || (RouteIsEmpty(ranges) || ...);
        };

        return Tuples::Apply(zip_is_empty, ranges_);
    }

    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::GetCount() const noexcept
    {
        auto zip_min_count = [](Immutable<TRanges>... ranges)
        {
            return Math::Min(RouteCount(ranges)...);
        };

        return Tuples::Apply(zip_min_count, ranges_);
    }

    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr decltype(auto) ZipRange<TRanges...>
    ::GetBack() const noexcept
    {
        auto zip_back = [](Immutable<TRanges>... ranges)
        {
            return Tuples::MakeTuple( RouteBack(ranges)... );
        };

        return Tuples::Apply(zip_back, ranges_);
    }

    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::PopBack() const noexcept
    {
        auto zip_pop_back = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ RoutePopBack(ranges)... };
        };

        return Tuples::Apply(zip_pop_back, ranges_);
    }

    template <Ranges::Concepts::ForwardRange... TRanges>
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

    template <Ranges::Concepts::ForwardRange... TRanges>
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

    template <Ranges::Concepts::ForwardRange... TRanges>
    template <typename URange>
    requires Ranges::Concepts::ContiguousRange<URange>
    [[nodiscard]] constexpr auto ZipRange<TRanges...>
    ::GetData() const noexcept
    {
        auto zip_data = [](Immutable<TRanges>... ranges)
        {
            return Tuples::MakeTuple( RouteData(ranges)... );
        };

        return Tuples::Apply(zip_data, ranges_);
    }

    // Utilities.

    template <Ranges::Concepts::ForwardRange... TRanges>
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

    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        return range.ranges_;
    }

    template <Ranges::Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range) noexcept
    {
        return Tuples::MakeTuple(range);
    }

    template <Ranges::Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...>
    ip(Immutable<TRanges>... ranges) noexcept
    {
        return MakeZipRangeFromTuple(Tuples::Concatenate(Unzip(ranges)...));
    }
}

// ===========================================================================
