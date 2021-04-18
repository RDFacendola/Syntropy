
/// \file to_string.details.h
///
/// \author Raffaele D. Facendola - April 2021

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/strings/details/strings.details.h"
#include "syntropy/core/strings/to_string_extensions.h"
#include "syntropy/core/strings/string.h"

// ===========================================================================

namespace Syntropy::Strings::Details
{
    /************************************************************************/
    /* TO STRING                                                            */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TType>
    auto
    InvokeToString(Immutable<TType> rhs, ExtensionPriority)
        noexcept -> decltype(Extensions::ToString<TType>{}(rhs));

    /// \brief Member-function.
    template <typename TType>
    auto
    InvokeToString(Immutable<TType> rhs, MemberFunctionPriority)
        noexcept -> decltype(rhs.ToString());

    /// \brief Non-member function, possibly using ADL.
    template <typename TType>
    auto
    InvokeToString(Immutable<TType> rhs, NonMemberFunctionPriority)
        noexcept -> decltype(ToString(rhs));

    /// \brief Fallback implementation for string literals.
    template <Int TSize>
    String
    InvokeToString(StringLiteral<TSize> rhs, FallbackPriority) noexcept;

    /// \brief Fallback implementation for booleans.
    template <Templates::IsBoolean TBoolean>
    Immutable<String>
    InvokeToString(TBoolean rhs, FallbackPriority) noexcept;

    /// \brief Fallback implementation for integrals.
    template <Templates::IsIntegral TIntegral>
    String
    InvokeToString(TIntegral rhs, FallbackPriority) noexcept;

    /// \brief Fallback implementation for reals.
    template <Templates::IsReal TReal>
    String
    InvokeToString(TReal rhs, FallbackPriority) noexcept;

    /// \brief Fallback implementation for everything else.
    template <typename TType>
    String
    InvokeToString(Immutable<TType> rhs, IllFormedPriority) noexcept;

    /// \brief Routes the invocation.
    template <typename TType>
    auto
    RouteToString(Immutable<TType> rhs)
        noexcept -> decltype(InvokeToString(rhs, Templates::kMaxPriority));

}

// ===========================================================================

#include "to_string.details.inl"

// ===========================================================================
