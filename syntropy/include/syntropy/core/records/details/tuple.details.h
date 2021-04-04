
/// \file tuple_details.h
///
/// \brief This header is part of Syntropy core module.
//         It contains implementation details for tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/diagnostics.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename... TTypes>
    struct Tuple;

    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<TElements...>
    MakeTuple(Forwarding<TElements>... elements) noexcept;

}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    /// \brief Alias for a type-list.
    template <typename... TElements>
    using TupleList = Templates::TypeList<TElements...>;

    /// \brief An alias for a tuple type.
    template <typename... TElements>
    using TupleAlias = Templates::Alias<Tuple<TElements...>>;

    // Default constructor.
    // ====================

    /// \brief True if all tuple elements are default-constructible.
    template <typename... TElements>
    inline constexpr
    Bool TupleDefaultConstructor
        = Templates::IsDefaultConstructible<TElements...>;

    /// \brief True if at least one tuple element is explicit
    ///        default-constructible.
    template <typename... TTypes>
    inline constexpr
    Bool TupleExplicitDefaultConstructor
        = !Templates::IsImplicitlyDefaultConstructible<TTypes...>;

    // Direct constructor.
    // ===================

    /// \brief True if each element in TElements can be constructed from
    ///        its respective element in UElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleDirectConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleDirectConstructor<TupleList<TElements...>, UElements...>
        = (Templates::IsConstructibleFrom<
               TElements, Templates::ForwardingOf<UElements>> && ...);

    /// \brief True if at least one among UElements is not implicitly
    ///        convertible to its relative element in TElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleExplicitDirectConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleExplicitDirectConstructor<TupleList<TElements...>, UElements...>
        = !(Templates::IsConvertible<
                Templates::ForwardingOf<UElements>, TElements> && ...);

    // Copy constructor.
    // =================

    /// \brief True if each element in TElements can be
    ///        converting-copy-constructed from its respective element in
    ///        UElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleCopyConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleCopyConstructor<TupleList<TElements...>, UElements...>
        = (Templates::IsConstructibleFrom<
               TElements, Templates::ImmutableOf<UElements>> && ...);

    /// \brief True if at least one among UElements is not implicitly
    ///        copy-convertible to its relative element in TElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleExplicitCopyConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleExplicitCopyConstructor<TupleList<TElements...>, UElements...>
        = !(Templates::IsConvertible<
                Templates::ImmutableOf<UElements>, TElements> && ...);

    // Move constructor.
    // =================

    /// \brief True if each element in TElements can be
    ///        converting-move-constructed from its respective element in
    ///        UElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleMoveConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleMoveConstructor<TupleList<TElements...>, UElements...>
        = (Templates::IsConstructibleFrom<
               TElements, Templates::MovableOf<UElements>> && ...);

    /// \brief True if at least one among UElements is not implicitly
    ///        copy-convertible to its relative element in TElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleExplicitMoveConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleExplicitMoveConstructor<TupleList<TElements...>, UElements...>
        = !(Templates::IsConvertible<
                Templates::MovableOf<UElements>, TElements> && ...);

    // Copy assignment.
    // ================

    /// \brief True if each element in TElements can be
    ///        converting-copy-assigned from its respective element in
    ///        UElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleCopyAssignment
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleCopyAssignment<TupleList<TElements...>, UElements...>
        = (Templates::IsAssignableFrom<
               TElements, Templates::ImmutableOf<UElements>> && ...);

   // Move assignment.
   // ================

   /// \brief True if each element in TElements can be
   ///        converting-move-assigned from its respective element in
   ///        UElements.
   template <typename TElements, typename... UElements>
   inline constexpr
   Bool TupleMoveAssignment
       = false;

   /// \brief Specialization for type list - parameter pack pairs.
   template <typename... TElements, typename... UElements>
   requires (sizeof...(TElements) == sizeof...(UElements))
   inline constexpr
   Bool TupleMoveAssignment<TupleList<TElements...>, UElements...>
       = (Templates::IsAssignableFrom<
              TElements, Templates::MovableOf<UElements>> && ...);

    /************************************************************************/
    /* TUPLE BASE                                                           */
    /************************************************************************/

    /// \brief Access a tuple base type by index.
    template <Int TCount, typename TTuple>
    struct TupleBaseHelper {};

    /// \brief Specialization for tuples.
    template <Int TCount, typename TElement, typename... TElements>
    struct TupleBaseHelper<TCount, Tuple<TElement, TElements...>>
        : TupleBaseHelper<TCount - 1, Tuple<TElements...>> {};

    /// \brief End of recursion.
    template <typename... TElements>
    struct TupleBaseHelper<0, Tuple<TElements...>>
        : TupleAlias<TElements...> {};

    /// \brief Access a tuple base type by index.
    template <Int TCount, typename TTuple>
    using TupleBase = typename TupleBaseHelper<TCount, TTuple>::Type;

    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // Concatenate.
    // ============

    /// \brief Generate a sequence that can be used to access tuples.
    template <Int TIndex,
              Records::RecordReference TTuple,
              Records::RecordReference... TRecords>
    struct EnumerateTupleIndexesHelper
    {
        using TupleSequence
            = typename EnumerateTupleIndexesHelper<TIndex, TTuple>::Type;

        using RecordsSequence
            = typename EnumerateTupleIndexesHelper<TIndex + 1,
                                                   TRecords...>::Type;

        using Type
            = Templates::SequenceCat<TupleSequence, RecordsSequence>;
    };

    /// \brief Generate a sequence of VIndex repeated a number of times equal
    ///        to the rank of TTuple.
    template <Int TIndex, Records::RecordReference TTuple>
    struct EnumerateTupleIndexesHelper<TIndex, TTuple>
    {
        using Type = Templates::SequenceRepeat<
            TIndex,
            Records::RankOf<TTuple>>;
    };

    /// \brief Generate a sequence that can be used to access tuples.
    template <Records::RecordReference... TRecords>
    using EnumerateTupleIndexes
        = typename EnumerateTupleIndexesHelper<0, TRecords...>::Type;

    /// \brief Generate a sequence that can be used to access tuple elements.
    template <Records::RecordReference TTuple,
              Records::RecordReference... TRecords>
    struct EnumerateTupleElementIndexesHelper
    {
        using TupleSequence
            = typename EnumerateTupleElementIndexesHelper<TTuple>::Type;

        using RecordsSequence
            = typename EnumerateTupleElementIndexesHelper<TRecords...>::Type;

        using Type
            = Templates::SequenceCat<TupleSequence, RecordsSequence>;
    };

    /// \brief Generate an increasing sequence from 0 to the rank of
    ///        TTuple (excluded).
    template <Records::RecordReference TTuple>
    struct EnumerateTupleElementIndexesHelper<TTuple>
    {
        using Type = Records::SequenceOf<TTuple>;
    };

    /// \brief Generate a sequence that can be used to access tuple elements.
    template <Records::RecordReference... TRecords>
    using EnumerateTupleElementIndexes
        = typename EnumerateTupleElementIndexesHelper<TRecords...>::Type;

    /// \brief Concatenate a set of tuples.
    template <Records::RecordReference... TRecords>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TRecords>... tuples) noexcept;

    // Flatten.
    // ========

    /// \brief Flatten a tuple recursively.
    template <Records::RecordReference TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept;

    /// \brief Flatten a tuple recursively. End of recursion.
    template <typename TElement>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TElement> element) noexcept;

}

// ===========================================================================

#include "tuple.details.inl"

// ===========================================================================
