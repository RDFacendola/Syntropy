
/// \file to_string.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions to stringify objects.
///
/// \author Raffaele D. Facendola - February 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* STRINGS EXTENSIONS                                                   */
    /************************************************************************/

    namespace Strings::Extensions
    {
        /// \brief Convert an object to a string.
        template <typename TType, typename = void>
        struct ToString;
    }

    /************************************************************************/
    /* STRINGS                                                              */
    /************************************************************************/

    namespace Strings
    {
        /// \brief Convert rhs to a string.
        template <typename TType>
        [[nodiscard]] constexpr decltype(auto)
        ToString(Immutable<TType> rhs) noexcept;
    }
}

// ===========================================================================

#include "details/to_string.inl"

// ===========================================================================
