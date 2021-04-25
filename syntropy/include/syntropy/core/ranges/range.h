
/// \file range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definition for ranges, homogeneous collection of
///        elements that can be visited in some order.
///
/// Ranges specifications based on the awesome
///     https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - March 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* RANGE EXTENSIONS                                                     */
    /************************************************************************/

    /// \brief Get a view to a range' elements
    template <typename TRange, typename = void>
    struct ViewOf;

    /// \brief Check whether a range view is empty.
    template <typename TRangeView, typename = void>
    struct IsEmpty;

    /// \brief Get the number of elements in a range view.
    template <typename TRangeView, typename = void>
    struct Count;

    /// \brief Select a subrange o elements.
    template <typename TRangeView, typename = void>
    struct Select;

    /// \brief Access the first element in a range view.
    template <typename TRangeView, typename = void>
    struct Front;

    /// \brief Access the last element in a range view.
    template <typename TRangeView, typename = void>
    struct Back;

    /// \brief Discard the first element in a range view and return a view to
    ///        the remaining elements.
    template <typename TRangeView, typename = void>
    struct PopFront;

    /// \brief Discard the last element in a range view and return a view to
    ///        the remaining elements.
    template <typename TRangeView, typename = void>
    struct PopBack;

    /// \brief Access an element of a range view by index.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <typename TRangeView, typename = void>
    struct At;

    /// \brief Access the element storage of a range view.
    template <typename TRangeView, typename = void>
    struct Data;

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    /// \brief Get a view to a range' elements.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    ViewOf(Forwarding<TRange> range) noexcept;

    /// \brief Check whether a range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr Bool
    IsEmpty(Forwarding<TRange> range) noexcept;

    /// \brief Get the number of elements in a range.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    Count(Forwarding<TRange> range) noexcept;

    /// \brief Access the first element in a range.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr decltype(auto)
    Front(Forwarding<TRange> range) noexcept;

    /// \brief Access the last element in a range.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr decltype(auto)
    Back(Forwarding<TRange> range) noexcept;

    /// \brief Discard the first element in a range and return a range to the
    ///        remaining elements.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    PopFront(Forwarding<TRange> range) noexcept;

    /// \brief Discard the last element in a range and return a range to the
    ///        remaining elements.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    PopBack(Forwarding<TRange> range) noexcept;

    /// \brief Access range element by index.
    ///
    /// \remarks Undefined behavior if range boundaries are exceeded.
    template <typename TRange, typename TIndex>
    [[nodiscard]] constexpr decltype(auto)
    At(Forwarding<TRange> range, Immutable<TIndex> index) noexcept;

    /// \brief Select a subrange of elements.
    ///
    /// \remarks Undefined behavior if range boundaries are exceeded.
    template <typename TRange, typename TCardinality>
    [[nodiscard]] constexpr decltype(auto)
    Select(Forwarding<TRange> range,
           Immutable<TCardinality> offset,
           Immutable<TCardinality> count) noexcept;

    /// \brief Access a range elements' storage.
    ///
    /// \remarks Undefined behavior if the range is empty.
    template <typename TRange>
    [[nodiscard]] constexpr decltype(auto)
    Data(Forwarding<TRange> range) noexcept;

    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Type of a range's view.
    template <typename TRange>
    using RangeViewTypeOf
        = Templates::UnqualifiedOf<decltype(
            Ranges::ViewOf(Templates::Declval<TRange>()))>;

    /// \brief Type of a range's cardinality.
    template <typename TRange>
    using RangeCardinalityTypeOf
        = Templates::UnqualifiedOf<decltype(
            Ranges::Count(Templates::Declval<TRange>()))>;

    /// \brief Type of a range elements.
    template <typename TRange>
    using RangeElementTypeOf
        = Templates::UnqualifiedOf<decltype(
            Ranges::Front(Templates::Declval<TRange>()))>;

}

// ===========================================================================

#include "details/range.inl"

// ===========================================================================
