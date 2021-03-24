
/// \file swap.h
///
/// \brief This header is part of Syntropy core module.
///        It contains swapping-related definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

#include "details/swap.details.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Concept for types whose instances are swappable.
    template <typename TType>
    concept IsSwappable = IsAssignableFrom<Mutable<TType>, Movable<TType>>
        && IsMoveConstructible<TType>;
}

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Swap.
    // =====

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <typename TType, typename UType = TType>
    requires Concepts::IsAssignableFrom<Mutable<TType>, Forwarding<UType>>
          && Concepts::IsMoveConstructible<TType>
    [[nodiscard]] constexpr TType
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept;

    /// \brief Swap lhs with rhs.
    template <Concepts::IsSwappable TType>
    constexpr void
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept;

}

// ===========================================================================

#include "details/swap.inl"

// ===========================================================================
