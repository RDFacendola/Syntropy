
/// \file rank_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of rank-related template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* SAME RANK                                                            */
    /************************************************************************/

    /// \brief Constant equal to true if all TTypes have the same rank, false otherwise.
    template <typename TType, typename... TTypes>
    inline constexpr Bool SameRank = (SameRank<TType, TTypes> && ...);

    /// \brief Specialization for two types.
    template <typename TType, typename UType>
    inline constexpr Bool SameRank<TType, UType> = (Rank<TType> == Rank<UType>);

    /// \brief Specialization for one type. Always true.
    template <typename TType>
    inline constexpr Bool SameRank<TType> = true;
}

// ===========================================================================