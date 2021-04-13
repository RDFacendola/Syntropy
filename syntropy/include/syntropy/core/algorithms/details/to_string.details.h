
/// \file to_string.details.h
///
/// \author Raffaele D. Facendola - April 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/algorithms/details/algorithms.details.h"

#include "syntropy/core/algorithms/to_string_extensions.h"

// ===========================================================================

namespace Syntropy::Algorithms::Details
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

#include "to_string.details.inl"

// ===========================================================================
