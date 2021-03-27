
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

#include "syntropy/core/algorithms/swap.h"
#include "syntropy/core/algorithms/compare.h"

#include "syntropy/core/foundation/ordering.h"

#include "syntropy/math/math.h"

// ===========================================================================

#include "details/record.details.h"

// ===========================================================================

namespace Syntropy::Records
{
    /************************************************************************/
    /* RANK TRAIT                                                           */
    /************************************************************************/

    /// \brief Int constant equal to the rank of a record.
    template <typename TRecord>
    struct RankTrait
    {
        // : Syntropy::Templates::IntConstant<rank>
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Record.
    // =======

    /// \brief Access a record element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename TRecord>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TRecord> record) noexcept
        -> decltype(Details::RouteGet<TIndex>(Forward<TRecord>(record)));

    /************************************************************************/
    /* RECORD                                                               */
    /************************************************************************/

    /// \brief Concept for a fixed-size record, for which indexed
    ///        compile-time access to its elements is provided.
    template <typename TRecord>
    concept Record = requires
    {
        /// \brief Rank of the record.
        { RankTrait<TRecord>::kValue } -> Concepts::IsIntegral;
    };

    /************************************************************************/
    /* FORWARDING RECORD                                                    */
    /************************************************************************/

    /// \brief Concept for a Record that can be perfectly-forwarded.
    template <typename TRecord>
    concept ForwardingRecord
        = Record<Templates::UnqualifiedOf<TRecord>>;

    /************************************************************************/
    /* RECORD                                                               */
    /************************************************************************/

    /// \brief Rank of a record.
    template <Record TRecord>
    inline constexpr Int RankOf =
        RankTrait<Templates::UnqualifiedOf<TRecord>>::kValue;

    /// \brief Constant equal to true if two records have the same rank,
    ///        equal to false otherwise.
    template <Record TRecord, Record URecord>
    inline constexpr Bool SameRank =
        (RankOf<TRecord> == RankOf<URecord>);

    /// \brief Type of a record element.
    template <Int TIndex, Record TRecord>
    using ElementTypeOf = Templates::UnqualifiedOf<decltype(
        Records::Get<TIndex>(Templates::Declval<TRecord>()))>;

    /// \brief Index of the first element with type TElement in a record.
    template <typename TElement, Record TRecord>
    inline constexpr Int ElementIndexOf = 0;

    /// \brief Generates a sequence that can be used to enumerate all
    ///        elements in a record.
    template <Record TRecord>
    using EnumerationSequenceOf =
        Syntropy::Templates::MakeSequence<RankOf<TRecord>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Element access.
    // ===============

    /// \brief Access the first element of type TElement in a record.
    ///
    /// \remarks Ill-formed if no such element exist.
    template <typename TElement, ForwardingRecord TRecord>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TRecord> record) noexcept
        -> decltype(Get<ElementIndexOf<
            TElement,
            Templates::QualifiedOf<TRecord>>>(
                Forward<TRecord>(record)));

    // Functional.
    // ===========

    /// \brief Invoke a function with arguments provided in form of record.
    template <typename TFunction, ForwardingRecord TRecord>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TRecord> record) noexcept;

    /// \brief Apply a function to each record elements individually.
    template <typename TFunction, ForwardingRecord TRecord>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TRecord> record) noexcept;

    /// \brief Apply a function to the index-th element of each record at once.
    template <Int TIndex,
              typename TFunction,
              ForwardingRecord... TRecords>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TRecords>... records) noexcept;

    /// \brief Apply a function to each argument list generated by projecting
    ///        the i-th records elements, for each index up to the minimum rank
    ///        among those records.
    template <typename TFunction, ForwardingRecord... TRecords>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TRecords>... records) noexcept;

    /// \brief Create a new instance of TType using TRecord as constructor
    ///        arguments.
    template <typename TType, ForwardingRecord TRecord>
    [[nodiscard]] constexpr TType
    MakeFromRecord(Forwarding<TRecord> record) noexcept;

    // Swap.
    // =====

    /// \brief Member-wise swap two records and returns the value of the
    ///        former.
    template <Record TRecord, ForwardingRecord URecord>
    requires SameRank<TRecord,
                                 Templates::UnqualifiedOf<URecord>>
    [[nodiscard]] constexpr TRecord
    Exchange(Mutable<TRecord> lhs, Forwarding<URecord> rhs) noexcept;

    // Comparison.
    // ===========

    /// \brief Check whether two records are member-wise equal.
    /// \remarks Equality implies equivalence.
    template <Record TRecord, Record URecord>
    requires SameRank<TRecord, URecord>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

    /// \brief Check whether two record are member-wise equivalent.
    /// \brief Equivalence doesn't imply equality.
    template <Record TRecord, Record URecord>
    requires SameRank<TRecord, URecord>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

    /// \brief Member-wise compare two records.
    template <Record TRecord, Record URecord>
    requires SameRank<TRecord, URecord>    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Rank of a record.
    template <Syntropy::Records::Record TRecord>
    struct std::tuple_size<TRecord>;

    /// \brief Type of a record element, by index.
    template <std::size_t TIndex,
              Syntropy::Records::Record TRecord>
    struct std::tuple_element<TIndex, TRecord>;

    /// \brief Access a record element, by index.
    template <std::size_t TIndex,
              Syntropy::Records::Record TRecord>
    decltype(auto) get(Syntropy::Immutable<TRecord> record);

    /// \brief Access a record element, by index..
    template <std::size_t TIndex,
              Syntropy::Records::Record TRecord>
    decltype(auto) get(Syntropy::Movable<TRecord> record);
}

// ===========================================================================

#include "details/record.inl"

// ===========================================================================
