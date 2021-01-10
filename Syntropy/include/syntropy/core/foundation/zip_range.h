
/// \file zip_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for adapters used to zip multiple ranges together.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/sequence.h"
#include "syntropy/language/support/compare.h"

#include "syntropy/core/concepts/range.h"
#include "syntropy/core/concepts/ntuple.h"
#include "syntropy/core/foundation/tuple.h"

#include "syntropy/math/math.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* ZIP RANGE                                                            */
    /************************************************************************/

    /// \brief Adapter class used to zip two or more ranges together.
    /// The new range has elements equal to the Tuple consisting of each element in each source range, tied together.
    /// \author Raffaele D. Facendola - November 2020.
    template <Concepts::ForwardRange... TRanges>
    class ZipRange
    {
        template <Concepts::ForwardRange... TRanges>
        friend constexpr Immutable<Tuples::Tuple<TRanges...>> Unzip(Immutable<ZipRange<TRanges...>> range) noexcept;

    public:

        /// \brief Create an empty zip-range.
        constexpr ZipRange() noexcept = default;

        /// \brief Create a new range by zipping together one or more ranges.
        template <typename = Templates::EnableIf<(sizeof...(TRanges) > 0)>>
        constexpr ZipRange(Immutable<TRanges>... ranges) noexcept;

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

    // Zip-range element access.
    // =========================

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Immutable<Syntropy::Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Mutable<Syntropy::Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Mutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Immovable<Syntropy::Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immovable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Movable<Syntropy::Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Movable<ZipRange<TRanges...>> range) noexcept;

    // Forward range.
    // ==============

    /// \brief Access the first element in a zip-range.
    /// \remarks Accessing the first element of an empty zip-range results in undefined behavior.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::ElementReference<TRanges>...> Front(Immutable<ZipRange<TRanges...>> range) noexcept;
    
    /// \brief Discard the first count elements in a zip-range and return the resulting zip-subrange.
    /// \remarks If the provided range is empty, the behavior of this method is undefined.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> PopFront(Immutable<ZipRange<TRanges...>> range) noexcept;

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Bool IsEmpty(Immutable<ZipRange<TRanges...>> range) noexcept;

    // Sized range.
    // ============

    /// \brief Get the number of elements in a range.
    template <Concepts::SizedRange... TRanges>
    [[nodiscard]] constexpr Templates::ElementCount<ZipRange<TRanges...>> Count(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::SizedRange... TRanges, Concepts::SizedRange... URanges>
    [[nodiscard]] constexpr Bool AreEquivalent(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept;

    /// \brief Compare two zip-ranges lexicographically.
    template <Concepts::SizedRange... TRanges, Concepts::SizedRange... URanges>
    [[nodiscard]] constexpr Ordering Compare(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept;

    // Bidirectional range.
    // ====================

    /// \brief Access the last element in a range.
    /// \remarks Accessing the last element of an empty range results in undefined behavior.
    template <Concepts::BidirectionalRange... TRanges>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::ElementReference<TRanges>...> Back(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Discard the last count elements in a range and return the resulting subrange.
    /// \remarks If this method would cause the subrange to exceed the original range, the behavior of this method is undefined.
    template <Concepts::BidirectionalRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> PopBack(Immutable<ZipRange<TRanges...>> range) noexcept;

    // Random access range.
    // ====================

    /// \brief Obtain a sub-range given an offset and a number of elements.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> Select(Immutable<ZipRange<TRanges...>> range, Templates::ElementCount<ZipRange<TRanges...>> offset, Templates::ElementCount<ZipRange<TRanges...>> count) noexcept;

    /// \brief Obtain a range element at given index.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange... TRanges>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::ElementReference<TRanges>...> Select(Immutable<ZipRange<TRanges...>> range, Templates::ElementCount<ZipRange<TRanges...>> index) noexcept;

    // Contiguous range.
    // =================

    /// \brief Access underlying range data.
    /// \remarks Accessing data of an empty range is allowed but the returned value is unspecified.
    template <Concepts::ContiguousRange... TRanges>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::ElementPointer<TRanges>...> Data(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::ContiguousRange... TRanges, Concepts::ContiguousRange... URanges>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new ZipRange by deducing templates types from arguments.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> MakeZipRange(Immutable<TRanges>... ranges) noexcept;

    /// \brief Create a new ZipRange by deducing templates types from arguments provided
    template <Syntropy::Concepts::NTuple TTuple>
    [[nodiscard]] constexpr auto MakeZipRangeFromTuple(Immutable<TTuple> ranges) noexcept;

    /// \brief Create a new range by element-wise joining different ranges and flattening zip-ranges on the first level.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto Zip(Immutable<TRanges>... ranges) noexcept;

    /// \brief Access the individual ranges in a zip range.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Immutable<Tuples::Tuple<TRanges...>> Unzip(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access a rage in a zip-range.
    /// This overload is provided for symmetry purposes.
    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange> Unzip(Immutable<TRange> range) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for zip-ranges.
    template <Concepts::ForwardRange... TRanges>
    struct ElementReferenceTypeTraits<Ranges::ZipRange<TRanges...>> : Syntropy::Templates::Alias<Tuples::Tuple<ElementReference<TRanges>...>> {};

    /// \brief Specialization for zip-ranges.
    template <Concepts::ForwardRange... TRanges>
    struct ElementPointerTypeTraits<Ranges::ZipRange<TRanges...>> : Syntropy::Templates::Alias<Tuples::Tuple<ElementPointer<TRanges>...>> {};

    /// \brief Specialization for zip-ranges.
    template <Concepts::ForwardRange... TRanges>
    struct ElementCountTypeTraits<Ranges::ZipRange<TRanges...>> : Syntropy::Templates::Alias<Syntropy::Templates::CommonType<ElementCount<TRanges>...>> {};

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* NTUPLE TRAITS                                                        */
    /************************************************************************/

/// \brief Partial template specialization for tuples.
    template <Int VIndex, Ranges::Concepts::ForwardRange... TRanges>
    struct TupleElementTypeTraits<VIndex, Ranges::ZipRange<TRanges...>> : Alias<TupleElementType<VIndex, Tuples::Tuple<TRanges...>>> {};

    /// \brief Partial template specialization for tuples.
    template <Ranges::Concepts::ForwardRange... TRanges>
    struct TupleRankTypeTraits<Ranges::ZipRange<TRanges...>> : IntConstant<sizeof...(TRanges)> {};
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
    template <typename>
    constexpr ZipRange<TRanges...>::ZipRange(Immutable<TRanges>... ranges) noexcept
        : ranges_{ ranges... }
    {

    }

    // Non-member functions.
    // =====================

    // Comparison.



    // Tuple-like.

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Immutable<Syntropy::Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        return Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Mutable<Syntropy::Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Mutable<ZipRange<TRanges...>> range) noexcept
    {
        return Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Immovable<Syntropy::Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immovable<ZipRange<TRanges...>> range) noexcept
    {
        return Get<VIndex>(Unzip(range));
    }

    template <Int VIndex, Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Movable<Syntropy::Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Movable<ZipRange<TRanges...>> range) noexcept
    {
        return Get<VIndex>(Unzip(range));
    }

    // Forward range.

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::ElementReference<TRanges>...> Front(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_front = [](Immutable<TRanges>... ranges)
        {
            return MakeTuple(Front(ranges)...);
        };

        return Tuples::Apply(zip_front, Unzip(range));
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> PopFront(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_pop_front = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ PopFront(ranges)... };
        };

        return Tuples::Apply(zip_pop_front, Unzip(range));
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Bool IsEmpty(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_is_empty = [](Immutable<TRanges>... ranges)
        {
            return (sizeof...(TRanges) == 0) || (IsEmpty(ranges) || ...);
        };

        return Tuples::Apply(zip_is_empty, Unzip(range));
    }

    // Sized range.

    template <Concepts::SizedRange... TRanges>
    [[nodiscard]] constexpr Templates::ElementCount<ZipRange<TRanges...>> Count(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_min_count = [](Immutable<TRanges>... ranges)
        {
            return Math::Min(static_cast<Templates::ElementCount<ZipRange<TRanges...>>>(Count(ranges))...);
        };

        return Tuples::Apply(zip_min_count, Unzip(range));
    }

    template <Concepts::SizedRange... TRanges, Concepts::SizedRange... URanges>
    [[nodiscard]] constexpr Bool AreEquivalent(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept
    {
        return AreEquivalent(Unzip(lhs), Unzip(rhs));
    }

    template <Concepts::SizedRange... TRanges, Concepts::SizedRange... URanges>
    [[nodiscard]] constexpr Bool Compare(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept
    {
        return Compare(Unzip(rhs), Unzip(lhs));
    }

    // Bidirectional range.

    template <Concepts::BidirectionalRange... TRanges>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::ElementReference<TRanges>...> Back(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_back = [](Immutable<TRanges>... ranges)
        {
            return Tuples::Tuple<Templates::ElementReference<TRanges>...>{ Back(ranges)... };
        };

        return Tuples::Apply(zip_back, Unzip(range));
    }

    template <Concepts::BidirectionalRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> PopBack(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_pop_back = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ PopBack(ranges)... };
        };

        return Tuples::Apply(zip_pop_back, Unzip(range));
    }

    // Random access range.

    template <Concepts::RandomAccessRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> Select(Immutable<ZipRange<TRanges...>> range, Templates::ElementCount<ZipRange<TRanges...>> offset, Templates::ElementCount<ZipRange<TRanges...>> count) noexcept
    {
        auto zip_select = [offset, count](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ Select(ranges, offset, count)... };
        };

        return Tuples::Apply(zip_select, Unzip(range));
    }

    template <Concepts::RandomAccessRange... TRanges>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::ElementReference<TRanges>...> Select(Immutable<ZipRange<TRanges...>> range, Templates::ElementCount<ZipRange<TRanges...>> index) noexcept
    {
        auto zip_select = [index](Immutable<TRanges>... ranges)
        {
            return Tuples::Tuple<Templates::ElementReference<TRanges>...>{ Select(ranges, index)... };
        };

        return Tuples::Apply(zip_select, Unzip(range));
    }

    // Contiguous range.

    template <Concepts::ContiguousRange... TRanges>
    [[nodiscard]] constexpr Tuples::Tuple<Templates::ElementPointer<TRanges>...> Data(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_data = [](Immutable<TRanges>... ranges)
        {
            return Tuples::Tuple<Templates::ElementPointer<TRanges>...>{ Data(ranges)... };
        };

        return Tuples::Apply(zip_data, Unzip(range));
    }

    template <Concepts::ContiguousRange... TRanges, Concepts::ContiguousRange... URanges>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept
    {
        return AreEqual(Unzip(lhs), Unzip(rhs));
    }

    // Utilities.

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr ZipRange<TRanges...> MakeZipRange(Immutable<TRanges>... ranges) noexcept
    {
        return { ranges... };
    }

    template <Syntropy::Concepts::NTuple TTuple>
    [[nodiscard]] constexpr auto MakeZipRangeFromTuple(Immutable<TTuple> ranges) noexcept
    {
        auto make_zip_range = [&]<Int... TIndex>(Syntropy::Templates::Sequence<TIndex...>)
        {
            return ZipRange(Get<TIndex>(ranges)...);
        };

        return make_zip_range(Syntropy::Templates::TupleSequenceFor<decltype(ranges)>{});
    }

    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr auto Zip(Immutable<TRanges>... ranges) noexcept
    {
        return MakeZipRangeFromTuple(Concatenate(Unzip(ranges)...));
    }

    /// \brief Access the individual ranges in a zip range.
    template <Concepts::ForwardRange... TRanges>
    [[nodiscard]] constexpr Immutable<Tuples::Tuple<TRanges...>> Unzip(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        return range.ranges_;
    }

    /// This overload is provided for symmetry purposes.
    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange> Unzip(Immutable<TRange> range) noexcept
    {
        return { range };
    }

}

// ===========================================================================
