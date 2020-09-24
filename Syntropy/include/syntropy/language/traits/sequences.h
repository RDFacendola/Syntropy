
/// \file sequences.h
/// \brief This header is part of Syntropy language module. It contains query-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/traits/details/sequences.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* IS CONTIGUOUS SEQUENCE                                               */
    /************************************************************************/

    /// \brief Constant equal to true if the sequence [VInts...] is contiguous.
    template <Int... VInts>
    constexpr Bool IsContiguousSequence = Details::IsContiguousSequence<VInts...>::kValue;

}
