
/// \file swap.inl
///
/// \author Raffaele D. Facendola - February 2021.

// ===========================================================================

#include "syntropy/language/support/invoke.h"

// ===========================================================================

namespace Syntropy::SupportADL
{
    /************************************************************************/
    /* SWAP EXTENSIONS                                                      */
    /************************************************************************/

    /// \brief Type alias for the AreEqual extension functor.
    template <typename TType, typename UType>
    using ExchangeExtension
        = Support::Extensions::Exchange<TType, UType>;

    /// \brief Type alias for the AreEquivalent extension functor.
    template <typename TType, typename UType>
    using SwapExtension
        = Support::Extensions::Swap<TType, UType>;

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

        return Support::InvokeAny(extension,
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
            rhs = Support::Exchange(lhs, rhs);
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function,
                                  fallback)(lhs, rhs);
    }
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TOOLSET                                                              */
    /************************************************************************/

    template <typename TType, typename UType>
    [[nodiscard]] constexpr TType
    Support
    ::Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept
    {
        return SupportADL::InvokeExchange(lhs, Forward<UType>(rhs));
    }

    template <typename TType>
    constexpr void
    Support
    ::Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept
    {
        SupportADL::InvokeSwap(lhs, rhs);
    }

}

// ===========================================================================
