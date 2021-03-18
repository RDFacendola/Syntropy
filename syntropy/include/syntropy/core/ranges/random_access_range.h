
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

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/ranges/bidirectional_range.h"
#include "syntropy/core/ranges/sized_range.h"
#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

#include "details/random_access_range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE VIEW                                             */
    /************************************************************************/

    /// \brief Minimal interface for range views whose elements can be visited
    ///        in any order.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept BaseRandomAccessRangeView =
        requires(Immutable<TRangeView> range_view)
        {
            /// \brief Get range view's elements count.
            { Details::RouteCount(range_view) };
        }
        && requires(Immutable<TRangeView> range_view,
                    Immutable<Templates::RangeViewCountType<TRangeView>> index,
                    Immutable<Templates::RangeViewCountType<TRangeView>> count)
        {
            /// \brief Access range view's element by index.
            { Details::RouteAt(range_view, index) };

            /// \brief Obtain a range view to a sub-range-view.
            { Details::RouteSlice(range_view, index, count) };
        };

    /// \brief Range whose elements can be visited in any order.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept RandomAccessRangeView = BaseRandomAccessRangeView<TRangeView>
        && BidirectionalRangeView<TRangeView>
        && SizedRangeView<TRangeView>;

    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Container whose elements can be visited in any order.
    ///
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept RandomAccessRange = requires(Immutable<TRange> range)
    {
        { ViewOf(range) } -> RandomAccessRangeView;
    };

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Random access range view.
    // =========================

    /// \brief Access range view's element by index.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView,
              typename TIndex = Templates::RangeViewCountType<TRangeView>>
    [[nodiscard]] constexpr decltype(auto)
    At(Immutable<TRangeView> range_view, Immutable<TIndex> index) noexcept;

    /// \brief Obtain a range view to a sub-range-view.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView,
              typename TIndex = Templates::RangeViewCountType<TRangeView>,
              typename TCount = TIndex>
    [[nodiscard]] constexpr TRangeView
    Slice(Immutable<TRangeView> range_view,
          Immutable<TIndex> index,
          Immutable<TCount> count) noexcept;

    /// \brief Take a number of elements from the range view's front.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView,
              typename TCount = Templates::RangeViewCountType<TRangeView>>
    [[nodiscard]] constexpr TRangeView
    Front(Immutable<TRangeView> range_view, Immutable<TCount> count) noexcept;

    /// \brief Take a number of elements from the range view's back.
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView,
              typename TCount = Templates::RangeViewCountType<TRangeView>>
    [[nodiscard]] constexpr TRangeView
    Back(Immutable<TRangeView> range_view, Immutable<TCount> count) noexcept;

    /// \brief Discard the first elements in a range view and return the
    ///        resulting subrange.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView,
              typename TCount = Templates::RangeViewCountType<TRangeView>>
    [[nodiscard]] constexpr TRangeView
    PopFront(Immutable<TRangeView> range_view,
             Immutable<TCount> count) noexcept;

    /// \brief Discard the last elements in a range view and return the
    ///        resulting subrange.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView,
              typename TCount = Templates::RangeViewCountType<TRangeView>>
    [[nodiscard]] constexpr TRangeView
    PopBack(Immutable<TRangeView> range_view,
            Immutable<TCount> count) noexcept;

    /// \brief Slice a range view returning the first element and a subrange to
    ///        the remaining ones.
    ///
    /// \remarks Calling this method with an empty range view results in
    ///          undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRangeView> range_view) noexcept;

    /// \brief Slice a range view returning the last element and a subrange to
    ///        the remaining ones.
    ///
    /// \remarks Calling this method with an empty range view results in
    ///          undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRangeView> range_view) noexcept;

    /// \brief Slice a range view returning a subrange to the first count
    ///        elements and another subrange-view to the remaining ones.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView,
              typename TCount = Templates::RangeViewCountType<TRangeView>>
    [[nodiscard]] constexpr auto
    SliceFront(Immutable<TRangeView> range_view,
               Immutable<TCount> count) noexcept;

    /// \brief Slice a range view returning a subrange to the last count
    ///        elements and another subrange to the remaining ones.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <Concepts::RandomAccessRangeView TRangeView,
              typename TCount = Templates::RangeViewCountType<TRangeView>>
    [[nodiscard]] constexpr auto
    SliceBack(Immutable<TRangeView> range_view,
              Immutable<TCount> count) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE EXTENSIONS                                       */
    /************************************************************************/

    /// \brief Access the first element in a range view.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    /// \remarks This extension adapts random access range views to meet both
    ///          bidirectional and sized range view concepts.
    template <Concepts::BaseRandomAccessRangeView TRangeView>
    struct Front<TRangeView>
    {
        [[nodiscard]] decltype(auto)
        operator()(Immutable<TRangeView> range_view) const noexcept;
    };

    /// \brief Discard the first element in a range view and return a view to
    ///        the remaining elements.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    /// \remarks This extension adapts random access range views to meet both
    ///          bidirectional and sized range view concepts.
    template <Concepts::BaseRandomAccessRangeView TRangeView>
    struct PopFront<TRangeView>
    {
        [[nodiscard]] TRangeView
        operator()(Immutable<TRangeView> range_view) const noexcept;
    };

    /// \brief Access the last element in a range view
    ///.
    /// \remarks Undefined behavior if the range view is empty.
    /// \remarks This extension adapts random access range views to meet both
    ///          bidirectional and sized range view concepts.
    template <Concepts::BaseRandomAccessRangeView TRangeView>
    struct Back<TRangeView>
    {
        [[nodiscard]] decltype(auto)
        operator()(Immutable<TRangeView> range_view) const noexcept;
    };

    /// \brief Discard the las element in a range view and return a view to
    ///        the remaining elements.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    /// \remarks This extension adapts random access range views to meet both
    ///          bidirectional and sized range view concepts.
    template <Concepts::BaseRandomAccessRangeView TRangeView>
    struct PopBack<TRangeView>
    {
        [[nodiscard]] TRangeView
        operator()(Immutable<TRangeView> range_view) const noexcept;
    };
}

// ===========================================================================

#include "details/random_access_range.inl"

// ===========================================================================
