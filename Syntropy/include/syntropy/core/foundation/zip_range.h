
/// \file zip_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for adapters used to zip multiple ranges together.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/sequence.h"

#include "syntropy/core/foundation/range.h"
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
    template <Concepts::RangeT... TRanges>
    class ZipRange
    {
        // Forward range.

        template <Concepts::ForwardRangeT... TRanges, Concepts::ForwardRangeT... URanges>
        friend constexpr Bool operator==(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept;

        template <Concepts::ForwardRangeT... TRanges>
        friend constexpr Tuple<Templates::RangeElementReferenceType<TRanges>...> Front(Immutable<ZipRange<TRanges...>> range) noexcept;

        template <Concepts::ForwardRangeT... TRanges>
        friend constexpr ZipRange<TRanges...> PopFront(Immutable<ZipRange<TRanges...>> range) noexcept;

        template <Concepts::ForwardRangeT... TRanges>
        friend constexpr Bool IsEmpty(Immutable<ZipRange<TRanges...>> range) noexcept;

        // Sized range.

        template <Concepts::SizedRangeT... TRanges>
        friend constexpr Int Count(Immutable<ZipRange<TRanges...>> range) noexcept;

        // Bidirectional range.

        template <Concepts::BidirectionalRangeT... TRanges>
        friend constexpr Tuple<Templates::RangeElementReferenceType<TRanges>...> Back(Immutable<ZipRange<TRanges...>> range) noexcept;

        template <Concepts::BidirectionalRangeT... TRanges>
        friend constexpr ZipRange<TRanges...> PopBack(Immutable<ZipRange<TRanges...>> range) noexcept;

        //template <Concepts::RandomAccessRangeT TRange>
        //friend constexpr TRange Select(Immutable<TRange> rhs, Int offset, Int count) noexcept;

        //template <Concepts::RandomAccessRangeT TRange>
        //friend constexpr Templates::RangeElementReferenceType<TRange> Select(Immutable<TRange> rhs, Int index) noexcept;

        //template <Concepts::ContiguousRangeT TRange>
        //friend constexpr Templates::RangeElementPointerType<TRange> Data(Immutable<TRange> rhs) noexcept;

    public:

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

    template <Concepts::ForwardRangeT... TRanges, Concepts::ForwardRangeT... URanges>
    constexpr Bool operator==(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept;

    // Forward range.
    // ==============

    /// \brief Access the first element in a zip-range.
    /// \remarks Accessing the first element of an empty zip-range results in undefined behavior.
    template <Concepts::ForwardRangeT... TRanges>
    constexpr Tuple<Templates::RangeElementReferenceType<TRanges>...> Front(Immutable<ZipRange<TRanges...>> range) noexcept;

    /// \brief Discard the first count elements in a zip-range and return the resulting zip-subrange.
    /// \remarks If the provided range is empty, the behavior of this method is undefined.
    template <Concepts::ForwardRangeT... TRanges>
    constexpr ZipRange<TRanges...> PopFront(Immutable<ZipRange<TRanges...>> range) noexcept;

    template <Concepts::ForwardRangeT... TRanges>
    constexpr Bool IsEmpty(Immutable<ZipRange<TRanges...>> range) noexcept;

    // Sized range.
    // ============

    /// \brief Get the number of elements in a span.
    template <Concepts::SizedRangeT... TRanges>
    constexpr Int Count(Immutable<ZipRange<TRanges...>> range) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new range by element-wise joining source ranges.
    /// \remarks The smallest range determines the size of the result.
    template <Concepts::RangeT... TRanges>
    constexpr ZipRange<TRanges...> Zip(Immutable<TRanges>... ranges) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for zip-ranges.
    template <Concepts::RangeT... TRanges>
    struct RangeTraits<ZipRange<TRanges...>>
    {
        /// \brief Type of a reference to a range element.
        using ElementReferenceType = Tuple<RangeElementReferenceType<TRanges>...>;

        /// \brief Type of a pointer to a range element.
        using ElementPointerType = Tuple<RangeElementPointerType<TRanges>...>;
    };
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ZipRange.
    // =========

    template <Concepts::RangeT... TRanges>
    constexpr ZipRange<TRanges...>::ZipRange(Immutable<TRanges>... ranges) noexcept
        : ranges_{ ranges... }
    {

    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <Concepts::ForwardRangeT... TRanges, Concepts::ForwardRangeT... URanges>
    constexpr Bool operator==(Immutable<ZipRange<TRanges...>> lhs, Immutable<ZipRange<URanges...>> rhs) noexcept
    {
        return false;
    }

    // Forward range.

    template <Concepts::ForwardRangeT... TRanges>
    constexpr Tuple<Templates::RangeElementReferenceType<TRanges>...> Front(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_front = []<typename...>(const auto&... ranges)
        {
            return Tuple<Templates::RangeElementReferenceType<TRanges>...>{ Front(ranges)... };
        };

        return Apply(zip_front, range.ranges_);
    }

    template <Concepts::ForwardRangeT... TRanges>
    constexpr ZipRange<TRanges...> PopFront(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_pop_front = []<typename...>(const auto&... ranges)
        {
            return ZipRange<TRanges...>{ PopFront(ranges)... };
        };

        return Apply(zip_pop_front, range.ranges_);
    }

    template <Concepts::ForwardRangeT... TRanges>
    constexpr Bool IsEmpty(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_is_empty = []<typename...>(const auto&... ranges)
        {
            return (sizeof...(TRanges) == 0) || (IsEmpty(ranges) || ...);
        };

        return Apply(zip_is_empty, range.ranges_);
    }

    // Sized range.

    template <Concepts::SizedRangeT... TRanges>
    constexpr Int Count(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_min_count = []<typename...>(const auto&... ranges)
        {
            return Math::Min(Count(ranges)...);
        };

        return Apply(zip_min_count, range.ranges_);
    }

    // Bidirectional range.

    template <Concepts::BidirectionalRangeT... TRanges>
    constexpr Tuple<Templates::RangeElementReferenceType<TRanges>...> Back(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_back = []<typename...>(const auto&... ranges)
        {
            return Tuple<Templates::RangeElementReferenceType<TRanges>...>{ Back(ranges)... };
        };

        return Apply(zip_back, range.ranges_);
    }

    template <Concepts::BidirectionalRangeT... TRanges>
    constexpr ZipRange<TRanges...> PopBack(Immutable<ZipRange<TRanges...>> range) noexcept
    {
        auto zip_pop_back = []<typename...>(const auto&... ranges)
        {
            return ZipRange<TRanges...>{ PopBack(ranges)... };
        };

        return Apply(zip_pop_back, range.ranges_);
    }

    // Utilities.

    template <Concepts::RangeT... TRanges>
    constexpr ZipRange<TRanges...> Zip(Immutable<TRanges>... ranges) noexcept
    {
        return ZipRange{ ranges... };
    }

}

// ===========================================================================