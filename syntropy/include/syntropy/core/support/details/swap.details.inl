
/// \file swap.details.h
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy::Algorithm::Details
{
    /************************************************************************/
    /* EXCHANGE                                                             */
    /************************************************************************/

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeExchange(Mutable<TType> lhs,
                   Forwarding<UType> rhs,
                   ExtensionPriority)
        noexcept -> decltype(Extensions::Exchange<TType, UType>{}(
            lhs,
            Forward<UType>(rhs)))
    {
        return Extensions::Exchange<TType, UType>{}(lhs,
                                                    Forward<UType>(rhs));
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeExchange(Mutable<TType> lhs,
                   Forwarding<UType> rhs,
                   MemberFunctionPriority)
        noexcept -> decltype(lhs.Exchange(Forward<UType>(rhs)))
    {
        return lhs.Exchange(Forward<UType>(rhs));
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeExchange(Mutable<TType> lhs,
                   Forwarding<UType> rhs,
                   NonMemberFunctionPriority)
        noexcept -> decltype(Exchange(lhs, Forward<UType>(rhs)))
    {
        return Exchange(lhs, Forward<UType>(rhs));
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr TType
    InvokeExchange(Mutable<TType> lhs, Forwarding<UType> rhs, FallbackPriority)
        noexcept
    {
        auto result = Move(lhs);

        lhs = Forward<UType>(rhs);

        return result;
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteExchange(Mutable<TType> lhs, Forwarding<UType> rhs)
        noexcept -> decltype(InvokeExchange(lhs,
                                            Forward<UType>(rhs),
                                            kMaxPriority))
    {
        return InvokeExchange(lhs, Forward<UType>(rhs), kMaxPriority);
    }

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    template <typename TType>
    [[nodiscard]] constexpr auto
    InvokeSwap(Mutable<TType> lhs, Mutable<TType> rhs, ExtensionPriority)
        noexcept -> decltype(Extensions::Swap<TType>{}(lhs, rhs))
    {
        return Extensions::Swap<TType>{}(lhs, rhs);
    }

    template <typename TType>
    [[nodiscard]] constexpr auto
    InvokeSwap(Mutable<TType> lhs, Mutable<TType> rhs, MemberFunctionPriority)
        noexcept -> decltype(lhs.Swap(rhs))
    {
        return lhs.Swap(rhs);
    }

    template <typename TType>
    [[nodiscard]] constexpr auto
    InvokeSwap(Mutable<TType> lhs,
               Mutable<TType> rhs,
               NonMemberFunctionPriority)
        noexcept -> decltype(Swap(lhs, rhs))
    {
        return Swap(lhs, rhs);
    }

    template <typename TType>
    constexpr void
    InvokeSwap(Mutable<TType> lhs, Mutable<TType> rhs, FallbackPriority)
        noexcept
    {
        rhs = RouteExchange(lhs, rhs);
    }

    template <typename TType>
    [[nodiscard]] constexpr auto
    RouteSwap(Mutable<TType> lhs, Mutable<TType> rhs)
        noexcept -> decltype(InvokeSwap(lhs, rhs, kMaxPriority))
    {
        return InvokeSwap(lhs, rhs, kMaxPriority);
    }

}

// ===========================================================================
