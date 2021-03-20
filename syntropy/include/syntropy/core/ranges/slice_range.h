
/// \file random_access_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        in any order.
///
/// Ranges specifications based on the awesome
///     https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/random_access_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* SLICE RANGE                                                          */
    /************************************************************************/

    /// \brief Adapter class used to project a subsection of a random access
    ///        range.
    ///
    /// \author Raffaele D. Facendola - March 2021.
    template <Concepts::RandomAccessRangeView TRangeView>
    class SliceRange
    {
    public:

        /// \brief Type of the range view's cardinality.
        using CountType = Templates::RangeCountType<TRangeView>;

        /// \brief Default constructor.
        constexpr
        SliceRange() noexcept = default;

        template <Concepts::RandomAccessRange TRange>
        constexpr
        SliceRange(Immutable<TRange> range,
                   Immutable<CountType> offset,
                   Immutable<CountType> count) noexcept;

        /// \brief Default copy constructor.
        constexpr
        SliceRange(Immutable<SliceRange> rhs) noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<SliceRange>
        operator=(Immutable<SliceRange> rhs) noexcept = default;

        /// \brief Default destructor.
        ~SliceRange() = default;

        /// \brief Get the number of element.
        [[nodiscard]] constexpr CountType
        GetCount() const noexcept;

        /// \brief Access a range view element by index.
        /// \remarks Undefined behavior if range view boundaries are exceeded.
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<CountType> index) const noexcept;

        /// \brief Access a range elements' storage.
        ///
        /// \remarks Calling this method on an empty range results in undefined
        ///          behavior.
        template <typename TRange = SliceRange>
        [[nodiscard]] constexpr auto
        GetData() const noexcept;

    private:

        /// \brief Underlying range view.
        TRangeView range_view_;

        /// \brief Offset from the underlying range view's begin.
        CountType offset_{};

        /// \brief Number of elements in the undelying rangeg view.
        CountType count_{};
    }

    /// \brief Deduction guieds for SliceRange.
    template<typename TRangeView>
    SliceRange(Immutable<TRangeView>,
               Immutable<Templates::RangeCountType<TRangeView>>,
               Immutable<Templates::RangeCountType<TRangeView>>)
    -> SliceRange<TRangeView>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // RandomAccessRange.
    // ==================

    /// \brief Obtain a range view to a sub-range-view.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange,
              typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    Slice(Immutable<TRange> range,
          Immutable<TCount> index,
          Immutable<TCount> count) noexcept;

    /// \brief Take a number of elements from the range view's front.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange,
              typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    Front(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Take a number of elements from the range view's back.
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange,
              typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    Back(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Discard the first elements in a range view and return the
    ///        resulting subrange.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange,
              typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    PopFront(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Discard the last elements in a range view and return the
    ///        resulting subrange.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange,
              typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    PopBack(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Slice a range view returning the first element and a subrange to
    ///        the remaining ones.
    ///
    /// \remarks Calling this method with an empty range view results in
    ///          undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range) noexcept;

    /// \brief Slice a range view returning the last element and a subrange to
    ///        the remaining ones.
    ///
    /// \remarks Calling this method with an empty range view results in
    ///          undefined behavior.
    template <Concepts::RandomAccessRange TRange>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range) noexcept;

    /// \brief Slice a range view returning a subrange to the first count
    ///        elements and another subrange-view to the remaining ones.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange,
              typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRange> range, Immutable<TCount> count) noexcept;

    /// \brief Slice a range view returning a subrange to the last count
    ///        elements and another subrange to the remaining ones.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRange TRange,
              typename TCount = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRange> range, Immutable<TCount> count) noexcept;

}

// ===========================================================================

#include "details/slice_range.inl"

// ===========================================================================
