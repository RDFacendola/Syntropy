
/// \file swap.h
/// \brief This header is part of Syntropy language module. It contains swapping-related definitions.
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
    template <typename TType> requires Concepts::AssignableFrom<MutableRef<TType>, MoveRef<TType>> && Concepts::MoveConstructible<TType>
    constexpr void Swap(MutableRef<TType> lhs, MutableRef<TType> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <typename TType, typename UType = TType> requires Concepts::AssignableFrom<MutableRef<TType>, ForwardRef<UType>> && Concepts::MoveConstructible<TType>
    constexpr TType Exchange(MutableRef<TType> lhs, ForwardRef<UType> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Swap.
    // =====

    template <typename TType> requires Concepts::AssignableFrom<MutableRef<TType>, MoveRef<TType>> && Concepts::MoveConstructible<TType>
    constexpr void Swap(MutableRef<TType> lhs, MutableRef<TType> rhs) noexcept
    {
        auto xhs = Move(lhs);

        lhs = Move(rhs);
        rhs = Move(xhs);
    }

    template <typename TType, typename UType> requires Concepts::AssignableFrom<MutableRef<TType>, ForwardRef<UType>> && Concepts::MoveConstructible<TType>
    constexpr TType Exchange(MutableRef<TType> lhs, ForwardRef<UType> rhs) noexcept
    {
        auto result = Move(lhs);

        lhs = Forward<UType>(rhs);
        
        return result;
    }

}

// ===========================================================================