
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
