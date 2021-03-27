
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
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
        -> decltype(Details::RouteExchange(lhs, Forward<UType>(rhs)));

    /// \brief Swap lhs with rhs.
    template <typename TType>
    constexpr auto
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
        -> decltype(Details::RouteSwap(lhs, rhs));

}

// ===========================================================================

#include "details/swap.inl"

// ===========================================================================
