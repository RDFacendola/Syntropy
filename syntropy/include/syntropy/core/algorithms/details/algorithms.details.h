
/// \file support.details.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains shared implementation details.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

#include "syntropy/core/algorithms/compare_extensions.h"

// ===========================================================================

namespace Syntropy::Algorithms::Details
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
