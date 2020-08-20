
/// \file vector.h
/// \brief This header is part of the Syntropy containers module. It contains wrappers for vectors.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <vector>

#include "syntropy/experimental/core/polymorphic_allocator.h"

namespace Syntropy
{
    /************************************************************************/
    /* VECTOR <TTYPE>                                                       */
    /************************************************************************/

    /// \brief Alias type for std::vector with polymorphic allocator type.
    template <typename TType>
    using Array = std::vector<TType, PolymorphicAllocator<TType>>;

}
