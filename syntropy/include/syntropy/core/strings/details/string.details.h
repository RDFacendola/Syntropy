
/// \file string.details.h
///
/// \author Raffaele D. Facendola - April 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

#include "syntropy/core/strings/string_extensions.h"

// ===========================================================================

namespace Syntropy::Strings::Details
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

    /// \brief Highest priority among method implementations.
    inline constexpr
    Templates::Priority kMaxPriority = Templates::kMaxPriority;

    /************************************************************************/
    /* TO STRING                                                            */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TType>
    constexpr auto
    InvokeToString(Immutable<TType> rhs, ExtensionPriority)
        noexcept -> decltype(Extensions::ToString<TType>{}(rhs));

    /// \brief Member-function.
    template <typename TType>
    constexpr auto
    InvokeToString(Immutable<TType> rhs, MemberFunctionPriority)
        noexcept -> decltype(rhs.ToString());

    /// \brief Non-member function, possibly using ADL.
    template <typename TType>
    constexpr auto
    InvokeToString(Immutable<TType> rhs, NonMemberFunctionPriority)
        noexcept -> decltype(ToString(rhs));

    /// \brief Routes the invocation.
    template <typename TType>
    constexpr auto
    RouteToString(Immutable<TType> rhs)
        noexcept -> decltype(InvokeToString(rhs, kMaxPriority));

}

// ===========================================================================

#include "string.details.inl"

// ===========================================================================
