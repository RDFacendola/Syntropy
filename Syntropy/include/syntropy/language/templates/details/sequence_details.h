
/// \file sequence_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of sequence-related type traits.
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

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

    // SequenceCat.
    // ============

    /// \brief Concatenate one or more sequences together.
    template <typename TSequence, typename... TSequences>
    struct SequenceCatHelper
    {
        using Types = typename SequenceCatHelper<TSequences...>::Type;

        using Type = typename SequenceCatHelper<TSequence, Types>::Type;
    };

    /// \brief Specialization for one sequence.
    template <Int... VSequence>
    struct SequenceCatHelper<Sequence<VSequence...>>
    {
        using Type = Sequence<VSequence...>;
    };

    /// \brief Specialization for two sequences.
    template <Int... VSequence, Int... USequence>
    struct SequenceCatHelper<Sequence<VSequence...>, Sequence<USequence...>>
    {
        using Type = Sequence<VSequence..., USequence...>;
    };

    /// \brief Concatenate zero or more sequences together.
    template <typename TSequence, typename... TSequences>
    using SequenceCat = typename SequenceCatHelper<TSequence, TSequences...>::Type;

    // SequenceRepeat.
    // ===============

    /// \brief Create a sequence of a repeating value.
    template <Int VValue, Int VRepeat>
    struct SequenceRepeatHelper
    {
        using Type = SequenceCat<Sequence<VValue>, typename SequenceRepeatHelper<VValue, VRepeat - 1>::Type>;
    };

    /// \brief Specialization for an empty sequence.
    template <Int VValue>
    struct SequenceRepeatHelper<VValue, 0>
    {
        using Type = Sequence<>;
    };

    /// \brief Create a sequence of a repeating value.
    template <Int VValue, Int VRepeat>
    using SequenceRepeat = typename SequenceRepeatHelper<VValue, VRepeat>::Type;

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