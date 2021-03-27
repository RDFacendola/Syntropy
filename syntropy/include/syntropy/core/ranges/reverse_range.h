
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

namespace Syntropy
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    /// \brief Adapter class used to reverse bidirectional ranges.
    /// \author Raffaele D. Facendola - November 2020.
    template <Ranges::BidirectionalRange TRange>
    class ReverseRange
    {
        template <Ranges::BidirectionalRange URange>
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
        template <Ranges::BidirectionalRange URange = TRange>
        constexpr explicit
        ReverseRange(Immutable<URange> range) noexcept;

        /// \brief Default destructor.
        ~ReverseRange() = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<ReverseRange>
        operator=(Immutable<ReverseRange> rhs) = default;

        /// \brief Access the first element in the range.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr decltype(auto)
        GetFront() const noexcept;

        /// \brief Discard the first element in the range and return the
        ///        range to the remaining elements.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr auto
        PopFront() const noexcept;

        /// \brief Get the number of element.
        [[nodiscard]] constexpr auto
        GetCount() const noexcept;

        /// \brief Access the last element in the range.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr decltype(auto)
        GetBack() const noexcept;

        /// \brief Discard the last element in the range and return the
        ///        range to the remaining elements.
        ///
        /// \remarks Undefined behavior if the range is empty.
        [[nodiscard]] constexpr auto
        PopBack() const noexcept;

        /// \brief Access a range element by index.
        /// \remarks Undefined behavior if range boundaries are exceeded.
        template <typename TIndex>
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<TIndex> index) const noexcept;

    private:

        /// \brief Underlying range.
        TRange range_;

    };

    /// \brief Deduction guieds for SliceRange.
    template<Ranges::BidirectionalRange TRange>
    ReverseRange(Immutable<TRange>) -> ReverseRange<TRange>;

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    /// \brief Get a reversed range.
    template <BidirectionalRange TRange>
    [[nodiscard]] constexpr auto
    Reverse(Immutable<TRange> range) noexcept;

    /// \brief Get a reversed range.
    template <BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    Reverse(Immutable<ReverseRange<TRange>> range) noexcept;

}

// ===========================================================================

#include "details/reverse_range.inl"

// ===========================================================================
