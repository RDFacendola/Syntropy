
/// \file swap.deails.h
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"
#include "syntropy/language/support/swap_extensions.h"

// ===========================================================================

namespace Syntropy::Algorithm::Details
{
    /************************************************************************/
    /* EXCHANGE                                                             */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr
    auto InvokeExchange(Mutable<TType> lhs,
                        Forwarding<UType> rhs,
                        Syntropy::Templates::Priority<3>) noexcept
        -> decltype(Algorithm::Extensions::Exchange<TType, UType>{}(
               lhs,
               Forward<UType>(rhs)))
    {
        return Algorithm::Extensions::Exchange<TType, UType>{}(
            lhs,
            Forward<UType>(rhs));
    }

    /// \brief Invoke the method via member-function.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr
    auto InvokeExchange(Mutable<TType> lhs,
                        Forwarding<UType> rhs,
                        Syntropy::Templates::Priority<2>)
        noexcept -> decltype(lhs.Exchange(Forward<UType>(rhs)))
    {
        return lhs.Exchange(Forward<UType>(rhs));
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr
    auto InvokeExchange(Mutable<TType> lhs,
                        Forwarding<UType> rhs,
                        Syntropy::Templates::Priority<1>)
        noexcept -> decltype(Exchange(lhs, Forward<UType>(rhs)))
    {
        return Exchange(lhs, Forward<UType>(rhs));
    }

    /// \brief Fall back to basic implementation.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr
    TType InvokeExchange(Mutable<TType> lhs,
                         Forwarding<UType> rhs,
                         Syntropy::Templates::Priority<0>) noexcept
    {
        auto result = Move(lhs);

        lhs = Forward<UType>(rhs);

        return result;
    }

    /// \brief Routes the invocation.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr
    auto RouteExchange(Mutable<TType> lhs, Forwarding<UType> rhs)
        noexcept -> decltype(InvokeExchange(lhs,
                                            Forward<UType>(rhs),
                                            Syntropy::Templates::kPriority<3>))
    {
        return InvokeExchange(lhs,
                              Forward<UType>(rhs),
                              Syntropy::Templates::kPriority<3>);
    }

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TType>
    [[nodiscard]] constexpr
    auto InvokeSwap(Mutable<TType> lhs,
                    Mutable<TType> rhs,
                    Syntropy::Templates::Priority<3>) noexcept
        -> decltype(Algorithm::Extensions::Swap<TType>{}(lhs, rhs))
    {
        return Algorithm::Extensions::Swap<TType>{}(lhs, rhs);
    }

    /// \brief Invoke the method via member-function.
    template <typename TType>
    [[nodiscard]] constexpr
    auto InvokeSwap(Mutable<TType> lhs,
                    Mutable<TType> rhs,
                    Syntropy::Templates::Priority<2>)
        noexcept -> decltype(lhs.Swap(rhs))
    {
        return lhs.Swap(rhs);
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TType>
    [[nodiscard]] constexpr
    auto InvokeSwap(Mutable<TType> lhs,
                    Mutable<TType> rhs,
                    Syntropy::Templates::Priority<1>)
        noexcept -> decltype(Swap(lhs, rhs))
    {
        return Swap(lhs, rhs);
    }

    /// \brief Fall back to basic implementation.
    template <typename TType>
    constexpr
    void InvokeSwap(Mutable<TType> lhs,
                    Mutable<TType> rhs,
                    Syntropy::Templates::Priority<0>) noexcept
    {
        rhs = RouteExchange(lhs, rhs);
    }

    /// \brief Routes the invocation.
    template <typename TType>
    [[nodiscard]] constexpr
    auto RouteSwap(Mutable<TType> lhs, Mutable<TType> rhs)
        noexcept -> decltype(InvokeSwap(lhs,
                                        rhs,
                                        Syntropy::Templates::kPriority<3>))
    {
        return InvokeSwap(lhs,
                          rhs,
                          Syntropy::Templates::kPriority<3>);
    }

}

// ===========================================================================
