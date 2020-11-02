
/// \file sequence_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of sequence-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATION                                                  */
    /************************************************************************/

    /// \brief A compile-time list of integers.
    template <Int... VSequence>
    struct Sequence;

}

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* UTILITIES                                                            */
    /************************************************************************/

    /// \brief Helper alias template used to generate a contiguous sequence of increasing integers, from 0 to VCount-1.
    template <Int VCount, Int... VSequence>
    struct MakeSequenceHelper : MakeSequenceHelper<VCount - 1, VCount - 1, VSequence...> {};

    /// \brief Specialization to end recursive definition.
    template <Int... VSequence>
    struct MakeSequenceHelper<0, VSequence...>
    {
        using Type = Sequence<VSequence...>;
    };

    /// \brief Helper alias template used to generate a contiguous sequence of increasing integers, from 0 to VCount-1.
    template <Int VCount>
    using MakeSequence = typename MakeSequenceHelper<VCount>::Type;

    /// \brief Helper alias template used to convert a parameter pack to an integer sequence of the same size.
    template <typename... TTypes>
    using SequenceFor = MakeSequence<sizeof...(TTypes)>;

    /************************************************************************/
    /* IS CONTIGUOUS SEQUENCE                                               */
    /************************************************************************/

    /// \brief Constant equal to true if TSequence is a monotonically increasing contiguous sequence, equal to false otherwise.
    template <typename TSequence>
    inline constexpr Bool IsContiguousSequence;

    /// \brief Partial specialization for sequences whose first two elements are contiguous.
    template <Int VFirst, Int VSecond, Int... VSequence>
    inline constexpr Bool IsContiguousSequence<Sequence<VFirst, VSecond, VSequence...>> = (VSecond == (VFirst+1)) && IsContiguousSequence<Sequence<VSecond, VSequence...>>;

    /// \brief Partial specialization for 1-sequences. True by definition.
    template <Int VLast>
    inline constexpr Bool IsContiguousSequence<Sequence<VLast>> = true;

}

// ===========================================================================