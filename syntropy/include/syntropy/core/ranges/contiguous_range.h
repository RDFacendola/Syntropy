
/// \file contiguous_range.h
///
/// \brief This header is part of the Syntropy core module. It contains
///        definitions for ranges whose elements are allocated contiguously.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/ranges/random_access_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Concept for ranges whose elements are allocated contiguously.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ContiguousRange = RandomAccessRange<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Access a range elements' storage.
            { Ranges::Data(range) };
        };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // ContiguousRange.
    // ================

    /// \brief Check whether two ranges are equal.
    template <ContiguousRange TRange, ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Check whether two contiguous ranges intersects.
    template <ContiguousRange TRange, ContiguousRange URange>
    [[nodiscard]] constexpr Bool
    Intersect(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

}

// ===========================================================================

#include "details/contiguous_range.inl"

// ===========================================================================
