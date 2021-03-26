
/// \file support.details.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains shared implementation details.
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

}

// ===========================================================================
