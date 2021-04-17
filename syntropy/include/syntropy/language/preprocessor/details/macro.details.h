
/// \file macro.details.h
///
/// \author Raffaele D. Facendola - April 2021

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

    template <typename TVoid, typename TTypename>
    struct ExpandTypename<TVoid(TTypename)>
        : Templates::Alias<TTypename> {};

}

// ===========================================================================
