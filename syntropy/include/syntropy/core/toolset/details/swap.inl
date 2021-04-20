
/// \file swap.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy::Algorithms
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Swap.
    // =====

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
        -> decltype(Details::RouteExchange(lhs, Forward<UType>(rhs)))
    {
        return Details::RouteExchange(lhs, Forward<UType>(rhs));
    }

    template <typename TType>
    constexpr auto
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
        -> decltype(Details::RouteSwap(lhs, rhs))
    {
        Details::RouteSwap(lhs, rhs);
    }

}

// ===========================================================================
