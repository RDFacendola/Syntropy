
/// \file forward_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        sequentially.
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/ranges/range.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    /// \brief Concept for ranges whose elements can be visited sequentially.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ForwardRange = requires(Immutable<TRange> range)
    {
        /// \brief Get a view to a range' elements.
        { Ranges::ViewOf(range) };

        /// \brief Access the first element in a range.
        { Ranges::Front(range) };

        /// \brief Discard the first element in a range and return a range to
        ///        the remaining elements.
        { Ranges::PopFront(range) };

        /// \brief Check whether a range is empty.
        { Ranges::IsEmpty(range) };
    };

    /// \brief Tag type associated to forward ranges.
    struct ForwardRangeTag{};

    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    namespace Ranges
    {
        /// \brief Apply a function to each element in a range.
        template <ForwardRange TRange, typename TFunction>
        constexpr void
        ForEach(Immutable<TRange> range, TFunction function) noexcept;

        /// \brief Member-wise copy elements between two ranges, until either
        ///        is exhausted.
        ///
        /// \return Returns the number of copied elements.
        template <ForwardRange TRange, ForwardRange URange>
        constexpr Int
        PartialCopy(Immutable<TRange> destination,
            Immutable<URange> source,
            ForwardRangeTag = {}) noexcept;

        /// \brief Member-wise move elements between two ranges, until either
        ///        is exhausted.
        ///
        /// \return Returns the number of copied elements.
        template <ForwardRange TRange, ForwardRange URange>
        constexpr Int
        PartialMove(Immutable<TRange> destination,
            Immutable<URange> source,
            ForwardRangeTag = {}) noexcept;

        /// \brief Member-wise swap elements in both ranges until either is
        ///        exhausted.
        ///
        /// \return Returns the number of swap elements.
        template <ForwardRange TRange>
        constexpr Int
        PartialSwap(Immutable<TRange> lhs,
            Immutable<TRange> rhs,
            ForwardRangeTag = {}) noexcept;

        /// \brief Check whether two ranges are element-wise equal.
        template <ForwardRange TRange, ForwardRange URange>
        [[nodiscard]] constexpr Bool
        AreEqual(Immutable<TRange> lhs,
            Immutable<URange> rhs,
            ForwardRangeTag = {}) noexcept;

        /// \brief Check whether two ranges are element-wise equivalent.
        template <ForwardRange TRange, ForwardRange URange>
        [[nodiscard]] constexpr Bool
        AreEquivalent(Immutable<TRange> lhs,
            Immutable<URange> rhs,
            ForwardRangeTag = {}) noexcept;

        /// \brief Compare two range views lexicographically.
        template <ForwardRange TRange, ForwardRange URange>
        [[nodiscard]] constexpr Ordering
        Compare(Immutable<TRange> lhs,
            Immutable<URange> rhs,
            ForwardRangeTag = {}) noexcept;

    }

}

// ===========================================================================

#include "details/forward_range.inl"

// ===========================================================================
