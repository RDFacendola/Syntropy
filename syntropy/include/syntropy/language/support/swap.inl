
/// \file swap.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

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
