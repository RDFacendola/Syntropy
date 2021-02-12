
/// \file swap.h
///
/// \brief This header is part of Syntropy language module.
///        It contains swapping-related definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

#include "swap.details.h"

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* EXCHANGE                                                             */
    /************************************************************************/

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <typename TType, typename UType = TType>
    requires Concepts::AssignableFrom<Mutable<TType>, Forwarding<UType>>
          && Concepts::MoveConstructible<TType>
    [[nodiscard]] constexpr TType
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept;

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Swap lhs with rhs.
    template <typename TType>
    requires Concepts::AssignableFrom<Mutable<TType>, Movable<TType>>
          && Concepts::MoveConstructible<TType>
    constexpr void
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept;

}

// ===========================================================================

#include "swap.inl"

// ===========================================================================
