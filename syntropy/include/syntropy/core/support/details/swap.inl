
/// \file swap.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Swap.
    // =====

    template <typename TType, typename UType>
    requires Concepts::IsAssignableFrom<Mutable<TType>, Forwarding<UType>>
          && Concepts::IsMoveConstructible<TType>
    [[nodiscard]] constexpr auto
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
        -> decltype(Details::RouteExchange(lhs, Forward<UType>(rhs)))
    {
        return Details::RouteExchange(lhs, Forward<UType>(rhs));
    }

    template <Concepts::IsSwappable TType>
    constexpr auto
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
        -> decltype(Details::RouteSwap(lhs, rhs))
    {
        Details::RouteSwap(lhs, rhs);
    }

}

// ===========================================================================
