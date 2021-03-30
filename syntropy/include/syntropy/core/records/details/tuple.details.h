
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

    template <Records::RecordReference... TRecords>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TRecords>... tuples) noexcept;

    template <Records::RecordReference TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept;
}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* EXPLICIT IF - TUPLE CONSTRUCTOR                                      */
    /************************************************************************/

    // (1)

    /// \brief False if all types in TTypeList are copy-list-initializable
    ///        from {}, true otherwise.
    template <typename... TTypes>
    inline constexpr Bool
    ExplicitIfTupleDefaultConstructor
        = !Templates::IsImplicitlyDefaultConstructible<
            Templates::TypeList<TTypes...>>;

    // (2)

    /// \brief False if all types in TTypeList are copy-constructible,
    ///        true otherwise.
    template <typename... TTypes>
    inline constexpr Bool
    ExplicitIfTupleDirectConstructor = !(Templates::IsConvertible<
        Immutable<TTypes>, TTypes> && ...);

    // (3)


    /// \brief False if each type in TTypeList can be constructed via its
    ///        respective type UType, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingConstructor
        = Templates::IllFormed<Bool, TTypeList, UTypes...>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingConstructor
    <Templates::TypeList<TTypes...>, UTypes...>
        = !(Templates::IsConvertible<Movable<UTypes>, TTypes> && ...);

    // (4)

    /// \brief False if each type in TTypeList can be constructed via its
    ///        respective const-reference-qualified type UType, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingCopyConstructor
        = Templates::IllFormed<Bool, TTypeList, UTypes...>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingCopyConstructor
    <Templates::TypeList<TTypes...>, UTypes...>
        = !(Templates::IsConvertible<Immutable<UTypes>,
                                              TTypes> && ...);

    // (5)

    /// \brief False if each type in TTypeList can be constructed via its
    ///        respective type UType, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingMoveConstructor
        = Templates::IllFormed<Bool, TTypeList, UTypes...>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingMoveConstructor
    <Templates::TypeList<TTypes...>, UTypes...>
        = !(Templates::IsConvertible<Movable<UTypes>, TTypes> && ...);

    /************************************************************************/
    /* TUPLE REQUIREMENTS                                                   */
    /************************************************************************/

    // (1)

    /// \brief Requires all tuple elements to be default constructible.
    template <typename... TElements>
    concept TupleDefaultConstructor
        = (Templates::IsDefaultConstructible<TElements> && ...);

    // (2)

    /// \brief Requires all tuple elements to be copy-constructible.
    template <typename... TElements>
    concept TupleDirectConstructor
        = (Templates::IsCopyConstructible<TElements> && ...);

    // (3)

    /// \brief Constant equal to true if TTypeList and UTypeLists have the same
    ///        rank and all types in TTypeList can be member-wise constructed
    ///        from the respective types in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool
    EnableIfTupleConvertingConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool
    EnableIfTupleConvertingConstructorHelper<true, TTypeList, UTypeLists...>
        = Templates::IsConstructibleFrom<
            TTypeList,
            Templates::MovableOf<UTypeLists>...>;

    /// \brief Enable converting tuple constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingConstructor
        = RWPtr<Templates::EnableIf<
            EnableIfTupleConvertingConstructorHelper<
                Templates::CountOf<TTypeList>
                    == sizeof...(UTypes),
                TTypeList,
                Templates::TypeList<UTypes>...>>>;

    // (4)

    /// \brief True if all types in TTypeList can be member-wise
    ///        converting-copy-constructed from the respective types
    ///        in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool
    EnableIfTupleConvertingCopyConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool
    EnableIfTupleConvertingCopyConstructorHelper
        <true, TTypeList, UTypeLists...>
            = Templates::IsConstructibleFrom<
                TTypeList,
                Templates::ImmutableOf<UTypeLists>...>;

    /// \brief Specialization for 1-tuples. True if TType can be constructed
    ///        from UType and the overload doesn't reduce to a
    ///        copy-constructor.
    template <typename TType, typename UType>
    inline constexpr Bool
    EnableIfTupleConvertingCopyConstructorHelper<
        true,
        Templates::TypeList<TType>,
        Templates::TypeList<UType>>
             =  Templates::IsConstructibleFrom<TType,
                                                     Immutable<UType>>
            && !Templates::IsConvertible<Immutable<Tuple<UType>>,
                                                  TType>
            && !Templates::IsConstructibleFrom<TType,
                                                      Immutable<Tuple<UType>>>
            && !Templates::IsSame<TType, UType>;

    /// \brief Enable converting tuple copy-constructor if both TTypeList
    ///        and UTypes have the same rank, all types in TTypeList can be
    ///        member-wise converting-copy-constructed from the respective
    ///        UType and the overload does not reduce to a copy-constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingCopyConstructor
        = RWPtr<Templates::EnableIf<
            EnableIfTupleConvertingCopyConstructorHelper<
                Templates::CountOf<TTypeList>
                    == sizeof...(UTypes),
                TTypeList,
                Templates::TypeList<UTypes>...>>>;

    // (5)

    /// \brief Enable converting tuple converting-move-constructor if all types
    ///        in TTypeList can be member-wise converting-move-constructed from
    ///        the respective types in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool
    EnableIfTupleConvertingMoveConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool
    EnableIfTupleConvertingMoveConstructorHelper
        <true, TTypeList, UTypeLists...>
            = Templates::IsConstructibleFrom<
                TTypeList,
                Templates::MovableOf<UTypeLists>...>;

    /// \brief Specialization for 1-tuples. True if TType can be constructed
    ///        from UType and the overload doesn't reduce to a
    ///        move-constructor.
    template <typename TType, typename UType>
    inline constexpr Bool
    EnableIfTupleConvertingMoveConstructorHelper<
        true,
        Templates::TypeList<TType>,
        Templates::TypeList<UType>>
             = Templates::IsConstructibleFrom<TType, Movable<UType>>
            && !Templates::IsConvertible<Tuple<UType>, TType>
            && !Templates::IsConvertible<TType, Tuple<UType>>
            && !Templates::IsSame<TType, UType>;

    /// \brief Enable converting tuple move-constructor if both TTypeList and
    ///        UTypes have the same rank, all types in TTypeList can be
    ///        member-wise converting-move-constructed from the respective
    ///        UType and the overload does not reduce to a move-constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingMoveConstructor
        = RWPtr<Templates::EnableIf<
            EnableIfTupleConvertingMoveConstructorHelper<
                Templates::CountOf<TTypeList>
                    == sizeof...(UTypes),
                TTypeList,
                Templates::TypeList<UTypes>...>>>;

    /************************************************************************/
    /* ENABLE IF - TUPLE ASSIGNMENT                                         */
    /************************************************************************/

    // (1)

    /// \brief Enable tuple copy assignment operator if all types in
    ///        TTypeList are copy-assignable.
    template <typename TTypeList>
    using EnableIfTupleCopyAssignment
        = RWPtr<Templates::EnableIf<
            Templates::IsCopyAssignable<TTypeList>>>;

    // (2)

    /// \brief Enable tuple copy assignment operator if all types in TTypeList
    ///        are move-assignable.
    template <typename TTypeList>
    using EnableIfTupleMoveAssignment
        = RWPtr<Templates::EnableIf<
            Templates::IsMoveAssignable<TTypeList>>>;

    // (3)

    /// \brief Enable tuple converting copy assignment operator if all types
    ///        in TTypeList can be converting-copy-assigned from the
    ///        corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingCopyAssignment
        = RWPtr<Templates::EnableIf<
             ! Templates::IsSame<TTypeList, UTypeList>
            && Templates::IsAssignableFrom<
                   TTypeList,
                   Templates::ImmutableOf<UTypeList>>>>;

    // (4)

    /// \brief Enable tuple converting move assignment operator if all types
    ///        in TTypeList can be converting-move-assigned from the
    ///        corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingMoveAssignment
        = RWPtr<Templates::EnableIf<
             ! Templates::IsSame<TTypeList, UTypeList>
            && Templates::IsAssignableFrom<
                TTypeList,
                UTypeList>>>;

    /************************************************************************/
    /* TUPLE BASE                                                           */
    /************************************************************************/

    /// \brief Access a tuple base type by index.
    template <Int VCount, Records::RecordReference TTuple>
    struct TupleBaseHelper;

    /// \brief Specialization for tuples.
    template <Int VCount, typename TElement, typename... TElements>
    struct TupleBaseHelper<VCount, Tuple<TElement, TElements...>>
        : TupleBaseHelper<VCount - 1, Tuple<TElements...>> {};

    /// \brief End of recursion.
    template <Records::RecordReference TTuple>
    struct TupleBaseHelper<0, TTuple>
    {
        using Type = TTuple;
    };

    /// \brief Access a tuple base type by index.
    template <Int VCount, Records::RecordReference TTuple>
    using TupleBase = typename TupleBaseHelper<VCount, TTuple>::Type;

    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // Concatenate.
    // =========

    // EnumerateTupleIndexes.

    /// \brief Generate a sequence that can be used to access tuples.
    template <Int VIndex,
              Records::RecordReference TTuple,
              Records::RecordReference... TRecords>
    struct EnumerateTupleIndexesHelper
    {
        using TupleSequence
            = typename EnumerateTupleIndexesHelper<VIndex, TTuple>::Type;

        using RecordsSequence
            = typename EnumerateTupleIndexesHelper<VIndex + 1,
                                                   TRecords...>::Type;

        using Type
            = Templates::SequenceCat<TupleSequence, RecordsSequence>;
    };

    /// \brief Generate a sequence of VIndex repeated a number of times equal
    ///        to the rank of TTuple.
    template <Int VIndex, Records::RecordReference TTuple>
    struct EnumerateTupleIndexesHelper<VIndex, TTuple>
    {
        using Type = Templates::SequenceRepeat<
            VIndex,
            Records::RankOf<TTuple>>;
    };

    /// \brief Generate a sequence that can be used to access tuples.
    template <Records::RecordReference... TRecords>
    using EnumerateTupleIndexes
        = typename EnumerateTupleIndexesHelper<0, TRecords...>::Type;

    // EnumerateTupleElementIndexes.

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
