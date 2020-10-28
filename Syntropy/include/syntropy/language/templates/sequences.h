
/// \file sequences.h
/// \brief This header is part of Syntropy language module. It contains query-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/sequences_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* INTEGER SEQUENCE                                                     */
    /************************************************************************/

    /// \brief A compile-time sequence of integers.
    template <Int... VSequence>
    struct IntegerSequence {};

    /************************************************************************/
    /* MAKE INTEGER SEQUENCE                                                */
    /************************************************************************/

    /// \brief Helper alias template used to generate a contiguous sequence of increasing integers, from 0 to VCount-1.
    /// \remarks If VCount is 0, this type is equal to an empty sequence.
    template <Int VCount>
    using MakeIntegerSequence = Details::MakeIntegerSequence<VCount>;

    /// \brief Helper alias template used to convert a parameter pack to an integer sequence of the same size.
    template <typename... TTypes>
    using IntegerSequenceFor = MakeIntegerSequence<sizeof...(TTypes)>;

    /************************************************************************/
    /* IS CONTIGUOUS SEQUENCE                                               */
    /************************************************************************/

    /// \brief Constant equal to true if the sequence [VInts...] is a monotonically increasing contiguous sequence, equal to false otherwise.
    template <Int... VSequence>
    constexpr Bool IsContiguousSequence = Details::IsContiguousSequence<VSequence...>;

}

// ===========================================================================