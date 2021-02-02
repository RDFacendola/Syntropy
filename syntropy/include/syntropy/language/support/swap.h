
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

namespace Syntropy
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Swap lhs with rhs.
    template <typename TType>
    requires Concepts::AssignableFrom<Mutable<TType>, Movable<TType>>
          && Concepts::MoveConstructible<TType>
    constexpr void
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <typename TType, typename UType = TType>
    requires Concepts::AssignableFrom<Mutable<TType>, Forwarding<UType>>
          && Concepts::MoveConstructible<TType>
    [[nodiscard]] constexpr TType
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Swap.
    // =====

    template <typename TType>
    requires Concepts::AssignableFrom<Mutable<TType>, Movable<TType>>
          && Concepts::MoveConstructible<TType>
    constexpr void
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
    {
        rhs = Exchange(lhs, rhs);
    }

    template <typename TType, typename UType>
    requires Concepts::AssignableFrom<Mutable<TType>, Forwarding<UType>>
          && Concepts::MoveConstructible<TType>
    [[nodiscard]] constexpr TType
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
    {
        auto result = Move(lhs);

        lhs = Forward<UType>(rhs);

        return result;
    }

}

// ===========================================================================
