
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

namespace Syntropy::Sequences
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename... TTypes>
    struct Tuple;

    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<TElements...>
    MakeTuple(Forwarding<TElements>... elements) noexcept;

    template <Concepts::ForwardingSequence... TSequences>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TSequences>... tuples) noexcept;

    template <Concepts::ForwardingSequence TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept;
}

// ===========================================================================

namespace Syntropy::Sequences::Details
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
        = !Syntropy::Concepts::IsImplicitlyDefaultConstructible<
            Syntropy::Templates::TypeList<TTypes...>>;

    // (2)

    /// \brief False if all types in TTypeList are copy-constructible,
    ///        true otherwise.
    template <typename... TTypes>
    inline constexpr Bool
    ExplicitIfTupleDirectConstructor = !(Syntropy::Concepts::IsConvertible<
        Immutable<TTypes>, TTypes> && ...);

    // (3)


    /// \brief False if each type in TTypeList can be constructed via its
    ///        respective type UType, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingConstructor
        = Syntropy::Templates::IllFormed<Bool, TTypeList, UTypes...>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingConstructor
    <Syntropy::Templates::TypeList<TTypes...>, UTypes...>
        = !(Syntropy::Concepts::IsConvertible<Movable<UTypes>, TTypes> && ...);

    // (4)

    /// \brief False if each type in TTypeList can be constructed via its
    ///        respective const-reference-qualified type UType, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingCopyConstructor
        = Syntropy::Templates::IllFormed<Bool, TTypeList, UTypes...>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingCopyConstructor
    <Syntropy::Templates::TypeList<TTypes...>, UTypes...>
        = !(Syntropy::Concepts::IsConvertible<Immutable<UTypes>,
                                              TTypes> && ...);

    // (5)

    /// \brief False if each type in TTypeList can be constructed via its
    ///        respective type UType, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingMoveConstructor
        = Syntropy::Templates::IllFormed<Bool, TTypeList, UTypes...>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool
    ExplicitIfTupleConvertingMoveConstructor
    <Syntropy::Templates::TypeList<TTypes...>, UTypes...>
        = !(Syntropy::Concepts::IsConvertible<Movable<UTypes>, TTypes> && ...);

    /************************************************************************/
    /* ENABLE IF - TUPLE CONSTRUCTOR                                        */
    /************************************************************************/

    // (1)

    /// \brief Enable (implicit) default tuple constructor if all types TTypes
    ///        are default constructible and all types have implicit
    ///        default constructor.
    template <typename TTypeList>
    using EnableIfTupleDefaultConstructor
        = RWPtr<Syntropy::Templates::EnableIf<
            Syntropy::Concepts::IsDefaultConstructible<TTypeList>>>;

    // (2)

    /// \brief Enable direct tuple constructor if all types TTypes are
    ///        implicitly copy-constructible.
    template <typename TTypeList>
    using EnableIfTupleDirectConstructor
        = RWPtr<Syntropy::Templates::EnableIf<
            Syntropy::Concepts::IsCopyConstructible<TTypeList>>>;

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
        = Syntropy::Concepts::IsConstructibleFrom<
            TTypeList,
            Syntropy::Templates::MovableOf<UTypeLists>...>;

    /// \brief Enable converting tuple constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingConstructor
        = RWPtr<Syntropy::Templates::EnableIf<
            EnableIfTupleConvertingConstructorHelper<
                Syntropy::Templates::CountOf<TTypeList>
                    == sizeof...(UTypes),
                TTypeList,
                Syntropy::Templates::TypeList<UTypes>...>>>;

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
            = Syntropy::Concepts::IsConstructibleFrom<
                TTypeList,
                Syntropy::Templates::ImmutableOf<UTypeLists>...>;

    /// \brief Specialization for 1-tuples. True if TType can be constructed
    ///        from UType and the overload doesn't reduce to a
    ///        copy-constructor.
    template <typename TType, typename UType>
    inline constexpr Bool
    EnableIfTupleConvertingCopyConstructorHelper<
        true,
        Syntropy::Templates::TypeList<TType>,
        Syntropy::Templates::TypeList<UType>>
             =  Syntropy::Concepts::IsConstructibleFrom<TType,
                                                     Immutable<UType>>
            && !Syntropy::Concepts::IsConvertible<Immutable<Tuple<UType>>,
                                                  TType>
            && !Syntropy::Concepts::IsConstructibleFrom<TType,
                                                      Immutable<Tuple<UType>>>
            && !Syntropy::Concepts::IsSame<TType, UType>;

    /// \brief Enable converting tuple copy-constructor if both TTypeList
    ///        and UTypes have the same rank, all types in TTypeList can be
    ///        member-wise converting-copy-constructed from the respective
    ///        UType and the overload does not reduce to a copy-constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingCopyConstructor
        = RWPtr<Syntropy::Templates::EnableIf<
            EnableIfTupleConvertingCopyConstructorHelper<
                Syntropy::Templates::CountOf<TTypeList>
                    == sizeof...(UTypes),
                TTypeList,
                Syntropy::Templates::TypeList<UTypes>...>>>;

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
            = Syntropy::Concepts::IsConstructibleFrom<
                TTypeList,
                Syntropy::Templates::MovableOf<UTypeLists>...>;

    /// \brief Specialization for 1-tuples. True if TType can be constructed
    ///        from UType and the overload doesn't reduce to a
    ///        move-constructor.
    template <typename TType, typename UType>
    inline constexpr Bool
    EnableIfTupleConvertingMoveConstructorHelper<
        true,
        Syntropy::Templates::TypeList<TType>,
        Syntropy::Templates::TypeList<UType>>
             = Syntropy::Concepts::IsConstructibleFrom<TType, Movable<UType>>
            && !Syntropy::Concepts::IsConvertible<Tuple<UType>, TType>
            && !Syntropy::Concepts::IsConvertible<TType, Tuple<UType>>
            && !Syntropy::Concepts::IsSame<TType, UType>;

    /// \brief Enable converting tuple move-constructor if both TTypeList and
    ///        UTypes have the same rank, all types in TTypeList can be
    ///        member-wise converting-move-constructed from the respective
    ///        UType and the overload does not reduce to a move-constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingMoveConstructor
        = RWPtr<Syntropy::Templates::EnableIf<
            EnableIfTupleConvertingMoveConstructorHelper<
                Syntropy::Templates::CountOf<TTypeList>
                    == sizeof...(UTypes),
                TTypeList,
                Syntropy::Templates::TypeList<UTypes>...>>>;

    /************************************************************************/
    /* ENABLE IF - TUPLE ASSIGNMENT                                         */
    /************************************************************************/

    // (1)

    /// \brief Enable tuple copy assignment operator if all types in
    ///        TTypeList are copy-assignable.
    template <typename TTypeList>
    using EnableIfTupleCopyAssignment
        = RWPtr<Syntropy::Templates::EnableIf<
            Syntropy::Concepts::IsCopyAssignable<TTypeList>>>;

    // (2)

    /// \brief Enable tuple copy assignment operator if all types in TTypeList
    ///        are move-assignable.
    template <typename TTypeList>
    using EnableIfTupleMoveAssignment
        = RWPtr<Syntropy::Templates::EnableIf<
            Syntropy::Concepts::IsMoveAssignable<TTypeList>>>;

    // (3)

    /// \brief Enable tuple converting copy assignment operator if all types
    ///        in TTypeList can be converting-copy-assigned from the
    ///        corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingCopyAssignment
        = RWPtr<Syntropy::Templates::EnableIf<
             ! Syntropy::Concepts::IsSame<TTypeList, UTypeList>
            && Syntropy::Concepts::IsAssignableFrom<
                   TTypeList,
                   Syntropy::Templates::ImmutableOf<UTypeList>>>>;

    // (4)

    /// \brief Enable tuple converting move assignment operator if all types
    ///        in TTypeList can be converting-move-assigned from the
    ///        corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingMoveAssignment
        = RWPtr<Syntropy::Templates::EnableIf<
             ! Syntropy::Concepts::IsSame<TTypeList, UTypeList>
            && Syntropy::Concepts::IsAssignableFrom<
                TTypeList,
                UTypeList>>>;

    /************************************************************************/
    /* TUPLE BASE                                                           */
    /************************************************************************/

    /// \brief Access a tuple base type by index.
    template <Int VCount, Concepts::ForwardingSequence TTuple>
    struct TupleBaseHelper;

    /// \brief Specialization for tuples.
    template <Int VCount, typename TElement, typename... TElements>
    struct TupleBaseHelper<VCount, Tuple<TElement, TElements...>>
        : TupleBaseHelper<VCount - 1, Tuple<TElements...>> {};

    /// \brief End of recursion.
    template <Concepts::ForwardingSequence TTuple>
    struct TupleBaseHelper<0, TTuple>
    {
        using Type = TTuple;
    };

    /// \brief Access a tuple base type by index.
    template <Int VCount, Concepts::ForwardingSequence TTuple>
    using TupleBase = typename TupleBaseHelper<VCount, TTuple>::Type;

    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // Concatenate.
    // =========

    // EnumerateTupleIndexes.

    /// \brief Generate a sequence that can be used to access tuples.
    template <Int VIndex,
              Concepts::ForwardingSequence TTuple,
              Concepts::ForwardingSequence... TSequences>
    struct EnumerateTupleIndexesHelper
    {
        using TupleSequence
            = typename EnumerateTupleIndexesHelper<VIndex, TTuple>::Type;

        using SequencesSequence
            = typename EnumerateTupleIndexesHelper<VIndex + 1,
                                                   TSequences...>::Type;

        using Type
            = Syntropy::Templates::SequenceCat<TupleSequence, SequencesSequence>;
    };

    /// \brief Generate a sequence of VIndex repeated a number of times equal
    ///        to the rank of TTuple.
    template <Int VIndex, Concepts::ForwardingSequence TTuple>
    struct EnumerateTupleIndexesHelper<VIndex, TTuple>
    {
        using Type = Syntropy::Templates::SequenceRepeat<
            VIndex,
            Templates::SequenceRankOf<TTuple>>;
    };

    /// \brief Generate a sequence that can be used to access tuples.
    template <Concepts::ForwardingSequence... TSequences>
    using EnumerateTupleIndexes
        = typename EnumerateTupleIndexesHelper<0, TSequences...>::Type;

    // EnumerateTupleElementIndexes.

    /// \brief Generate a sequence that can be used to access tuple elements.
    template <Concepts::ForwardingSequence TTuple,
              Concepts::ForwardingSequence... TSequences>
    struct EnumerateTupleElementIndexesHelper
    {
        using TupleSequence
            = typename EnumerateTupleElementIndexesHelper<TTuple>::Type;

        using SequencesSequence
            = typename EnumerateTupleElementIndexesHelper<TSequences...>::Type;

        using Type
            = Syntropy::Templates::SequenceCat<TupleSequence, SequencesSequence>;
    };

    /// \brief Generate an increasing sequence from 0 to the rank of
    ///        TTuple (excluded).
    template <Concepts::ForwardingSequence TTuple>
    struct EnumerateTupleElementIndexesHelper<TTuple>
    {
        using Type = Templates::SequenceEnumerationOf<TTuple>;
    };

    /// \brief Generate a sequence that can be used to access tuple elements.
    template <Concepts::ForwardingSequence... TSequences>
    using EnumerateTupleElementIndexes
        = typename EnumerateTupleElementIndexesHelper<TSequences...>::Type;

    /// \brief Concatenate a set of tuples.
    template <Concepts::ForwardingSequence... TSequences>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TSequences>... tuples) noexcept;

    // Flatten.
    // ========

    /// \brief Flatten a tuple recursively.
    template <Concepts::ForwardingSequence TTuple>
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
