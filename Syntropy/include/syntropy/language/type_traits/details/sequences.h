
/// \file sequences.h
/// \brief This header is part of Syntropy language module. It contains implementation details of sequence-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/type_traits/constants.h"

namespace Syntropy::Traits::Details
{
    /************************************************************************/
    /* IS CONTIGUOUS SEQUENCE                                               */
    /************************************************************************/

    /// \brief Provide a member constant Value equal to true if the sequence [kInts...] is contiguous, equal to false otherwise.
    /// Partial specialization for non-contiguous sequences.
    template <Int... kInts>
    struct IsContiguousSequence : False
    {
    
    };

    /// \brief Provide a member constant Value equal to true if the sequence [kInt] is contiguous, equal to false otherwise.
    /// Partial specialization for sequences of one element.
    template <Int kInt>
    struct IsContiguousSequence<kInt> : True
    {

    };

    /// \brief Provide a member constant Value equal to true if the sequence [kInt, kInts...] is contiguous, equal to false otherwise.
    /// \brief Partial specialization for two-element sequences or more.
    template <Int kInt, Int... kInts>
    struct IsContiguousSequence<kInt, kInt + 1, kInts...> : IsContiguousSequence<kInt + 1, kInts...>
    {

    };

}