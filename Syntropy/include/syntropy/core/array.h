
/// \file array.h
/// \brief This header is part of the Syntropy core module. It contains array definitions.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <array>

#include "syntropy/core/types.h"

namespace Syntropy
{
    /************************************************************************/
    /* VECTOR <TYPE, SIZE>                                                  */
    /************************************************************************/

    /// \brief Alias type for a fixed-size array.
    template <typename TType, Int size>
    using Array = std::array<TType, size>;

}
