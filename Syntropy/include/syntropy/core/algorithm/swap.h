
/// \file swap.h
/// \brief This header is part of Syntropy core module. It contains definitions for swapping functionalities.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Swap lhs with rhs.
    template <typename TType>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Swap.
    // =====

    template <typename TType>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept
    {
        auto xhs = Move(lhs);

        lhs = Move(rhs);
        rhs = Move(xhs);
    }
}

// ===========================================================================
