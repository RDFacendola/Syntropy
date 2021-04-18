
/// \file strings.details.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains shared implementation details of strings.
///
/// \author Raffaele D. Facendola - April 2021

// ===========================================================================

#pragma once

#include "syntropy/language/templates/priority.h"

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

}

// ===========================================================================
