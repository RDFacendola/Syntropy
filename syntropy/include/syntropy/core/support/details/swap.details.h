
/// \file swap.details.h
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

#include "syntropy/core/support/swap_extensions.h"

// ===========================================================================

namespace Syntropy::Algorithm::Details
{
    /************************************************************************/
    /* CUSTOMIZATION-POINT-OBJECTS                                          */
    /************************************************************************/

    // Core idea based on this amazing post:
    // https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /// \brief Priority of a custom extension implementation.
    using ExtensionPriority = Templates::Priority<4>;

    /// \brief Priority of a member-function implementation.
    using MemberFunctionPriority = Templates::Priority<3>;

    /// \brief Priority of a member-operator implementation.
    using MemberOperatorPriority = Templates::Priority<2>;

    /// \brief Priority of a non-member-function implementation.
    using NonMemberFunctionPriority = Templates::Priority<1>;

    /// \brief Priority of a fallback implementation.
    using FallbackPriority = Templates::Priority<0>;

    /// \brief Highest priority among method implementations.
    inline constexpr Templates::Priority kMaxPriority = ExtensionPriority{};

    /************************************************************************/
    /* EXCHANGE                                                             */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeExchange(Mutable<TType> lhs,
                   Forwarding<UType> rhs,
                   ExtensionPriority)
        noexcept -> decltype(Extensions::Exchange<TType, UType>{}(
            lhs,
            Forward<UType>(rhs)));

    /// \brief Member-function.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeExchange(Mutable<TType> lhs,
                   Forwarding<UType> rhs,
                   MemberFunctionPriority)
        noexcept -> decltype(lhs.Exchange(Forward<UType>(rhs)));

    /// \brief Non-member function, possibly using ADL.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeExchange(Mutable<TType> lhs,
                   Forwarding<UType> rhs,
                   NonMemberFunctionPriority)
        noexcept -> decltype(Exchange(lhs, Forward<UType>(rhs)));

    /// \brief Fallback.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr TType
    InvokeExchange(Mutable<TType> lhs, Forwarding<UType> rhs, FallbackPriority)
        noexcept;

    /// \brief Routes the invocation.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteExchange(Mutable<TType> lhs, Forwarding<UType> rhs)
        noexcept -> decltype(InvokeExchange(lhs,
                                            Forward<UType>(rhs),
                                            kMaxPriority));

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TType>
    [[nodiscard]] constexpr auto
    InvokeSwap(Mutable<TType> lhs, Mutable<TType> rhs, ExtensionPriority)
        noexcept -> decltype(Extensions::Swap<TType>{}(lhs, rhs));

    /// \brief Member-function.
    template <typename TType>
    [[nodiscard]] constexpr auto
    InvokeSwap(Mutable<TType> lhs, Mutable<TType> rhs, MemberFunctionPriority)
        noexcept -> decltype(lhs.Swap(rhs));

    /// \brief Non-member function, possibly using ADL.
    template <typename TType>
    [[nodiscard]] constexpr auto
    InvokeSwap(Mutable<TType> lhs,
               Mutable<TType> rhs,
               NonMemberFunctionPriority)
        noexcept -> decltype(Swap(lhs, rhs));

    /// \brief Fallback.
    template <typename TType>
    constexpr void
    InvokeSwap(Mutable<TType> lhs, Mutable<TType> rhs, FallbackPriority)
        noexcept;

    /// \brief Routes the invocation.
    template <typename TType>
    [[nodiscard]] constexpr auto
    RouteSwap(Mutable<TType> lhs, Mutable<TType> rhs)
        noexcept -> decltype(InvokeSwap(lhs, rhs, kMaxPriority));

}

// ===========================================================================
