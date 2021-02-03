
/// \file reverse_range_details.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains implementation details about reverse ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/concepts/forward_range.h"
#include "syntropy/core/concepts/sized_range.h"
#include "syntropy/core/concepts/bidirectional_range.h"
#include "syntropy/core/concepts/random_access_range.h"
#include "syntropy/core/concepts/contiguous_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    /// \brief Adapter class used to reverse a bidirectional range.
    /// \author Raffaele D. Facendola - November 2020.
    template <Ranges::Concepts::BidirectionalRange TRange>
    class ReverseRange
    {
        template <Ranges::Concepts::BidirectionalRange URange>
        friend constexpr URange
        Reverse(Immutable<ReverseRange<URange>> range) noexcept;

    public:

        /// \brief Default constructor.
        constexpr
        ReverseRange() = default;

        /// \brief Default copy-constructor.
        constexpr
        ReverseRange(Immutable<ReverseRange> rhs) = default;

        /// \brief Create a new reverse range.
        template <Ranges::Concepts::BidirectionalRange URange = TRange>
        constexpr explicit
        ReverseRange(Immutable<URange> range) noexcept;

        /// \brief Default destructor.
        ~ReverseRange() = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<ReverseRange>
        operator=(Immutable<ReverseRange> rhs) = default;

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
        /// \remarks Accessing the last element of an empty range results
        ///          in undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetBack() const noexcept;

        /// \brief Discard range's last element and return the resulting range.
        /// \remarks If the provided range is empty, the behavior of this
        ///          method is undefined.
        [[nodiscard]] constexpr auto
        PopBack() const noexcept;

        /// \brief Access range's element by index.
        /// \remarks Exceeding range boundaries results in undefined behavior.
        template <typename TIndex>
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<TIndex> index) const noexcept;

        /// \brief Obtain a view to a sub-range.
        /// \remarks Exceeding range boundaries results in undefined behavior.
        template <typename TIndex, typename TCount>
        [[nodiscard]] constexpr auto
        Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept;

    private:

        /// \brief Underlying range.
        TRange range_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get a reversed range.
    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr ReverseRange<TRange>
    Reverse(Immutable<TRange> range) noexcept;

    /// \brief Get a reversed range.
    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    Reverse(Immutable<ReverseRange<TRange>> range) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ReverseRange.
    // =============

    template <Ranges::Concepts::BidirectionalRange TRange>
    template <Ranges::Concepts::BidirectionalRange URange>
    constexpr ReverseRange<TRange>
    ::ReverseRange(Immutable<URange> range) noexcept
        : range_(range)
    {

    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::GetFront() const noexcept
    {
        return RouteBack(range_);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::PopFront() const noexcept
    {
        return ReverseRange<TRange>{ RoutePopBack(range_) };
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr Bool ReverseRange<TRange>
    ::IsEmpty() const noexcept
    {
        return RouteIsEmpty(range_);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::GetCount() const noexcept
    {
        return RouteCount(range_);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::GetBack() const noexcept
    {
        return RouteFront(range_);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::PopBack() const noexcept
    {
        return ReverseRange<TRange>{ RoutePopFront(range_) };
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    template <typename TIndex>
    [[nodiscard]] constexpr decltype(auto) ReverseRange<TRange>
    ::At(Immutable<TIndex> index) const noexcept
    {
        return RouteAt(range_, RouteCount(range_) - index - TIndex{ 1 });
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    template <typename TIndex, typename TCount>
    [[nodiscard]] constexpr auto ReverseRange<TRange>
    ::Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept
    {
        return ReverseRange<TRange>
        {
            RouteSlice(range_, RouteCount(range_) - index - count, count)
        };
    }

    // Non-member functions.
    // =====================

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr ReverseRange<TRange>
    Reverse(Immutable<TRange> range) noexcept
    {
        return ReverseRange<TRange>(range);
    }

    template <Ranges::Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    Reverse(Immutable<ReverseRange<TRange>> range) noexcept
    {
        return range.range_;
    }
}

// ===========================================================================
