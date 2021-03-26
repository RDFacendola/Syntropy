
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
    struct RankTrait
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

    /// \brief Access a record element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename TRecord>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TRecord> record) noexcept
        -> decltype(Details::RouteGet<TIndex>(Forward<TRecord>(record)));

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
        /// \brief Rank of the record.
        { Records::Templates::RankTrait<TRecord>::kValue }
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
    /* RECORD                                                               */
    /************************************************************************/

    /// \brief Rank of a record.
    template <Concepts::Record TRecord>
    inline constexpr Int RankOf =
        RankTrait<TRecord>::kValue;

    /// \brief Constant equal to true if two records have the same rank,
    ///        equal to false otherwise.
    template <Concepts::Record TRecord, Concepts::Record URecord>
    inline constexpr Bool SameRank =
        (RankOf<TRecord> == RankOf<URecord>);

    /// \brief Type of a record element.
    template <Int TIndex, Concepts::Record TRecord>
    using ElementTypeOf = Syntropy::Templates::UnqualifiedOf<decltype(
        Records::Get<TIndex>(Syntropy::Templates::Declval<TRecord>()))>;

    /// \brief Generates a sequence that can be used to enumerate all
    ///        elements in a record.
    template <Concepts::Record TRecord>
    using EnumerationSequenceOf =
        Syntropy::Templates::MakeSequence<RankOf<TRecord>>;

}

// ===========================================================================

namespace Syntropy::Records
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Functional.
    // ===========

    /// \brief Invoke a function with arguments provided in form of record.
    template <typename TFunction, Concepts::ForwardingRecord TRecord>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TRecord> record) noexcept;

    /// \brief Apply a function to each record elements individually.
    template <typename TFunction, Concepts::ForwardingRecord TRecord>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TRecord> record) noexcept;

    /// \brief Apply a function to the index-th element of each record at once.
    template <Int TIndex,
              typename TFunction,
              Concepts::ForwardingRecord... TRecords>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TRecords>... records) noexcept;

    /// \brief Apply a function to each argument list generated by projecting
    ///        the i-th records elements, for each index up to the minimum rank
    ///        among those records.
    template <typename TFunction, Concepts::ForwardingRecord... TRecords>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TRecords>... records) noexcept;

    /// \brief Create a new instance of TType using TRecord as constructor
    ///        arguments.
    template <typename TType, Concepts::ForwardingRecord TRecord>
    [[nodiscard]] constexpr TType
    MakeFromRecord(Forwarding<TRecord> record) noexcept;
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

    /// \brief Swap two records and return the old value of the first.
    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::SameRank<TRecord, URecord>
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

    /// \brief Check whether two records are equal.
    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::SameRank<TRecord, URecord>
    struct AreEqual<TRecord, URecord>
    {
        [[nodiscard]] constexpr Bool
        operator()(Immutable<TRecord> lhs, Immutable<URecord> rhs)
        const noexcept;
    };

    /// \brief Check whether two records are equivalent.
    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::SameRank<TRecord, URecord>
    struct AreEquivalent<TRecord, URecord>
    {
        [[nodiscard]] constexpr Bool
        operator()(Immutable<TRecord> lhs, Immutable<URecord> rhs)
        const noexcept;
    };

    /// \brief Compare two records.
    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::SameRank<TRecord, URecord>
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

    /// \brief Rank of a record.
    template <Syntropy::Records::Concepts::Record TRecord>
    struct std::tuple_size<TRecord>;

    /// \brief Type of a record element, by index.
    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    struct std::tuple_element<TIndex, TRecord>;

    /// \brief Access a record element, by index.
    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    decltype(auto) get(Syntropy::Immutable<TRecord> record);

    /// \brief Access a record element, by index..
    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    decltype(auto) get(Syntropy::Movable<TRecord> record);
}

// ===========================================================================

#include "details/record.inl"

// ===========================================================================
