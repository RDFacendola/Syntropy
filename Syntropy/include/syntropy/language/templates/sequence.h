
/// \file sequence.h
/// \brief This header is part of Syntropy language module. It contains template definitions for sequences.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/sequence_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief A compile-time sequence of integers.
    template <Int... VSequence>
    using Sequence = Details::Sequence<VSequence...>;

    /************************************************************************/
    /* UTILITIES                                                            */
    /************************************************************************/

    /// \brief Helper alias template used to generate a contiguous sequence of increasing integers, from 0 to VCount-1.
    /// \remarks If VCount is 0, this type is equal to an empty sequence.
    template <Int VCount>
    using MakeSequence = Details::MakeSequence<VCount>;

    /// \brief Helper alias template used to convert a parameter pack to an integer sequence of the same size.
    template <typename... TTypes>
    using SequenceFor = Details::SequenceFor<TTypes...>;

    /// \brief Constant equal to true if the sequence [VInts...] is a monotonically increasing contiguous sequence, equal to false otherwise.
    template <Int... VSequence>
    constexpr Bool IsContiguousSequence = Details::IsContiguousSequence<VSequence...>;

}

// ===========================================================================