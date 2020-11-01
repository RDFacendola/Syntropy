
/// \file rank.h
/// \brief This header is part of Syntropy language module. It contains type traits to determine object's rank.
///
/// \author Raffaele D. Facendola - November  2020

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/templates.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANK                                                                 */
    /************************************************************************/

    /// \brief Constant equal to the rank of TType. The program is ill-formed if TType's rank is meaningless.
    template <typename TType>
    inline constexpr Int Rank = IllFormed<TType>;

}

// ===========================================================================

