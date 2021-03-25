
/// \file compare.details.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains implementation details of compare.h.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

#include "syntropy/core/support/compare_extensions.h"

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
    /* ARE EQUAL                                                            */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   ExtensionPriority) noexcept
        -> decltype(Extensions::AreEqual<TType, UType>{}(lhs, rhs));

    /// \brief Member-function.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   MemberFunctionPriority) noexcept
        -> decltype(lhs.IsEqual(rhs));

    /// \brief Non-member function, possibly using ADL.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   NonMemberFunctionPriority) noexcept
        -> decltype(AreEqual(lhs, rhs));

    /// \brief Fallback.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   FallbackPriority) noexcept
        -> decltype(lhs == rhs);

    /// \brief Routes the invocation.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteAreEqual(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(InvokeAreEqual(lhs, rhs, kMaxPriority));

    /************************************************************************/
    /* ARE EQUIVALENT                                                       */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        ExtensionPriority) noexcept
        -> decltype(Extensions::AreEquivalent<TType, UType>{}(lhs, rhs));

    /// \brief Member-function.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        MemberFunctionPriority) noexcept
        -> decltype(lhs.IsEquivalent(rhs));

    /// \brief Non-member function, possibly using ADL.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        NonMemberFunctionPriority) noexcept
        -> decltype(AreEquivalent(lhs, rhs));

    /// \brief Fallback.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        FallbackPriority) noexcept
        -> decltype(Details::RouteAreEqual(lhs, rhs));

    /// \brief Routes the invocation.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteAreEquivalent(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(InvokeAreEquivalent(lhs, rhs, kMaxPriority));

    /************************************************************************/
    /* COMPARE                                                              */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  ExtensionPriority) noexcept
        -> decltype(Extensions::Compare<TType, UType>{}(lhs, rhs));

    /// \brief Member-function.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  MemberFunctionPriority) noexcept
        -> decltype(lhs.Compare(rhs));

    /// \brief Non-member function, possibly using ADL.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  NonMemberFunctionPriority) noexcept
        -> decltype(Compare(lhs, rhs));

    /// \brief Fallback.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  FallbackPriority) noexcept
        -> decltype(lhs <=> rhs);

    /// \brief Routes the invocation.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteCompare(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(InvokeCompare(lhs, rhs, kMaxPriority));

}

// ===========================================================================

#include "compare.details.inl"

// ===========================================================================
