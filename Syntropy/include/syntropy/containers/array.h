
/// \file array.h
/// \brief This header is part of the Syntropy containers module. It contains wrappers for arrays.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <array>

#include "syntropy/core/types.h"

namespace syntropy
{
    /************************************************************************/
    /* ARRAY <TTYPE, KSIZE>                                                 */
    /************************************************************************/

        /// \brief Alias type for std::array.
    template <typename TType, Int kSize>
    using Array = std::array<TType, static_cast<std::size_t>(kSize)>;

}
