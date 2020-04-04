
/// \file vector.h
/// \brief This header is part of the syntropy containers. It contains wrappers for vectors.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <vector>

#include "syntropy/allocators/polymorphic_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* VECTOR <TTYPE>                                                       */
    /************************************************************************/

    /// \brief Alias type for std::vector with polymorphic allocator type.
    template <typename TType>
    using Vector = std::vector<TType, PolymorphicAllocator<TType>>;

}
