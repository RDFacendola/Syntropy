
/// \file compare.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for comparisons.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/support/ordering.h"

#include <compare>

// ===========================================================================

#include "details/compare.details.h"

// ===========================================================================

namespace Syntropy::Algorithms
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equal.
    /// \remarks Equality implies equivalence.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    AreEqual(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(Details::RouteAreEqual(lhs, rhs));

    /// \brief Check whether lhs and rhs are equivalent.
    /// \brief Equivalence doesn't imply equality.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    AreEquivalent(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(Details::RouteAreEquivalent(lhs, rhs));

    /// \brief Compare lhs against rhs.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    Compare(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(Details::RouteCompare(lhs, rhs));

}

// ===========================================================================

#include "details/compare.inl"

// ===========================================================================
