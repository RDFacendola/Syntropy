
/// \file rank.h
/// \brief This header is part of Syntropy language module. It contains type traits to determine object's rank.
///
/// \author Raffaele D. Facendola - November  2020

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/templates.h"
#include "syntropy/language/templates/manipulation.h"

#include "syntropy/language/templates/details/rank_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANK                                                                 */
    /************************************************************************/

    /// \brief Constant equal to the rank of TType. The program is ill-formed if TType's rank is meaningless.
    /// This trait is expected to be specialized by types having a "rank" concept.
    template <typename TType>
    inline constexpr Int RankImplementation = IllFormed<TType>;

    /// \brief Constant equal to the rank of TType. The program is ill-formed if TType's rank is meaningless.
    template <typename TType>
    inline constexpr Int Rank = RankImplementation<Decay<TType>>;

    /************************************************************************/
    /* SAME RANK                                                            */
    /************************************************************************/

    /// \brief Constant equal to true if all TTypes have the same rank, false otherwise.
    template <typename TType, typename... TTypes>
    inline constexpr Bool SameRank = Details::SameRank<Decay<TType>, Decay<TTypes>...>;
}

// ===========================================================================

