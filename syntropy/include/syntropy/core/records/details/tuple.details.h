
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

namespace Syntropy::Details
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    /// \brief Alias for a type-list.
    template <typename... TElements>
    using TupleList = Templates::TypeList<TElements...>;

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

    /// \brief Walk tuple class hierarchy upwards until TCount drops to 0 or
    ///        the tuple is exhausted.
    template <Int TCount, typename TTuple>
    struct TupleBaseHelper {};

    /// \brief Specialization for tuples.
    template <template <typename...> typename TTuple,
              Int TCount,
              typename TElement,
              typename... TElements>
    struct TupleBaseHelper<TCount, TTuple<TElement, TElements...>>
        : TupleBaseHelper<TCount - 1, TTuple<TElements...>> {};

    /// \brief End of recursion.
    template <template <typename...> typename TTuple, typename... TElements>
    struct TupleBaseHelper<0, TTuple<TElements...>>
        : Templates::Alias<TTuple<TElements...>> {};

    /// \brief Access a tuple base type by index.
    template <Int TCount, typename TTuple>
    using TupleBase
        = typename TupleBaseHelper<TCount, TTuple>::Type;

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
        = typename EnumerateTupleIndexesHelper<0, Templates::UnqualifiedOf<TRecords>...>::Type;




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
        = typename EnumerateTupleElementIndexesHelper<Templates::UnqualifiedOf<TRecords>...>::Type;

}

// ===========================================================================
