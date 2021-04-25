
/// \file tuple_details.h
///
/// \brief This header is part of Syntropy core module.
//         It contains implementation details for tuples.
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"

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
        = (Templates::IsConstructible<TElements> && ...);

    /// \brief True if at least one tuple element is default-constructible.
    template <typename... TTypes>
    inline constexpr
    Bool TupleExplicitDefaultConstructor
        = !(Templates::IsConstructible<TTypes> && ...);

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
        = (Templates::IsConstructible<
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
        = (Templates::IsConstructible<
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
        = (Templates::IsConstructible<
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
        = (Templates::IsAssignable<
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
       = (Templates::IsAssignable<
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

    // Concatenate helpers.
    // ====================

    // In order to concatenate records together, two sequences with the same
    // total size are generated.
    // The first sequence is used to associate each element in the resulting
    // sequence with the index of the record the element is taken from.
    // The second sequence is used to associate each element in the resulting
    // sequence with the index of the element in the owning record.

    // Example: A {a,b,c}, B {d,e}, C {f}
    // First sequence:  000-11-2 AAA-BB-C
    // Second sequence: 012-01-0 abc-de-f

    /// \brief Sequence associating each element to the generating tuple.
    template <typename TSequence, Records::RecordReference... TRecords>
    struct TupleEnumerateRecordsHelper {};

    /// \brief Partial template specialization for sequence-records pairs.
    template <Int... TIndex, Records::RecordReference... TRecords>
    struct TupleEnumerateRecordsHelper<Templates::Sequence<TIndex...>,
                                       TRecords...>
        : Templates::Alias<
            Templates::SequenceConcatenate<
                Templates::SequenceRepeat<TIndex,
                                          Records::RankOf<TRecords>>...>> {};

    /// \brief Sequence associating each element to the source record.
    template <Records::RecordReference... TRecords>
    using TupleEnumerateRecords
        = typename TupleEnumerateRecordsHelper<
            Templates::SequenceFor<TRecords...>, TRecords...>::Type;

    /// \brief Sequence associating each element to the source record element.
    template <Records::RecordReference... TRecords>
    using TupleEnumerateRecordElements
        = Templates::SequenceConcatenate<Records::SequenceOf<TRecords>...>;

}

// ===========================================================================
