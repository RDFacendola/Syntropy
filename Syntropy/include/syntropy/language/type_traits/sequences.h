
/// \file sequences.h
/// \brief This header is part of Syntropy language module. It contains query-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/type_traits/details/sequences.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* IS CONTIGUOUS SEQUENCE                                               */
    /************************************************************************/

    /// \brief Constant equal to true if the sequence [kInts...] is contiguous.
    template <Int... kInts>
    constexpr Bool IsContiguousSequence = Details::IsContiguousSequence<kInts...>::Value;

}
