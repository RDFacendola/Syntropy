
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

namespace Syntropy::Records
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename... TTypes>
    struct Tuple;

    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<TElements...>
    MakeTuple(Forwarding<TElements>... elements) noexcept;

    template <ForwardingRecord... TRecords>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TRecords>... tuples) noexcept;

    template <ForwardingRecord TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept;
}

// ===========================================================================

namespace Syntropy::Records::Details
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
        = !Concepts::IsImplicitlyDefaultConstructible<
            Templates::TypeList<TTypes...>>;

    // (2)

    /// \brief False if all types in TTypeList are copy-constructible,
    ///        true otherwise.
    template <typename... TTypes>
    inline constexpr Bool
    ExplicitIfTupleDirectConstructor = !(Concepts::IsConvertible<
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
        = !(Concepts::IsConvertible<Movable<UTypes>, TTypes> && ...);

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
        = !(Concepts::IsConvertible<Immutable<UTypes>,
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
        = !(Concepts::IsConvertible<Movable<UTypes>, TTypes> && ...);

    /************************************************************************/
    /* ENABLE IF - TUPLE CONSTRUCTOR                                        */
    /************************************************************************/

    // (1)

    /// \brief Enable (implicit) default tuple constructor if all types TTypes
    ///        are default constructible and all types have implicit
    ///        default constructor.
    template <typename TTypeList>
    using EnableIfTupleDefaultConstructor
        = RWPtr<Templates::EnableIf<
            Concepts::IsDefaultConstructible<TTypeList>>>;

    // (2)

    /// \brief Enable direct tuple constructor if all types TTypes are
    ///        implicitly copy-constructible.
    template <typename TTypeList>
    using EnableIfTupleDirectConstructor
        = RWPtr<Templates::EnableIf<
            Concepts::IsCopyConstructible<TTypeList>>>;

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
        = Concepts::IsConstructibleFrom<
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
            = Concepts::IsConstructibleFrom<
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
             =  Concepts::IsConstructibleFrom<TType,
                                                     Immutable<UType>>
            && !Concepts::IsConvertible<Immutable<Tuple<UType>>,
                                                  TType>
            && !Concepts::IsConstructibleFrom<TType,
                                                      Immutable<Tuple<UType>>>
            && !Concepts::IsSame<TType, UType>;

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
            = Concepts::IsConstructibleFrom<
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
             = Concepts::IsConstructibleFrom<TType, Movable<UType>>
            && !Concepts::IsConvertible<Tuple<UType>, TType>
            && !Concepts::IsConvertible<TType, Tuple<UType>>
            && !Concepts::IsSame<TType, UType>;

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
            Concepts::IsCopyAssignable<TTypeList>>>;

    // (2)

    /// \brief Enable tuple copy assignment operator if all types in TTypeList
    ///        are move-assignable.
    template <typename TTypeList>
    using EnableIfTupleMoveAssignment
        = RWPtr<Templates::EnableIf<
            Concepts::IsMoveAssignable<TTypeList>>>;

    // (3)

    /// \brief Enable tuple converting copy assignment operator if all types
    ///        in TTypeList can be converting-copy-assigned from the
    ///        corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingCopyAssignment
        = RWPtr<Templates::EnableIf<
             ! Concepts::IsSame<TTypeList, UTypeList>
            && Concepts::IsAssignableFrom<
                   TTypeList,
                   Templates::ImmutableOf<UTypeList>>>>;

    // (4)

    /// \brief Enable tuple converting move assignment operator if all types
    ///        in TTypeList can be converting-move-assigned from the
    ///        corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingMoveAssignment
        = RWPtr<Templates::EnableIf<
             ! Concepts::IsSame<TTypeList, UTypeList>
            && Concepts::IsAssignableFrom<
                TTypeList,
                UTypeList>>>;

    /************************************************************************/
    /* TUPLE BASE                                                           */
    /************************************************************************/

    /// \brief Access a tuple base type by index.
    template <Int VCount, ForwardingRecord TTuple>
    struct TupleBaseHelper;

    /// \brief Specialization for tuples.
    template <Int VCount, typename TElement, typename... TElements>
    struct TupleBaseHelper<VCount, Tuple<TElement, TElements...>>
        : TupleBaseHelper<VCount - 1, Tuple<TElements...>> {};

    /// \brief End of recursion.
    template <ForwardingRecord TTuple>
    struct TupleBaseHelper<0, TTuple>
    {
        using Type = TTuple;
    };

    /// \brief Access a tuple base type by index.
    template <Int VCount, ForwardingRecord TTuple>
    using TupleBase = typename TupleBaseHelper<VCount, TTuple>::Type;

    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // Concatenate.
    // =========

    // EnumerateTupleIndexes.

    /// \brief Generate a sequence that can be used to access tuples.
    template <Int VIndex,
              ForwardingRecord TTuple,
              ForwardingRecord... TRecords>
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
    template <Int VIndex, ForwardingRecord TTuple>
    struct EnumerateTupleIndexesHelper<VIndex, TTuple>
    {
        using Type = Templates::SequenceRepeat<
            VIndex,
            RankOf<TTuple>>;
    };

    /// \brief Generate a sequence that can be used to access tuples.
    template <ForwardingRecord... TRecords>
    using EnumerateTupleIndexes
        = typename EnumerateTupleIndexesHelper<0, TRecords...>::Type;

    // EnumerateTupleElementIndexes.

    /// \brief Generate a sequence that can be used to access tuple elements.
    template <ForwardingRecord TTuple,
              ForwardingRecord... TRecords>
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
    template <ForwardingRecord TTuple>
    struct EnumerateTupleElementIndexesHelper<TTuple>
    {
        using Type = EnumerationSequenceOf<TTuple>;
    };

    /// \brief Generate a sequence that can be used to access tuple elements.
    template <ForwardingRecord... TRecords>
    using EnumerateTupleElementIndexes
        = typename EnumerateTupleElementIndexesHelper<TRecords...>::Type;

    /// \brief Concatenate a set of tuples.
    template <ForwardingRecord... TRecords>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TRecords>... tuples) noexcept;

    // Flatten.
    // ========

    /// \brief Flatten a tuple recursively.
    template <ForwardingRecord TTuple>
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
