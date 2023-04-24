
/// \file preprocessor.details.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/templates/templates.h"

// ===========================================================================

namespace Syntropy::Macros::Details
{
    /************************************************************************/
    /* EXPAND TYPENAME                                                      */
    /************************************************************************/

    /// \brief Provide a member type alias equal to TTypename.`
    template <typename TTypename>
    struct ExpandTypename {};

    /// \brief Template specialization for void types.
    template <typename TVoid>
    struct ExpandTypename<TVoid()>
        : Templates::Alias<void> {};

    /// \brief Template specialization for routine types.
    template <typename TVoid, typename TTypename>
    struct ExpandTypename<TVoid(TTypename)>
        : Templates::Alias<TTypename> {};

}

// ===========================================================================
