
/// \file reverse_range.h
///
/// \brief This header is part of the Syntropy core module.
///        it contains definitions for adapters used to iterate a range_view in
///        reverse ordedr.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/ranges/bidirectional_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* REVERSE RANGE VIEW                                                   */
    /************************************************************************/

    /// \brief Adapter class used to reverse bidirectional range_view views.
    /// \author Raffaele D. Facendola - November 2020.
    template <Concepts::BidirectionalRangeView TRangeView>
    class ReverseRange
    {
        template <Concepts::BidirectionalRangeView URangeView>
        friend constexpr URangeView
        Reverse(Immutable<ReverseRange<URangeView>> range_view) noexcept;

    public:

        /// \brief Default constructor.
        constexpr
        ReverseRange() = default;

        /// \brief Default copy-constructor.
        constexpr
        ReverseRange(Immutable<ReverseRange> rhs) = default;

        /// \brief Create a new reverse range_view.
        template <Concepts::BidirectionalRangeView URangeView = TRangeView>
        constexpr explicit
        ReverseRange(Immutable<URangeView> range_view) noexcept;

        /// \brief Default destructor.
        ~ReverseRange() = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<ReverseRange>
        operator=(Immutable<ReverseRange> rhs) = default;

        /// \brief Access the first element in the view.
        ///
        /// \remarks If this view is empty, calling this method results in
        ///          undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetFront() const noexcept;

        /// \brief Discard the first element in the view and return the
        ///        view to the remaining elements.
        ///
        /// \remarks If this view is empty, calling this method results in
        ///          undefined behavior.
        [[nodiscard]] constexpr auto
        PopFront() const noexcept;

        /// \brief Check whether the view is empty.
        [[nodiscard]] constexpr Bool
        IsEmpty() const noexcept;

        /// \brief Get the number of element.
        [[nodiscard]] constexpr auto
        GetCount() const noexcept;

        /// \brief Access the last element in the view.
        ///
        /// \remarks If this view is empty, calling this method results in
        ///          undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetBack() const noexcept;

        /// \brief Discard the last element in the view and return the
        ///        view to the remaining elements.
        ///
        /// \remarks If this view is empty, calling this method results in
        ///          undefined behavior.
        [[nodiscard]] constexpr auto
        PopBack() const noexcept;

        /// \brief Access a range view element by index.
        /// \remarks Exceeding view boundaries results in undefined behavior.
        template <typename TIndex>
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<TIndex> index) const noexcept;

        /// \brief Obtain a sub-range-view.
        /// \remarks Exceeding view boundaries results in undefined behavior.
        template <typename TIndex, typename TCount>
        [[nodiscard]] constexpr auto
        Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept;

    private:

        /// \brief Underlying range_view.
        TRangeView range_view_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get a reversed range view.
    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr ReverseRange<TRangeView>
    Reverse(Immutable<TRangeView> range_view) noexcept;

    /// \brief Get a reversed range view.
    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    Reverse(Immutable<ReverseRange<TRangeView>> range_view) noexcept;

}

// ===========================================================================

#include "details/reverse_range.inl"

// ===========================================================================
