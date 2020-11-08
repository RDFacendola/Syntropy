
/// \file swap.h
/// \brief This header is part of Syntropy language module. It contains definitions for swapping functionalities.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Swap lhs with rhs, accounting for any member function TType::Swap, if present.
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
        if constexpr (Templates::HasMemberSwap<TType>)
        {
            lhs.Swap(rhs);
        }
        else
        {
            auto xhs = Move(lhs);

            lhs = Move(rhs);
            rhs = Move(xhs);
        }
    }
}

// ===========================================================================
