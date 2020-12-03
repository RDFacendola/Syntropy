
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
    public:

        /// \brief Create a new range by zipping together one or more ranges.
        constexpr ZipRange(Ref<TRanges>... ranges) noexcept;

        /// \brief Access the zipped ranges.
        constexpr Ref<TupleT<TRanges...>> GetRanges() const noexcept;

    private:

        /// \brief Underlying ranges.
        TupleT<TRanges...> ranges_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Range.
    // ======

    /// \brief Create a new range by joining together multiple ranges.
    /// The range is considered to be non-empty until at least one of the ranges becomes empty.
    template <Concepts::RangeT... TRanges>
    ZipRange<TRanges...> Zip(Ref<TRanges>... ranges) noexcept;

    // Forward range.
    // ==============

    /// \brief Access the first element in a zip-range.
    /// \remarks Accessing the first element of an empty zip-range results in undefined behavior.
    template <Concepts::ForwardRangeT... TRanges>
    constexpr TupleT<Templates::RangeElementReferenceType<TRanges>...> Front(Ref<ZipRange<TRanges...>> zip_range) noexcept;

    /// \brief Access the first element in each range and zip the result in a tuple,
    /// \remarks Accessing the first element when any of the provided ranges is empty results results in undefined behavior.
    template <Concepts::ForwardRangeT... TRanges>
    constexpr TupleT<Templates::RangeElementReferenceType<TRanges>...> ZipFront(Ref<TRanges>... ranges) noexcept;

    /// \brief Discard the first count elements in a zip-range and return the resulting sub-zip-range.
    /// \remarks If this method would cause the sub-zip-range to exceed the original zip-range, the behavior of this method is undefined.
    template <Concepts::ForwardRangeT... TRanges>
    constexpr ZipRange<TRanges...> PopFront(Ref<ZipRange<TRanges...>> zip_range) noexcept;

    /// \brief Check whether a zip-range is empty.
    /// \return Returns true if the zip-range is empty, returns false otherwise.
    template <Concepts::ForwardRangeT... TRanges>
    constexpr Bool IsEmpty(Ref<ZipRange<TRanges...>> zip_range) noexcept;

    // Bidirectional range.
    // ====================

    /// \brief Access the last element in a zip-range.
    /// \remarks Accessing the last element of an empty zip-range results in undefined behavior.
    template <Concepts::BidirectionalRangeT... TRanges>
    constexpr MutableRef<TupleT<TRanges...>> Back(Ref<ZipRange<TRanges...>> zip_range) noexcept;

    /// \brief Discard the last count elements in a zip-range and return the resulting sub-zip-range.
    /// \remarks If this method would cause the sub-zip-range to exceed the original zip-range, the behavior of this method is undefined.
    template <Concepts::BidirectionalRangeT... TRanges>
    constexpr ZipRange<TRanges...> PopBack(Ref<ZipRange<TRanges...>> zip_range) noexcept;

    // Sized range.
    // ============

    /// \brief Get the number of elements in a zip-range.
    template <Concepts::SizedRangeT... TRanges>
    constexpr Int Count(Ref<ZipRange<TRanges...>> zip_range) noexcept;

    // Random access range.
    // ====================

    /// \brief Obtain a sub-zip-range given an offset and a number of elements.
    /// \remarks Exceeding zip-range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeT... TRanges>
    constexpr ZipRange<TRanges...> Select(Ref<ZipRange<TRanges...>> zip_range, Int offset, Int count) noexcept;

    /// \brief Obtain a zip-range element at given index.
    /// \remarks Exceeding zip-range boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeT... TRanges>
    constexpr TupleT<TRanges...>& Select(Ref<ZipRange<TRanges...>> zip_range, Int index) noexcept;

    // Contiguous range.
    // =================

    /// \brief Access underlying zip-range data.
    /// \remarks Accessing data of an empty zip-range is allowed but the returned value is unspecified.
    template <Concepts::ContiguousRangeT... TRanges>
    constexpr TupleT<TRanges*...> Data(Ref<ZipRange<TRanges...>> zip_range) noexcept;

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
        using ElementReferenceType = TupleT<RangeElementReferenceType<TRanges>...>;
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
    inline constexpr ZipRange<TRanges...>::ZipRange(Ref<TRanges>... ranges) noexcept
        : ranges_(ranges...)
    {

    }

    template <Concepts::RangeT... TRanges>
    constexpr const TupleT<TRanges...>& ZipRange<TRanges...>::GetRanges() const noexcept
    {
        return ranges_;
    }

    // Non-member functions.
    // =====================

    // Range.

    template <Concepts::RangeT... TRanges>
    ZipRange<TRanges...> Zip(Ref<TRanges>... ranges) noexcept
    {
        return ZipRange{ ranges... };
    }

    // Forward range.

    template <Concepts::ForwardRangeT... TRanges>
    constexpr TupleT<Templates::RangeElementReferenceType<TRanges>...> Front(Ref<ZipRange<TRanges...>> zip_range) noexcept
    {
        auto& ranges = zip_range.GetRanges();

        return Apply(ZipFront<TRanges...>, ranges);
    }

    template <Concepts::ForwardRangeT... TRanges>
    constexpr TupleT<Templates::RangeElementReferenceType<TRanges>...> ZipFront(Ref<TRanges>... ranges) noexcept
    {
        return { Front(ranges)... };
    }

}

// ===========================================================================