
/// \file swap.h
/// \brief This header is part of Syntropy language module. It contains swapping-related definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Swap lhs with rhs.
    template <typename TType> requires Concepts::AssignableFrom<TType&, TType&&> && Concepts::MoveConstructible<TType>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <typename TType, typename UType = TType> requires Concepts::AssignableFrom<TType&, UType&&>&& Concepts::MoveConstructible<TType>
    constexpr TType Exchange(TType& lhs, UType&& rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Swap.
    // =====

    template <typename TType> requires Concepts::AssignableFrom<TType&, TType&&>&& Concepts::MoveConstructible<TType>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept
    {
        auto xhs = Move(lhs);

        lhs = Move(rhs);
        rhs = Move(xhs);
    }

    template <typename TType, typename UType> requires Concepts::AssignableFrom<TType&, UType&&>&& Concepts::MoveConstructible<TType>
    constexpr TType Exchange(TType& lhs, UType&& rhs) noexcept
    {
        auto result = Move(lhs);

        lhs = Forward<UType>(rhs);
        
        return result;
    }

}

// ===========================================================================