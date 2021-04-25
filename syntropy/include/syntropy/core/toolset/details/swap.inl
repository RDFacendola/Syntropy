
/// \file swap.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

#include "syntropy/language/templates/invoke.h"

// ===========================================================================

namespace Syntropy::ToolsetADL
{
    /************************************************************************/
    /* SWAP EXTENSIONS                                                      */
    /************************************************************************/

    /// \brief Type alias for the AreEqual extension functor.
    template <typename TType, typename UType>
    using ExchangeExtension
        = Toolset::Extensions::Exchange<TType, UType>;

    /// \brief Type alias for the AreEquivalent extension functor.
    template <typename TType, typename UType>
    using SwapExtension
        = Toolset::Extensions::Swap<TType, UType>;

    /// \brief Invoke the Exchange function via extension functor.
    template <typename TType, typename UType>
    [[nodiscard]] auto
    InvokeExchangeExtension(Mutable<TType> lhs, Forwarding<UType> rhs)
        noexcept -> decltype(
            ExchangeExtension<TType, UType>{}(lhs, Forward<UType>(rhs)))
    {
        return ExchangeExtension<TType, UType>{}(lhs, Forward<UType>(rhs));
    }

    /// \brief Invoke the Swap function via extension functor.
    template <typename TType, typename UType>
    [[nodiscard]] auto
    InvokeSwapExtension(Mutable<TType> lhs, Mutable<TType> rhs)
        noexcept -> decltype(SwapExtension<TType, UType>{}(lhs, rhs))
    {
        return SwapExtension<TType, UType>{}(lhs, rhs);
    }

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Invoke the Exchange function, trying different implementations.
    template <typename TType, typename UType>
    constexpr decltype(auto)
    InvokeExchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
    {
        auto extension = [](auto&& lhs, auto&& rhs)
            -> decltype(InvokeExchangeExtension(lhs,
                                                Forward<decltype(rhs)>(rhs)))
        {
            return InvokeExchangeExtension(lhs, Forward<decltype(rhs)>(rhs));
        };

        auto member_function = [](auto&& lhs, auto&& rhs)
            -> decltype(lhs.Exchange(Forward<decltype(rhs)>(rhs)))
        {
            return lhs.Exchange(Forward<decltype(rhs)>(rhs));
        };

        auto non_member_function = [](auto&& lhs, auto&& rhs)
            -> decltype(Exchange(lhs, Forward<decltype(rhs)>(rhs)))
        {
            return Exchange(lhs, Forward<decltype(rhs)>(rhs));
        };

        auto fallback = [](auto&& lhs, auto&& rhs)
        {
            auto result = Move(lhs);

            lhs = Forward<decltype(rhs)>(rhs);

            return result;
        };

        return Templates::InvokeAny(extension,
                                    member_function,
                                    non_member_function,
                                    fallback)(lhs, rhs);
    }

    /// \brief Invoke the Swap function, trying different implementations.
    template <typename TType>
    constexpr decltype(auto)
    InvokeSwap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
    {
        auto extension = [](auto&& lhs, auto&& rhs)
            -> decltype(InvokeSwapExtension(lhs, rhs))
        {
            return InvokeSwapExtension(lhs, rhs);
        };

        auto member_function = [](auto&& lhs, auto&& rhs)
            -> decltype(lhs.Swap(rhs))
        {
            return lhs.Swap(rhs);
        };

        auto non_member_function = [](auto&& lhs, auto&& rhs)
            -> decltype(Swap(lhs, rhs))
        {
            return Swap(lhs, rhs);
        };

        auto fallback =[](auto&& lhs, auto&& rhs)
        {
            rhs = Toolset::Exchange(lhs, rhs);
        };

        return Templates::InvokeAny(extension,
                                    member_function,
                                    non_member_function,
                                    fallback)(lhs, rhs);
    }
}

// ===========================================================================

namespace Syntropy::Toolset
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    template <typename TType, typename UType>
    [[nodiscard]] constexpr TType
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
    {
        return ToolsetADL::InvokeExchange(lhs, Forward<UType>(rhs));
    }

    template <typename TType>
    constexpr void
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
    {
        ToolsetADL::InvokeSwap(lhs, rhs);
    }

}

// ===========================================================================
