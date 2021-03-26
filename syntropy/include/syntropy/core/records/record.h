
/// \file record.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for N-tuples concepts and related
///        functionalities.
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/support/compare.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/record.h"

#include "syntropy/core/support/swap.h"
#include "syntropy/core/support/compare.h"

#include "syntropy/math/math.h"

// ===========================================================================

#include "details/record.details.h"

// ===========================================================================

namespace Syntropy::Sequences::Templates
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief Exposes a member kValue equal to the rank of a sequence.
    template <typename TSequence>
    struct SequenceRankTrait
    {
        // Int kValue = <sequence rank>
    };

}

// ===========================================================================

namespace Syntropy::Sequences
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Sequence.
    // =========

    /// \brief Access an element in a sequence by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename TSequence>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TSequence> sequence) noexcept
        -> decltype(Details::RouteGet<TIndex>(Forward<TSequence>(sequence)));

}

// ===========================================================================

namespace Syntropy::Sequences::Concepts
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief Concept for a limited-size sequence, for which indexed
    ///        compile-time access to its elements is provided.
    template <typename TSequence>
    concept Sequence = requires
    {
        /// \brief Rank of the sequence.
        { Sequences::Templates::SequenceRankTrait<TSequence>::kValue }
            -> Syntropy::Concepts::IsIntegral;
    };

    /// \brief Concept for a Sequence that can be perfectly-forwarded.
    template <typename TSequence>
    concept ForwardingSequence
        = Sequence<Syntropy::Templates::UnqualifiedOf<TSequence>>;

}

// ===========================================================================

namespace Syntropy::Sequences::Templates
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief Rank of a sequence.
    template <Concepts::Sequence TSequence>
    inline constexpr Int SequenceRankOf =
        SequenceRankTrait<TSequence>::kValue;

    /// \brief Constant equal to true if two sequeneces have the same rank,
    ///        equal to false otherwise.
    template <Concepts::Sequence TSequence, Concepts::Sequence USequence>
    inline constexpr Bool SequenceSameRank =
        (SequenceRankOf<TSequence> == SequenceRankOf<USequence>);

    /// \brief Type of a sequence element.
    template <Int TIndex, Concepts::Sequence TSequence>
    using SequenceElementTypeOf = Syntropy::Templates::UnqualifiedOf<decltype(
        Sequences::Get<TIndex>(Syntropy::Templates::Declval<TSequence>()))>;

    /// \brief Generates a numeric sequence that can be used to enumerate all
    ///        elements in a given sequence.
    template <Concepts::Sequence TSequence>
    using SequenceEnumerationOf =
        Syntropy::Templates::MakeSequence<SequenceRankOf<TSequence>>;

}

// ===========================================================================

namespace Syntropy::Sequences
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Invoke a function with arguments provided in form of n-tuple.
    template <typename TFunction, Concepts::ForwardingSequence TSequence>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TSequence> ntuple) noexcept;

    /// \brief Invoke a function to each element in a n-tuple individually.
    template <typename TFunction, Concepts::ForwardingSequence TSequence>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TSequence> ntuple) noexcept;

    /// \brief Invoke a function to the TIndex-th element of each provided
    ///        tuple at once.
    template <Int TIndex,
              typename TFunction,
              Concepts::ForwardingSequence... TSequences>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TSequences>... tuples) noexcept;

    /// \brief Invoke a function to each argument list generated by projecting
    ///        the i-th element of all the provided tuples at once,
    ///        for each index up to the minimum rank among those tuples.
    template <typename TFunction, Concepts::ForwardingSequence... TSequences>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TSequences>... tuples) noexcept;

    /// \brief Create a new instance of type TType using TSequence as constructor
    ///        arguments.
    template <typename TType, Concepts::ForwardingSequence TSequence>
    [[nodiscard]] constexpr TType
    MakeFromTuple(Forwarding<TSequence> tuple) noexcept;
}

// ===========================================================================

namespace Syntropy::Algorithm::Extensions
{
    /************************************************************************/
    /* SWAP EXTENSIONS                                                      */
    /************************************************************************/

    /// \brief Swap two elements.
    template <Sequences::Concepts::Sequence TSequence>
    struct Swap<TSequence>
    {
        constexpr void
        operator()(Mutable<TSequence> lhs, Mutable<TSequence> rhs)
        const noexcept;
    };

    /// \brief Swap two sequences and return the old value of the first.
    template <Sequences::Concepts::Sequence TSequence,
              Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    struct Exchange<TSequence, USequence>
    {
        constexpr TSequence
        operator()(Mutable<TSequence> lhs, Immutable<USequence> rhs)
        const noexcept;

        constexpr TSequence
        operator()(Mutable<TSequence> lhs, Movable<USequence> rhs)
        const noexcept;
    };

    /************************************************************************/
    /* COMPARE EXTENSIONS                                                   */
    /************************************************************************/

    /// \brief Check whether two sequences are equal.
    template <Sequences::Concepts::Sequence TSequence,
              Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    struct AreEqual<TSequence, USequence>
    {
        [[nodiscard]] constexpr Bool
        operator()(Immutable<TSequence> lhs, Immutable<USequence> rhs)
        const noexcept;
    };

    /// \brief Check whether two sequences are equivalent.
    template <Sequences::Concepts::Sequence TSequence,
              Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    struct AreEquivalent<TSequence, USequence>
    {
        [[nodiscard]] constexpr Bool
        operator()(Immutable<TSequence> lhs, Immutable<USequence> rhs)
        const noexcept;
    };

    /// \brief Compare two sequences.
    template <Sequences::Concepts::Sequence TSequence,
              Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    struct Compare<TSequence, USequence>
    {
        [[nodiscard]] constexpr Ordering
        operator()(Immutable<TSequence> lhs, Immutable<USequence> rhs)
        const noexcept;
    };

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Rank of a Sequence.
    template <Syntropy::Sequences::Concepts::Sequence TSequence>
    struct std::tuple_size<TSequence>;

    /// \brief Type of a sequence element, by index.
    template <std::size_t TIndex,
              Syntropy::Sequences::Concepts::Sequence TSequence>
    struct std::tuple_element<TIndex, TSequence>;

    /// \brief Access a sequence element, by index.
    template <std::size_t TIndex,
              Syntropy::Sequences::Concepts::Sequence TSequence>
    decltype(auto) get(Syntropy::Immutable<TSequence> tuple);

    /// \brief Access a sequence element, by index..
    template <std::size_t TIndex,
              Syntropy::Sequences::Concepts::Sequence TSequence>
    decltype(auto) get(Syntropy::Movable<TSequence> tuple);
}

// ===========================================================================

#include "details/record.inl"

// ===========================================================================
