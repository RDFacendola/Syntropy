
/// \file compare.details.inl
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

// ===========================================================================

namespace Syntropy::Algorithm::Details
{
    /************************************************************************/
    /* ARE EQUAL                                                            */
    /************************************************************************/

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   ExtensionPriority) noexcept
        -> decltype(Extensions::AreEqual<TType, UType>{}(lhs, rhs))
    {
        return Extensions::AreEqual<TType, UType>{}(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   MemberFunctionPriority) noexcept
        -> decltype(lhs.IsEqual(rhs))
    {
        return lhs.IsEqual(rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   NonMemberFunctionPriority) noexcept
        -> decltype(AreEqual(lhs, rhs))
    {
        return AreEqual(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   FallbackPriority) noexcept
        -> decltype(lhs == rhs)
    {
        return (lhs == rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteAreEqual(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(InvokeAreEqual(lhs, rhs, kMaxPriority))
    {
        return InvokeAreEqual(lhs, rhs, kMaxPriority);
    }

    /************************************************************************/
    /* ARE EQUIVALENT                                                       */
    /************************************************************************/

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        ExtensionPriority) noexcept
        -> decltype(Extensions::AreEquivalent<TType, UType>{}(lhs, rhs))
    {
        return Extensions::AreEquivalent<TType, UType>{}(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        MemberFunctionPriority) noexcept
        -> decltype(lhs.IsEquivalent(rhs))
    {
        return lhs.IsEquivalent(rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        NonMemberFunctionPriority) noexcept
        -> decltype(AreEquivalent(lhs, rhs))
    {
        return AreEquivalent(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        FallbackPriority) noexcept
        -> decltype(Details::AreEqual(lhs, rhs))
    {
        return Details::AreEqual(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteAreEquivalent(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(InvokeAreEquivalent(lhs, rhs, kMaxPriority))
    {
        return InvokeAreEquivalent(lhs, rhs, kMaxPriority);
    }

    /************************************************************************/
    /* COMPARE                                                              */
    /************************************************************************/

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  ExtensionPriority) noexcept
        -> decltype(Extensions::Compare<TType, UType>{}(lhs, rhs))
    {
        return Extensions::Compare<TType, UType>{}(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  MemberFunctionPriority) noexcept
        -> decltype(lhs.Compare(rhs))
    {
        return lhs.Compare(rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  NonMemberFunctionPriority) noexcept
        -> decltype(Compare(lhs, rhs))
    {
        return Compare(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  FallbackPriority) noexcept
        -> decltype(lhs <=> rhs)
    {
        return (lhs <=> rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteCompare(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(InvokeCompare(lhs, rhs, kMaxPriority))
    {
        return InvokeCompare(lhs, rhs, kMaxPriority);
    }

}

// ===========================================================================
