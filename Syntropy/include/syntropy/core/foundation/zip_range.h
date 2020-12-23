
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

namespace Syntropy
{
    /************************************************************************/
    /* ZIP RANGE                                                            */
    /************************************************************************/

    /// \brief Adapter class used to zip two or more ranges together.
    /// The new range has elements equal to the Tuple consisting of each element in each source range, tied together.
    /// \author Raffaele D. Facendola - November 2020.
    template <Concepts::Range... TRanges>
    class ZipRange
    {
        // N-tuple.

        template <Int VIndex, Concepts::Range... TRanges>
        friend constexpr Immutable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immutable<ZipRange<TRanges...>> range) noexcept;
                                   
        template <Int VIndex, Concepts::Range... TRanges>
        friend constexpr Mutable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Mutable<ZipRange<TRanges...>> range) noexcept;

        template <Int VIndex, Concepts::Range... TRanges>
        friend constexpr Immovable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immovable<ZipRange<TRanges...>> range) noexcept;

        template <Int VIndex, Concepts::Range... TRanges>
        friend constexpr Movable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Movable<ZipRange<TRanges...>> range) noexcept;

        // Forward range.

        template <Concepts::ForwardRange... TRanges, Concepts::ForwardRange... URanges>
        friend constexpr Bool operator==(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept;

        template <Concepts::ForwardRange... TRanges>
        friend constexpr Tuple<Templates::RangeElementReference<TRanges>...> Front(Immutable<ZipRange<TRanges...>> range) noexcept;

        template <Concepts::ForwardRange... TRanges>
        friend constexpr ZipRange<TRanges...> PopFront(Immutable<ZipRange<TRanges...>> range) noexcept;

        template <Concepts::ForwardRange... TRanges>
        friend constexpr Bool IsEmpty(Immutable<ZipRange<TRanges...>> range) noexcept;

        // Sized range.

        template <Concepts::SizedRange... TRanges>
        friend constexpr Templates::RangeElementCount<ZipRange<TRanges...>> Count(Immutable<ZipRange<TRanges...>> range) noexcept;

        // Bidirectional range.

        template <Concepts::BidirectionalRange... TRanges>
        friend constexpr Tuple<Templates::RangeElementReference<TRanges>...> Back(Immutable<ZipRange<TRanges...>> range) noexcept;

        template <Concepts::BidirectionalRange... TRanges>
        friend constexpr ZipRange<TRanges...> PopBack(Immutable<ZipRange<TRanges...>> range) noexcept;

        // Random access range.

        template <Concepts::RandomAccessRange... TRanges>
        friend constexpr ZipRange<TRanges...> Select(Immutable<ZipRange<TRanges...>> range, Templates::RangeElementCount<ZipRange<TRanges...>> offset, Templates::RangeElementCount<ZipRange<TRanges...>> count) noexcept;

        template <Concepts::RandomAccessRange... TRanges>
        friend constexpr Tuple<Templates::RangeElementReference<TRanges>...> Select(Immutable<ZipRange<TRanges...>> range, Templates::RangeElementCount<ZipRange<TRanges...>> index) noexcept;

        // Contiguous range.

        template <Concepts::ContiguousRange... TRanges>
        friend constexpr Tuple<Templates::RangeElementPointer<TRanges>...> Data(Immutable<ZipRange<TRanges...>> range) noexcept;

    public:

        /// \brief Create an empty zip-range.
        constexpr ZipRange() noexcept = default;

        /// \brief Create a new range by zipping together one or more ranges.
        constexpr ZipRange(Immutable<TRanges>... ranges) noexcept;

    private:

        /// \brief Underlying ranges.
        Tuple<TRanges...> ranges_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::ForwardRange... TRanges, Concepts::ForwardRange... URanges>
    constexpr Bool operator==(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept;

    /// \brief Compare two zip-ranges lexicographically.
    template <Concepts::ForwardRange... TRanges, Concepts::ForwardRange... URanges>
    constexpr Ordering operator<=>(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept;

    // Zip-range element access.
    // =========================

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::Range... TRanges>
    constexpr Immutable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::Range... TRanges>
    constexpr Mutable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Mutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::Range... TRanges>
    constexpr Immovable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immovable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Access the VIndex-th range in a zip-range.
    /// \remarks The program is ill-formed if no such range exists.
    template <Int VIndex, Concepts::Range... TRanges>
    constexpr Movable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Movable<ZipRange<TRanges...>> range) noexcept;

    // Forward range.
    // ==============

    /// \brief Access the first element in a zip-range.
    /// \remarks Accessing the first element of an empty zip-range results in undefined behavior.
    template <Concepts::ForwardRange... TRanges>
    constexpr Tuple<Templates::RangeElementReference<TRanges>...> Front(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Discard the first count elements in a zip-range and return the resulting zip-subrange.
    /// \remarks If the provided range is empty, the behavior of this method is undefined.
    template <Concepts::ForwardRange... TRanges>
    constexpr ZipRange<TRanges...> PopFront(Immutable<ZipRange<TRanges...>> range) noexcept;

    template <Concepts::ForwardRange... TRanges>
    constexpr Bool IsEmpty(Immutable<ZipRange<TRanges...>> range) noexcept;

    // Sized range.
    // ============

    /// \brief Get the number of elements in a range.
    template <Concepts::SizedRange... TRanges>
    constexpr Templates::RangeElementCount<ZipRange<TRanges...>> Count(Immutable<ZipRange<TRanges...>> range) noexcept;

    // Bidirectional range.
    // ====================

    /// \brief Access the last element in a range.
    /// \remarks Accessing the last element of an empty range results in undefined behavior.
    template <Concepts::BidirectionalRange... TRanges>
    constexpr Tuple<Templates::RangeElementReference<TRanges>...> Back(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Discard the last count elements in a range and return the resulting subrange.
    /// \remarks If this method would cause the subrange to exceed the original range, the behavior of this method is undefined.
    template <Concepts::BidirectionalRange... TRanges>
    constexpr ZipRange<TRanges...> PopBack(Immutable<ZipRange<TRanges...>> range) noexcept;

    // Random access range.
    // ====================

    /// \brief Obtain a sub-range given an offset and a number of elements.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange... TRanges>
    constexpr ZipRange<TRanges...> Select(Immutable<ZipRange<TRanges...>> range, Templates::RangeElementCount<ZipRange<TRanges...>> offset, Templates::RangeElementCount<ZipRange<TRanges...>> count) noexcept;

    /// \brief Obtain a range element at given index.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange... TRanges>
    constexpr Tuple<Templates::RangeElementReference<TRanges>...> Select(Immutable<ZipRange<TRanges...>> range, Templates::RangeElementCount<ZipRange<TRanges...>> index) noexcept;

    // Contiguous range.
    // =================

    /// \brief Access underlying range data.
    /// \remarks Accessing data of an empty range is allowed but the returned value is unspecified.
    template <Concepts::ContiguousRange... TRanges>
    constexpr Tuple<Templates::RangeElementPointer<TRanges>...> Data(Immutable<ZipRange<TRanges...>> range) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new range by element-wise joining source ranges.
    /// \remarks The smallest range determines the size of the result.
    template <Concepts::Range... TRanges>
    constexpr ZipRange<TRanges...> Zip(Immutable<TRanges>... ranges) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for zip-ranges.
    template <Concepts::Range... TRanges>
    struct RangeEnableTypeTraits<ZipRange<TRanges...>> : Alias<void> {};

    /// \brief Specialization for zip-ranges.
    template <Concepts::Range... TRanges>
    struct RangeElementReferenceTypeTraits<ZipRange<TRanges...>> : Alias<Tuple<RangeElementReference<TRanges>...>> {};

    /// \brief Specialization for zip-ranges.
    template <Concepts::Range... TRanges>
    struct RangeElementPointerTypeTraits<ZipRange<TRanges...>> : Alias<Tuple<RangeElementPointer<TRanges>...>> {};

    /// \brief Specialization for zip-ranges.
    template <Concepts::Range... TRanges>
    struct RangeElementCountTypeTraits<ZipRange<TRanges...>> : Alias<CommonType<RangeElementCount<TRanges>...>> {};

    /************************************************************************/
    /* NTUPLE TRAITS                                                        */
    /************************************************************************/

    /// \brief Partial template specialization for tuples.
    template <Int VIndex, Concepts::Range... TRanges>
    struct TupleElementTypeTraits<VIndex, ZipRange<TRanges...>> : Alias<TupleElementType<VIndex, ZipRange<TRanges...>>> {};

    /// \brief Partial template specialization for tuples.
    template <Concepts::Range... TRanges>
    struct TupleRankTypeTraits<ZipRange<TRanges...>> : IntConstant<sizeof...(TRanges)> {};
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ZipRange.
    // =========

    template <Concepts::Range... TRanges>
    constexpr ZipRange<TRanges...>::ZipRange(Immutable<TRanges>... ranges) noexcept
        : ranges_{ ranges... }
    {

    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <Concepts::ForwardRange... TRanges, Concepts::ForwardRange... URanges>
    constexpr Bool operator==(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept
    {
        using namespace Ranges;

        return AreEquivalent(lhs, rhs);
    }

    template <Concepts::ForwardRange... TRanges, Concepts::ForwardRange... URanges>
    constexpr Ordering operator<=>(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept
    {
        using namespace Ranges;

        return Compare(lhs, rhs);
    }

    // Tuple-like.

    template <Int VIndex, Concepts::Range... TRanges>
    constexpr Immutable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        return Get<VIndex>(range.ranges_);
    }

    template <Int VIndex, Concepts::Range... TRanges>
    constexpr Mutable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Mutable<ZipRange<TRanges...>> range) noexcept
    {
        return Get<VIndex>(range.ranges_);
    }

    template <Int VIndex, Concepts::Range... TRanges>
    constexpr Immovable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Immovable<ZipRange<TRanges...>> range) noexcept
    {
        return Get<VIndex>(range.ranges_);
    }

    template <Int VIndex, Concepts::Range... TRanges>
    constexpr Movable<Templates::TupleElementType<VIndex, ZipRange<TRanges...>>> Get(Movable<ZipRange<TRanges...>> range) noexcept
    {
        return Get<VIndex>(range.ranges_);
    }

    // Forward range.

    template <Concepts::ForwardRange... TRanges>
    constexpr Tuple<Templates::RangeElementReference<TRanges>...> Front(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_front = [](Immutable<TRanges>... ranges)
        {
            return Tuple<Templates::RangeElementReference<TRanges>...>{ Front(ranges)... };
        };

        return Tuples::Apply(zip_front, range.ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    constexpr ZipRange<TRanges...> PopFront(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_pop_front = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ PopFront(ranges)... };
        };

        return Tuples::Apply(zip_pop_front, range.ranges_);
    }

    template <Concepts::ForwardRange... TRanges>
    constexpr Bool IsEmpty(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_is_empty = [](Immutable<TRanges>... ranges)
        {
            return (sizeof...(TRanges) == 0) || (IsEmpty(ranges) || ...);
        };

        return Tuples::Apply(zip_is_empty, range.ranges_);
    }

    // Sized range.

    template <Concepts::SizedRange... TRanges>
    constexpr Templates::RangeElementCount<ZipRange<TRanges...>> Count(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_min_count = [](Immutable<TRanges>... ranges)
        {
            return Math::Min(static_cast<Templates::RangeElementCount<ZipRange<TRanges...>>>(Count(ranges))...);
        };

        return Tuples::Apply(zip_min_count, range.ranges_);
    }

    // Bidirectional range.

    template <Concepts::BidirectionalRange... TRanges>
    constexpr Tuple<Templates::RangeElementReference<TRanges>...> Back(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_back = [](Immutable<TRanges>... ranges)
        {
            return Tuple<Templates::RangeElementReference<TRanges>...>{ Back(ranges)... };
        };

        return Tuples::Apply(zip_back, range.ranges_);
    }

    template <Concepts::BidirectionalRange... TRanges>
    constexpr ZipRange<TRanges...> PopBack(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_pop_back = [](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ PopBack(ranges)... };
        };

        return Tuples::Apply(zip_pop_back, range.ranges_);
    }

    // Random access range.

    template <Concepts::RandomAccessRange... TRanges>
    constexpr ZipRange<TRanges...> Select(Immutable<ZipRange<TRanges...>> range, Templates::RangeElementCount<ZipRange<TRanges...>> offset, Templates::RangeElementCount<ZipRange<TRanges...>> count) noexcept
    {
        auto zip_select = [offset, count](Immutable<TRanges>... ranges)
        {
            return ZipRange<TRanges...>{ Select(ranges, offset, count)... };
        };

        return Tuples::Apply(zip_select, range.ranges_);
    }

    template <Concepts::RandomAccessRange... TRanges>
    constexpr Tuple<Templates::RangeElementReference<TRanges>...> Select(Immutable<ZipRange<TRanges...>> range, Templates::RangeElementCount<ZipRange<TRanges...>> index) noexcept
    {
        auto zip_select = [index](Immutable<TRanges>... ranges)
        {
            return Tuple<Templates::RangeElementReference<TRanges>...>{ Select(ranges, index)... };
        };

        return Tuples::Apply(zip_select, range.ranges_);
    }

    // Contiguous range.

    template <Concepts::ContiguousRange... TRanges>
    constexpr Tuple<Templates::RangeElementPointer<TRanges>...> Data(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_data = [](Immutable<TRanges>... ranges)
        {
            return Tuple<Templates::RangeElementPointer<TRanges>...>{ Data(ranges)... };
        };

        return Tuples::Apply(zip_data, range.ranges_);
    }

    // Utilities.

    template <Concepts::Range... TRanges>
    constexpr ZipRange<TRanges...> Zip(Immutable<TRanges>... ranges) noexcept
    {
        return ZipRange{ ranges... };
    }

}

// ===========================================================================
