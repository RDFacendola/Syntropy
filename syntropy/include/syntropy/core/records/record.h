
/// \file record.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for record concepts and related
///        functionalities.
///
/// \author Raffaele D. Facendola - November 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/type_traits.h"

#include "syntropy/core/toolset/swap.h"

#include "syntropy/core/comparisons/compare.h"
#include "syntropy/core/comparisons/ordering.h"

#include "syntropy/math/math.h"

// ===========================================================================

#include "details/record.details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RECORDS EXTENSIONS                                                   */
    /************************************************************************/

    namespace Records::Extensions
    {
        /// \brief Access a record element by index.
        template <Int TIndex, typename TRecord, typename = void>
        struct Get;
    }

    /************************************************************************/
    /* RECORDS TYPE TRAITS                                                  */
    /************************************************************************/

    /// \brief Number of elements in a record.
    template <typename TRecord>
    struct RecordRankTrait
    {
        // : Syntropy::Templates::IntType<rank>
    };

    /// \brief Type of an element in a record, by index.
    template <Int TIndex, typename TRecord>
    struct RecordElementTypeTrait
    {
        // : Syntropy::Templates::Alias<element type>;
    };

    /************************************************************************/
    /* RECORD                                                               */
    /************************************************************************/

    /// \brief Concept for a record whose elements can be accessed at
    ///        compile-time by index.
    template <typename TRecord>
    concept Record = requires
    {
        /// \brief Rank of the record.
        { RecordRankTrait<TRecord>::kValue } -> Templates::IsIntegral;
    };

    /// \brief Concept for a reference to a record.
    template <typename TRecord>
    concept RecordReference
        = Record<Templates::UnqualifiedOf<TRecord>>;

    /// \brief Tag type associated to records.
    struct RecordTag{};

    /************************************************************************/
    /* RECORDS                                                              */
    /************************************************************************/

    namespace Records
    {
        /// \brief Number of elements in a record.
        template <RecordReference TRecord>
        inline constexpr
        Int RankOf
            = RecordRankTrait<Templates::UnqualifiedOf<TRecord>>::kValue;

        /// \brief True if two records have the same rank, false otherwise.
        template <RecordReference TRecord, RecordReference URecord>
        inline constexpr
        Bool IsSameRank
            = (RankOf<TRecord> == RankOf<URecord>);

        /// \brief Type of a record element.
        ///
        /// \remarks Ill-formed if exceeding record bounds.
        template <Int TIndex, RecordReference TRecord>
        using ElementTypeOf
            = typename RecordElementTypeTrait<
                TIndex, Templates::UnqualifiedOf<TRecord>>::Type;

        /// \brief Generates a sequence that can be used to enumerate all
        ///        elements in a record.
        template <RecordReference TRecord>
        using SequenceOf
            = Templates::MakeSequence<RankOf<TRecord>>;

        /// \brief List of types of a record's elements.
        template <RecordReference TRecord>
        using ElementTypeListOf
            = Details::ElementTypeListOf<TRecord,
                                         SequenceOf<TRecord>,
                                         RecordElementTypeTrait>;

        /// \brief Index of the first element with type TElement in a record.
        ///
        /// \remarks Ill-formed if no such element exist.
        template <typename TElement, RecordReference TRecord>
        inline constexpr
        Int ElementIndexOf
            = Templates::ElementIndexOf<TElement, ElementTypeListOf<TRecord>>;

        /// \brief Access a record element by index.
        ///
        /// \remarks Ill-formed if the index exceeds record's bounds.
        template <Int TIndex, RecordReference TRecord>
        [[nodiscard]] constexpr decltype(auto)
        Get(Forwarding<TRecord> record) noexcept;

        /// \brief Access the first element of type TElement in a record.
        ///
        /// \remarks Ill-formed if no such element exist.
        template <typename TElement, RecordReference TRecord>
        [[nodiscard]] constexpr decltype(auto)
        Get(Forwarding<TRecord> record) noexcept;

        /// \brief Invoke a function with arguments provided in form of record.
        template <typename TFunction, RecordReference TRecord>
        constexpr decltype(auto)
        Apply(Forwarding<TFunction> function, Forwarding<TRecord> record)
        noexcept;

        /// \brief Apply a function to each record elements individually.
        template <typename TFunction, RecordReference TRecord>
        constexpr void
        ForEachApply(Forwarding<TFunction> function,
                     Forwarding<TRecord> record) noexcept;

        /// \brief Apply a function to the index-th element of each record at
        ///        once.
        template <Int TIndex, typename TFunction, RecordReference... TRecords>
        constexpr decltype(auto)
        ProjectApply(Forwarding<TFunction> function,
                     Forwarding<TRecords>... records) noexcept;

        /// \brief Apply a function to each argument list generated by
        ///        projecting the i-th records elements, for each index up to
        ///        the minimum rank among those records.
        template <typename TFunction, RecordReference... TRecords>
        constexpr void
        LockstepApply(Forwarding<TFunction> function,
                      Forwarding<TRecords>... records) noexcept;

        /// \brief Create a new instance of TType using TRecord as constructor
        ///        arguments.
        template <typename TType, RecordReference TRecord>
        [[nodiscard]] constexpr TType
        MakeFromRecord(Forwarding<TRecord> record) noexcept;

        /// \brief Member-wise copy a record to another one with the same rank.
        template <Record TRecord, Record URecord>
        constexpr void
        Copy(Mutable<TRecord> destination, Immutable<URecord> source)
        noexcept;

        /// \brief Member-wise move a record to another one with the same rank.
        template <Record TRecord, RecordReference URecord>
        constexpr void
        Move(Mutable<TRecord> destination, Forwarding<URecord> source)
        noexcept;

        /// \brief Member-wise swap two records.
        template <Record TRecord, Record URecord>
        constexpr void
        Swap(Mutable<TRecord> lhs, Mutable<URecord> rhs) noexcept;

        /// \brief Member-wise swap two records and returns the previous value
        ///        of the former.
        template <Record TRecord, RecordReference URecord>
        [[nodiscard]] constexpr TRecord
        Exchange(Mutable<TRecord> lhs, Forwarding<URecord> rhs) noexcept;

        /// \brief Member-wise copy a record to another one, until either is
        ///        exhausted.
        /// \return Returns the number of copied elements.
        template <Record TRecord, Record URecord>
        constexpr Int
        PartialCopy(Mutable<TRecord> destination, Immutable<URecord> source)
        noexcept;

        /// \brief Member-wise move a record to another one, until either is
        ///        exhausted.
        /// \return Returns the number of copied elements.
        template <Record TRecord, RecordReference URecord>
        constexpr Int
        PartialMove(Mutable<TRecord> destination, Forwarding<URecord> source)
        noexcept;

        /// \brief Member-wise swap two records until either is exhausted.
        ///
        /// \return Returns the number of swapped elements.
        template <Record TRecord, Record URecord>
        constexpr Int
        PartialSwap(Mutable<TRecord> lhs, Mutable<URecord> rhs) noexcept;

        /// \brief Check whether two records are member-wise equal.
        /// \remarks Equality implies equivalence.
        template <Record TRecord, Record URecord>
        [[nodiscard]] constexpr Bool
        AreEqual(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

        /// \brief Check whether two record are member-wise equivalent.
        /// \remarks Equivalence doesn't imply equality.
        template <Record TRecord, Record URecord>
        [[nodiscard]] constexpr Bool
        AreEquivalent(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

        /// \brief Member-wise compare two records lexicographically.
        template <Record TRecord, Record URecord>
        [[nodiscard]] constexpr Ordering
        Compare(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

    }

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Rank of a record.
    template <Syntropy::Record TRecord>
    struct std::tuple_size<TRecord>;

    /// \brief Type of a record element, by index.
    template <std::size_t TIndex,
              Syntropy::Record TRecord>
    struct std::tuple_element<TIndex, TRecord>;

    /// \brief Access a record element, by index.
    template <std::size_t TIndex,
              Syntropy::Record TRecord>
    decltype(auto) get(Syntropy::Immutable<TRecord> record);

    /// \brief Access a record element, by index..
    template <std::size_t TIndex,
              Syntropy::Record TRecord>
    decltype(auto) get(Syntropy::Movable<TRecord> record);
}

// ===========================================================================

#include "details/record.inl"

// ===========================================================================
