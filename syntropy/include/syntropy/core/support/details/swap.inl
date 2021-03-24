
/// \file swap.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* EXCHANGE                                                             */
    /************************************************************************/

    template <typename TType, typename UType>
    requires Concepts::IsAssignableFrom<Mutable<TType>, Forwarding<UType>>
          && Concepts::IsMoveConstructible<TType>
    [[nodiscard]] constexpr TType
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
    {
        return Details::RouteExchange(lhs, Forward<UType>(rhs));
    }

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    template <Concepts::IsSwappable TType>
    constexpr void
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
    {
        Details::RouteSwap(lhs, rhs);
    }

}

// ===========================================================================
