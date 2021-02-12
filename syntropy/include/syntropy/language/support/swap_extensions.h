
/// \file swap_extensions.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains definitions for swap-related customization point
///        objects.
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Algorithm::Extensions
{
    /************************************************************************/
    /* EXCHANGE                                                             */
    /************************************************************************/

    template <typename TType, typename UType, typename = void>
    struct Exchange;

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    template <typename TType, typename = void>
    struct Swap;
}

// ===========================================================================
