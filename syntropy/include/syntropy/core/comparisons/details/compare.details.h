
/// \file compare.details.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains implementation details of compare.h.
///
/// \author Raffaele D. Facendola - November 2020

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/comparisons/compare_extensions.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Comparisons::Details
{
    /************************************************************************/
    /* CUSTOMIZATION-POINT-OBJECTS                                          */
    /************************************************************************/

    /// \brief Priority of a custom extension implementation.
    using ExtensionPriority = Templates::ExtensionPriority;

    /// \brief Priority of a member-function implementation.
    using MemberFunctionPriority = Templates::MemberFunctionPriority;

    /// \brief Priority of a member-operator implementation.
    using MemberOperatorPriority = Templates::MemberOperatorPriority;

    /// \brief Priority of a non-member-function implementation.
    using NonMemberFunctionPriority = Templates::NonMemberFunctionPriority;

    /// \brief Priority of a fallback implementation.
    using FallbackPriority = Templates::FallbackPriority;

    /// \brief Priority of a ill-formed implementation.
    using IllFormedPriority = Templates::IllFormedPriority;

    /// \brief Highest priority among method implementations.
    inline constexpr
    Templates::Priority kMaxPriority = Templates::kMaxPriority;

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

    /// \brief Ill-formed.
    template <typename TType, typename UType>
    constexpr void
    InvokeAreEqual(Immutable<TType> lhs,
                   Immutable<UType> rhs,
                   IllFormedPriority) noexcept;

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
        -> decltype(lhs == rhs);

    /// \brief Ill-formed.
    template <typename TType, typename UType>
    constexpr void
    InvokeAreEquivalent(Immutable<TType> lhs,
                        Immutable<UType> rhs,
                        IllFormedPriority) noexcept;

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

    /// \brief Ill-formed.
    template <typename TType, typename UType>
    constexpr void
    InvokeCompare(Immutable<TType> lhs,
                  Immutable<UType> rhs,
                  IllFormedPriority) noexcept;

    /// \brief Routes the invocation.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    RouteCompare(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(InvokeCompare(lhs, rhs, kMaxPriority));

}

// ===========================================================================

#include "compare.details.inl"

// ===========================================================================
