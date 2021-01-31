
/// \file sequence.h
/// \brief This header is part of Syntropy language module. It contains template definitions for sequences.
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/details/sequence_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief A compile-time list of integers.
    template <Int... VSequence>
    struct Sequence {};

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

    /// \brief Constant equal to true if TSequence is a monotonically increasing contiguous sequence, equal to false otherwise.
    template <typename TSequence>
    inline constexpr Bool IsContiguousSequence = Details::IsContiguousSequence<TSequence>;

    /// \brief Concatenate one or more sequences together.
    template <typename TSequence, typename... TSequences>
    using SequenceCat = Details::SequenceCat<TSequence, TSequences...>;

    /// \brief Create a sequence of a repeating value.
    template <Int VValue, Int VRepeat>
    using SequenceRepeat = Details::SequenceRepeat<VValue, VRepeat>;
}

// ===========================================================================
