
/// \file string.details.h
///
/// \author Raffaele D. Facendola - April 2021

#pragma once

// ===========================================================================

namespace Syntropy::Strings::Details
{
    /************************************************************************/
    /* TO STRING                                                            */
    /************************************************************************/

    template <typename TType>
    constexpr auto
    InvokeToString(Immutable<TType> rhs, ExtensionPriority)
        noexcept -> decltype(Extensions::ToString<TType>{}(rhs))
    {
        return Extensions::ToString<TType>{}(rhs);
    }

    template <typename TType>
    constexpr auto
    InvokeToString(Immutable<TType> rhs, MemberFunctionPriority)
        noexcept -> decltype(rhs.ToString()))
    {
        return rhs.ToString();
    }

    template <typename TType>
    constexpr auto
    InvokeToString(Immutable<TType> rhs, NonMemberFunctionPriority)
        noexcept -> decltype(ToString(rhs))
    {
        return ToString(rhs);
    }

    template <typename TType>
    constexpr auto
    RouteToString(Mutable<TType> lhs, Mutable<TType> rhs)
        noexcept -> decltype(InvokeToString(rhs, kMaxPriority))
    {
        return InvokeToString(rhs, kMaxPriority);
    }

}

// ===========================================================================
