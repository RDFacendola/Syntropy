
/// \file core.h
/// \brief THIS HEADER IS TEMPORARY.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

namespace Syntropy
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    // Swap both lhs and rhs.
    template <typename TType>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept;

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


