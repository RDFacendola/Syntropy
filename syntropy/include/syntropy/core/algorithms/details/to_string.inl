
/// \file to_string.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

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
    constexpr auto
    ToString(Immutable<TType> rhs) noexcept
        -> decltype(Details::RouteToString(rhs))
    {
        Details::RouteToString(rhs);
    }

}

// ===========================================================================
