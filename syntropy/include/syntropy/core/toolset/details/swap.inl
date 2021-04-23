
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
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Invoke the Exchange function, trying different implementations.
    template <typename TType, typename UType>
    constexpr decltype(auto)
    InvokeExchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
    {
        using Toolset::Extensions::Exchange;

        auto extension = [](auto& lhs, auto&& rhs)
            -> decltype(Exchange<decltype(lhs), decltype(rhs)>{}(lhs, rhs))
        {
            return Exchange<decltype(lhs), decltype(rhs)>{}(lhs, rhs);
        };

        auto member_function = [](auto& lhs, auto&& rhs)
            -> decltype(lhs.Exchange(Forward<decltype(rhs)>(rhs)))
        {
            return lhs.Exchange(Forward<decltype(rhs)>(rhs));
        };

        auto non_member_function = [](auto& lhs, auto&& rhs)
            -> decltype(Exchange(lhs, Forward<decltype(rhs)>(rhs)))
        {
            return Exchange(lhs, Forward<decltype(rhs)>(rhs));
        };

        auto fallback = [](auto& lhs, auto&& rhs)
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
        using Toolset::Extensions::Swap;

        auto extension = [](auto& lhs, auto& rhs)
            -> decltype(Swap<decltype(lhs)>{}(lhs, rhs))
        {
            return Swap<decltype(lhs)>{}(lhs, rhs);
        };

        auto member_function = [](auto& lhs, auto& rhs)
            -> decltype(lhs.Swap(rhs))
        {
            return lhs.Swap(rhs);
        };

        auto non_member_function = [](auto& lhs, auto& rhs)
            -> decltype(Swap(lhs, rhs))
        {
            return Swap(lhs, rhs);
        };

        auto fallback =[](auto& lhs, auto& rhs)
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
    [[nodiscard]] constexpr decltype(auto)
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
    {
        return ToolsetADL::InvokeExchange(lhs, Forward<UType>(rhs));
    }

    template <typename TType>
    constexpr decltype(auto)
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
    {
        return ToolsetADL::InvokeSwap(lhs, rhs);
    }

}

// ===========================================================================
