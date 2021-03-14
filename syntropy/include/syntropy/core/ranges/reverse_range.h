
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

        /// \brief Access range_view's first element.
        ///
        /// \remarks Accessing the first element of an empty range_view results
        ///          in undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetFront() const noexcept;

        /// \brief Discard range_view's first element and return the resulting
        ///        range_view.
        ///
        /// \remarks If the provided range_view is empty, the behavior of this
        ///          method is undefined.
        [[nodiscard]] constexpr auto
        PopFront() const noexcept;

        /// \brief Check whether the range_view is empty.
        [[nodiscard]] constexpr Bool
        IsEmpty() const noexcept;

        /// \brief Get range_view's elements count.
        [[nodiscard]] constexpr auto
        GetCount() const noexcept;

        /// \brief Access range_view's last element.
        /// \remarks Accessing the last element of an empty range_view results
        ///          in undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetBack() const noexcept;

        /// \brief Discard range_view's last element and return the resulting range_view.
        /// \remarks If the provided range_view is empty, the behavior of this
        ///          method is undefined.
        [[nodiscard]] constexpr auto
        PopBack() const noexcept;

        /// \brief Access range_view's element by index.
        /// \remarks Exceeding range_view boundaries results in undefined behavior.
        template <typename TIndex>
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<TIndex> index) const noexcept;

        /// \brief Obtain a view to a sub-range_view.
        /// \remarks Exceeding range_view boundaries results in undefined behavior.
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

    /// \brief Get a reversed range_view.
    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr ReverseRange<TRangeView>
    Reverse(Immutable<TRangeView> range_view) noexcept;

    /// \brief Get a reversed range_view.
    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    Reverse(Immutable<ReverseRange<TRangeView>> range_view) noexcept;

}

// ===========================================================================

#include "details/reverse_range.inl"

// ===========================================================================
