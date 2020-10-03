
/// \file sequences_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of sequence-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/templates.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS CONTIGUOUS SEQUENCE                                               */
    /************************************************************************/

    /// \brief Provide a member constant Value equal to true if the sequence [VInts...] is contiguous, equal to false otherwise.
    /// Partial specialization for non-contiguous sequences.
    template <Int... VInt>
    struct IsContiguousSequence : Templates::False
    {
    
    };

    /// \brief Provide a member constant Value equal to true if the sequence [VInt] is contiguous, equal to false otherwise.
    /// Partial specialization for sequences of one element.
    template <Int VInt>
    struct IsContiguousSequence<VInt> : Templates::True
    {

    };

    /// \brief Provide a member constant Value equal to true if the sequence [VInt, VInts...] is contiguous, equal to false otherwise.
    /// \brief Partial specialization for two-element sequences or more.
    template <Int VInt, Int... VInts>
    struct IsContiguousSequence<VInt, VInt + 1, VInts...> : IsContiguousSequence<VInt + 1, VInts...>
    {

    };

}