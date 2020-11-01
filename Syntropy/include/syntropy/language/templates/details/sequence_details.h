
/// \file sequence_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of sequence-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/templates.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <Int... VIndexes>
    struct IntegerSequence;
}

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* MAKE INTEGER SEQUENCE                                                */
    /************************************************************************/

    /// \brief Helper alias template used to generate a contiguous sequence of increasing integers, from 0 to VCount-1.
    template <Int VCount, Int... VSequence>
    struct MakeIntegerSequenceHelper : MakeIntegerSequenceHelper<VCount - 1, VCount - 1, VSequence...> {};

    /// \brief Specialization to end recursive definition.
    template <Int... VSequence>
    struct MakeIntegerSequenceHelper<0, VSequence...> : Alias<IntegerSequence<VSequence...>> {};

    /// \brief Helper alias template used to generate a contiguous sequence of increasing integers, from 0 to VCount-1.
    template <Int VCount>
    using MakeIntegerSequence = typename MakeIntegerSequenceHelper<VCount>::Type;

    /************************************************************************/
    /* IS CONTIGUOUS SEQUENCE                                               */
    /************************************************************************/

    /// \brief Constant equals to true if VSequence... is a monotonically increasing contiguous sequence, equals to false otherwise.
    template <Int... VSequence>
    inline constexpr Bool IsContiguousSequence = false;

    /// \brief Partial specialization for sequences whose first two elements are contiguous.
    template <Int VFirst, Int VSecond, Int... VSequence>
    inline constexpr Bool IsContiguousSequence<VFirst, VSecond, VSequence...> = (VSecond == (VFirst+1)) && IsContiguousSequence<VSecond, VSequence...>;

    /// \brief Partial specialization for 1-sequences. True by definition.
    template <Int VLast>
    inline constexpr Bool IsContiguousSequence<VLast> = true;

}

// ===========================================================================