
/// \file record.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for record concepts and related
///        functionalities.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/sequence.h"

#include "syntropy/core/support/compare.h"
#include "syntropy/core/support/swap.h"
#include "syntropy/core/support/compare.h"

#include "syntropy/math/math.h"

// ===========================================================================

#include "details/record.details.h"

// ===========================================================================

namespace Syntropy::Records::Templates
{
    /************************************************************************/
    /* RECORD                                                               */
    /************************************************************************/

    /// \brief Exposes a member kValue equal to the rank of a record.
    template <typename TRecord>
    struct RecordRankTrait
    {
        // Int kValue = <record rank>
    };

}

// ===========================================================================

namespace Syntropy::Records
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Record.
    // =======

    /// \brief Access an element in a record by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename TRecord>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TRecord> sequence) noexcept
        -> decltype(Details::RouteGet<TIndex>(Forward<TRecord>(sequence)));

}

// ===========================================================================

namespace Syntropy::Records::Concepts
{
    /************************************************************************/
    /* RECORD                                                               */
    /************************************************************************/

    /// \brief Concept for a fixed-size record, for which indexed
    ///        compile-time access to its elements is provided.
    template <typename TRecord>
    concept Record = requires
    {
        /// \brief Rank of the sequence.
        { Records::Templates::RecordRankTrait<TRecord>::kValue }
            -> Syntropy::Concepts::IsIntegral;
    };

    /// \brief Concept for a Record that can be perfectly-forwarded.
    template <typename TRecord>
    concept ForwardingRecord
        = Record<Syntropy::Templates::UnqualifiedOf<TRecord>>;

}

// ===========================================================================

namespace Syntropy::Records::Templates
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief Rank of a sequence.
    template <Concepts::Record TRecord>
    inline constexpr Int RecordRankOf =
        RecordRankTrait<TRecord>::kValue;

    /// \brief Constant equal to true if two sequeneces have the same rank,
    ///        equal to false otherwise.
    template <Concepts::Record TRecord, Concepts::Record URecord>
    inline constexpr Bool RecordSameRank =
        (RecordRankOf<TRecord> == RecordRankOf<URecord>);

    /// \brief Type of a sequence element.
    template <Int TIndex, Concepts::Record TRecord>
    using RecordElementTypeOf = Syntropy::Templates::UnqualifiedOf<decltype(
        Records::Get<TIndex>(Syntropy::Templates::Declval<TRecord>()))>;

    /// \brief Generates a numeric sequence that can be used to enumerate all
    ///        elements in a given sequence.
    template <Concepts::Record TRecord>
    using RecordEnumerationOf =
        Syntropy::Templates::MakeSequence<RecordRankOf<TRecord>>;

}

// ===========================================================================

namespace Syntropy::Records
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Invoke a function with arguments provided in form of n-tuple.
    template <typename TFunction, Concepts::ForwardingRecord TRecord>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TRecord> ntuple) noexcept;

    /// \brief Invoke a function to each element in a n-tuple individually.
    template <typename TFunction, Concepts::ForwardingRecord TRecord>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TRecord> ntuple) noexcept;

    /// \brief Invoke a function to the TIndex-th element of each provided
    ///        tuple at once.
    template <Int TIndex,
              typename TFunction,
              Concepts::ForwardingRecord... TRecords>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TRecords>... tuples) noexcept;

    /// \brief Invoke a function to each argument list generated by projecting
    ///        the i-th element of all the provided tuples at once,
    ///        for each index up to the minimum rank among those tuples.
    template <typename TFunction, Concepts::ForwardingRecord... TRecords>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TRecords>... tuples) noexcept;

    /// \brief Create a new instance of type TType using TRecord as constructor
    ///        arguments.
    template <typename TType, Concepts::ForwardingRecord TRecord>
    [[nodiscard]] constexpr TType
    MakeFromTuple(Forwarding<TRecord> tuple) noexcept;
}

// ===========================================================================

namespace Syntropy::Algorithm::Extensions
{
    /************************************************************************/
    /* SWAP EXTENSIONS                                                      */
    /************************************************************************/

    /// \brief Swap two elements.
    template <Records::Concepts::Record TRecord>
    struct Swap<TRecord>
    {
        constexpr void
        operator()(Mutable<TRecord> lhs, Mutable<TRecord> rhs)
        const noexcept;
    };

    /// \brief Swap two sequences and return the old value of the first.
    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    struct Exchange<TRecord, URecord>
    {
        constexpr TRecord
        operator()(Mutable<TRecord> lhs, Immutable<URecord> rhs)
        const noexcept;

        constexpr TRecord
        operator()(Mutable<TRecord> lhs, Movable<URecord> rhs)
        const noexcept;
    };

    /************************************************************************/
    /* COMPARE EXTENSIONS                                                   */
    /************************************************************************/

    /// \brief Check whether two sequences are equal.
    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    struct AreEqual<TRecord, URecord>
    {
        [[nodiscard]] constexpr Bool
        operator()(Immutable<TRecord> lhs, Immutable<URecord> rhs)
        const noexcept;
    };

    /// \brief Check whether two sequences are equivalent.
    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    struct AreEquivalent<TRecord, URecord>
    {
        [[nodiscard]] constexpr Bool
        operator()(Immutable<TRecord> lhs, Immutable<URecord> rhs)
        const noexcept;
    };

    /// \brief Compare two sequences.
    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    struct Compare<TRecord, URecord>
    {
        [[nodiscard]] constexpr Ordering
        operator()(Immutable<TRecord> lhs, Immutable<URecord> rhs)
        const noexcept;
    };

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Rank of a Record.
    template <Syntropy::Records::Concepts::Record TRecord>
    struct std::tuple_size<TRecord>;

    /// \brief Type of a sequence element, by index.
    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    struct std::tuple_element<TIndex, TRecord>;

    /// \brief Access a sequence element, by index.
    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    decltype(auto) get(Syntropy::Immutable<TRecord> tuple);

    /// \brief Access a sequence element, by index..
    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    decltype(auto) get(Syntropy::Movable<TRecord> tuple);
}

// ===========================================================================

#include "details/record.inl"

// ===========================================================================
