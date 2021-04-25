
/// \file compare.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for comparisons.
///
/// \author Raffaele D. Facendola - November 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/comparisons/ordering.h"

// ===========================================================================

namespace Syntropy::Comparisons::Extensions
{
    /************************************************************************/
    /* COMPARE EXTENSIONS                                                   */
    /************************************************************************/

    /// \brief Check whether two objects are equal.
    template <typename TType, typename UType, typename = void>
    struct AreEqual;

    /// \brief Check whether two objects are equivalent.
    template <typename TType, typename UType, typename = void>
    struct AreEquivalent;

    /// \brief Compare two objects.
    template <typename TType, typename UType, typename = void>
    struct Compare;

}

// ===========================================================================

namespace Syntropy::Comparisons
{
    /************************************************************************/
    /* COMPARE                                                              */
    /************************************************************************/

    /// \brief Check whether lhs and rhs are equal.
    /// \remarks Equality implies equivalence.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TType> lhs, Immutable<UType> rhs) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TType> lhs, Immutable<UType> rhs) noexcept;

    /// \brief Compare lhs against rhs.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TType> lhs, Immutable<UType> rhs) noexcept;

}

// ===========================================================================

#include "details/compare.inl"

// ===========================================================================
