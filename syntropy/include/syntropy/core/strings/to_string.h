
/// \file to_string.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions to stringify objects.
///
/// \author Raffaele D. Facendola - February 2021

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

#include "details/to_string.details.h"

// ===========================================================================

namespace Syntropy::Algorithms
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // ToString.
    // =========

    /// \brief Convert rhs to a string.
    template <typename TType>
    auto
    ToString(Immutable<TType> rhs) noexcept
        -> decltype(Details::RouteToString(rhs));

}

// ===========================================================================

#include "details/to_string.inl"

// ===========================================================================
