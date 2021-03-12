
/// \file reverse_range.h
///
/// \brief This header is part of the Syntropy core module.
///        it contains definitions for adapters used to iterate a range in
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
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    /// \brief Adapter class used to reverse a bidirectional range.
    /// \author Raffaele D. Facendola - November 2020.
    template <Concepts::BidirectionalRange TRange>
    class ReverseRange
    {
        template <Concepts::BidirectionalRange URange>
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
        template <Concepts::BidirectionalRange URange = TRange>
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
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr ReverseRange<TRange>
    Reverse(Immutable<TRange> range) noexcept;

    /// \brief Get a reversed range.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    Reverse(Immutable<ReverseRange<TRange>> range) noexcept;

}

// ===========================================================================

#include "details/reverse_range.inl"

// ===========================================================================
