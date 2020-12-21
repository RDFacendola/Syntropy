
/// \file tuple_details.h
/// \brief This header is part of Syntropy core module. It contains implementation details for tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename... TTypes>
    struct Tuple;

    template <typename... TElements>
    constexpr Tuple<TElements...> MakeTuple(Forwarding<TElements>... elements) noexcept;

    template <Concepts::NTupleReference... TTuples>
    constexpr decltype(auto) TupleCat(Forwarding<TTuples>... tuples) noexcept;

    template <Concepts::NTupleReference TTuple>
    constexpr decltype(auto) TupleFlat(Forwarding<TTuple> tuple) noexcept;
}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* EXPLICIT IF - TUPLE CONSTRUCTOR                                      */
    /************************************************************************/

    // (1)

    /// \brief False if all types in TTypeList are copy-list-initializable from {}, true otherwise.
    template <typename... TTypes>
    inline constexpr Bool ExplicitIfTupleDefaultConstructor = !Templates::IsImplicitlyDefaultConstructible<Templates::TypeList<TTypes...>>;

    // (2)

    /// \brief False if all types in TTypeList are copy-constructible, true otherwise.
    template <typename... TTypes>
    inline constexpr Bool ExplicitIfTupleDirectConstructor = !Templates::IsConvertible<Templates::TypeList<Immutable<TTypes>...>, Templates::TypeList<TTypes...>>;

    // (3)


    /// \brief False if each type in TTypeList can be constructed via their respective type in UTypeList, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingConstructor;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingConstructor<Templates::TypeList<TTypes...>, UTypes...> = !Templates::IsConvertible<Templates::TypeList<Movable<UTypes>...>, Templates::TypeList<TTypes...>>;

    // (4)

    /// \brief False if each type in TTypeList can be constructed via their respective const-referenced-qualified type in UTypeList, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingCopyConstructor;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingCopyConstructor<Templates::TypeList<TTypes...>, UTypes...> = !Templates::IsConvertible<Templates::TypeList<Immutable<UTypes>...>, Templates::TypeList<TTypes...>>;

    // (5)

    /// \brief False if each type in TTypeList can be constructed via their respective type in UTypeList, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingMoveConstructor;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingMoveConstructor<Templates::TypeList<TTypes...>, UTypes...> = !Templates::IsConvertible<Templates::TypeList<Movable<UTypes>...>, Templates::TypeList<TTypes...>>;

    /************************************************************************/
    /* ENABLE IF - TUPLE CONSTRUCTOR                                        */
    /************************************************************************/

    // (1)

    /// \brief Enable (implicit) default tuple constructor if all types TTypes are default constructible and all types have implicit default constructor.
    template <typename TTypeList>
    using EnableIfTupleDefaultConstructor = RWPtr<Templates::EnableIf<Templates::IsDefaultConstructible<TTypeList>>>;

    // (2)

    /// \brief Enable direct tuple constructor if all types TTypes are implicitly copy-constructible.
    template <typename TTypeList>
    using EnableIfTupleDirectConstructor = RWPtr<Templates::EnableIf<Templates::IsCopyConstructible<TTypeList>>>;

    // (3)

    /// \brief Constant equal to true if TTypeList and UTypeLists have the same rank and all types in TTypeList can be member-wise constructed from the respective types in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingConstructorHelper<true, TTypeList, UTypeLists...> = Templates::IsConstructible<TTypeList, Templates::AddRValueReference<UTypeLists>...>;

    /// \brief Enable converting tuple constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingConstructor = RWPtr<Templates::EnableIf<EnableIfTupleConvertingConstructorHelper<Templates::TypeListRank<TTypeList> == sizeof...(UTypes), TTypeList, Templates::TypeList<UTypes>...>>>;

    // (4)

    /// \brief True if all types in TTypeList can be member-wise converting-copy-constructed from the respective types in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingCopyConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingCopyConstructorHelper<true, TTypeList, UTypeLists...> = Templates::IsConstructible<TTypeList, Templates::AddLValueConstReference<UTypeLists>...>;

    /// \brief Specialization for 1-tuples. True if TType can be constructed from UType and the overload doesn't reduce to a copy-constructor.
    template <typename TType, typename UType>
    inline constexpr Bool EnableIfTupleConvertingCopyConstructorHelper<true, Templates::TypeList<TType>, Templates::TypeList<UType>> = Templates::IsConstructible<TType, Immutable<UType>>
        && !Templates::IsConvertible<Immutable<Tuple<UType>>, TType>
        && !Templates::IsConstructible<TType, Immutable<Tuple<UType>>>
        && !Templates::IsSame<TType, UType>;

    /// \brief Enable converting tuple copy-constructor if both TTypeList and UTypes have the same rank, all types in TTypeList can be member-wise converting-copy-constructed from the respective UType and the overload does not reduce to a copy-constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingCopyConstructor = RWPtr<Templates::EnableIf<EnableIfTupleConvertingCopyConstructorHelper<Templates::TypeListRank<TTypeList> == sizeof...(UTypes), TTypeList, Templates::TypeList<UTypes>...>>>;

    // (5)

    /// \brief Enable converting tuple converting-move-constructor if all types in TTypeList can be member-wise converting-move-constructed from the respective types in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingMoveConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingMoveConstructorHelper<true, TTypeList, UTypeLists...> = Templates::IsConstructible<TTypeList, Templates::AddRValueReference<UTypeLists>...>;

    /// \brief Specialization for 1-tuples. True if TType can be constructed from UType and the overload doesn't reduce to a move-constructor.
    template <typename TType, typename UType>
    inline constexpr Bool EnableIfTupleConvertingMoveConstructorHelper<true, Templates::TypeList<TType>, Templates::TypeList<UType>> = Templates::IsConstructible<TType, Movable<UType>>
        && !Templates::IsConvertible<Tuple<UType>, TType>
        && !Templates::IsConstructible<TType, Tuple<UType>>
        && !Templates::IsSame<TType, UType>;

    /// \brief Enable converting tuple move-constructor if both TTypeList and UTypes have the same rank, all types in TTypeList can be member-wise converting-move-constructed from the respective UType and the overload does not reduce to a move-constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingMoveConstructor = RWPtr<Templates::EnableIf<EnableIfTupleConvertingMoveConstructorHelper<Templates::TypeListRank<TTypeList> == sizeof...(UTypes), TTypeList, Templates::TypeList<UTypes>...>>>;

    /************************************************************************/
    /* ENABLE IF - TUPLE ASSIGNMENT                                         */
    /************************************************************************/

    // (1)

    /// \brief Enable tuple copy assignment operator if all types in TTypeList are copy-assignable.
    template <typename TTypeList>
    using EnableIfTupleCopyAssignment = RWPtr<Templates::EnableIf<Templates::IsCopyAssignable<TTypeList>>>;

    // (2)

    /// \brief Enable tuple copy assignment operator if all types in TTypeList are move-assignable.
    template <typename TTypeList>
    using EnableIfTupleMoveAssignment = RWPtr<Templates::EnableIf<Templates::IsMoveAssignable<TTypeList>>>;

    // (3)

    /// \brief Enable tuple converting copy assignment operator if all types in TTypeList can be converting-copy-assigned from the corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingCopyAssignment = RWPtr<Templates::EnableIf<!Templates::IsSame<TTypeList, UTypeList> && Templates::IsAssignable<Templates::AddLValueReference<TTypeList>, Templates::AddLValueConstReference<UTypeList>>>>;

    // (4)

    /// \brief Enable tuple converting move assignment operator if all types in TTypeList can be converting-move-assigned from the corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingMoveAssignment = RWPtr<Templates::EnableIf<!Templates::IsSame<TTypeList, UTypeList> && Templates::IsAssignable<Templates::AddLValueReference<TTypeList>, UTypeList>>>;

    /************************************************************************/
    /* TUPLE BASE                                                           */
    /************************************************************************/

    /// \brief Access a tuple base type by index.
    template <Int VCount, Concepts::NTupleReference TTuple>
    struct TupleBaseHelper;

    /// \brief Specialization for tuples.
    template <Int VCount, typename TElement, typename... TElements>
    struct TupleBaseHelper<VCount, Tuple<TElement, TElements...>> : TupleBaseHelper<VCount - 1, Tuple<TElements...>> {};

    /// \brief End of recursion.
    template <Concepts::NTupleReference TTuple>
    struct TupleBaseHelper<0, TTuple>
    {
        using Type = TTuple;
    };

    /// \brief Access a tuple base type by index.
    template <Int VCount, Concepts::NTupleReference TTuple>
    using TupleBase = typename TupleBaseHelper<VCount, TTuple>::Type;

    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // TupleCat.
    // =========

    // EnumerateTupleIndexes.

    /// \brief Generate a sequence that can be used to access tuples.
    template <Int VIndex, Concepts::NTupleReference TTuple, Concepts::NTupleReference... TTuples>
    struct EnumerateTupleIndexesHelper
    {
        using TupleSequence = typename EnumerateTupleIndexesHelper<VIndex, TTuple>::Type;
        using TuplesSequence = typename EnumerateTupleIndexesHelper<VIndex + 1, TTuples...>::Type;

        using Type = Templates::SequenceCat<TupleSequence, TuplesSequence>;
    };

    /// \brief Generate a sequence of VIndex repeated a number of times equal to the rank of TTuple.
    template <Int VIndex, Concepts::NTupleReference TTuple>
    struct EnumerateTupleIndexesHelper<VIndex, TTuple>
    {
        using Type = Templates::SequenceRepeat<VIndex, Templates::TupleRank<TTuple>>;
    };

    /// \brief Generate a sequence that can be used to access tuples.
    template <Concepts::NTupleReference... TTuples>
    using EnumerateTupleIndexes = typename EnumerateTupleIndexesHelper<0, TTuples...>::Type;

    // EnumerateTupleElementIndexes.

    /// \brief Generate a sequence that can be used to access tuple elements.
    template <Concepts::NTupleReference TTuple, Concepts::NTupleReference... TTuples>
    struct EnumerateTupleElementIndexesHelper
    {
        using TupleSequence = typename EnumerateTupleElementIndexesHelper<TTuple>::Type;
        using TuplesSequence = typename EnumerateTupleElementIndexesHelper<TTuples...>::Type;

        using Type = Templates::SequenceCat<TupleSequence, TuplesSequence>;
    };

    /// \brief Generate an increasing sequence from 0 to the rank of TTuple (excluded).
    template <Concepts::NTupleReference TTuple>
    struct EnumerateTupleElementIndexesHelper<TTuple>
    {
        using Type = Templates::TupleSequenceFor<TTuple>;
    };

    /// \brief Generate a sequence that can be used to access tuple elements.
    template <Concepts::NTupleReference TTuple, Concepts::NTupleReference... TTuples>
    using EnumerateTupleElementIndexes = typename EnumerateTupleElementIndexesHelper<TTuple, TTuples...>::Type;

    /// \brief Concatenate a set of tuples.
    template <Concepts::NTupleReference... TTuples>
    constexpr decltype(auto) TupleCat(Forwarding<TTuples>... tuples) noexcept;

    // Flat.
    // =====

    /// \brief Flatten a tuple recursively.
    template <Concepts::NTupleReference TTuple>
    constexpr decltype(auto) TupleFlat(Forwarding<TTuple> tuple) noexcept;

    /// \brief Flatten a tuple recursively. End of recursion.
    template <typename TElement>
    constexpr decltype(auto) TupleFlat(Forwarding<TElement> element) noexcept;

}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Utilities.
    // ==========

    template <Concepts::NTupleReference... TTuples>
    constexpr decltype(auto) TupleCat(Forwarding<TTuples>... tuples) noexcept
    {
        auto tuple_cat = [&]<Concepts::NTupleReference TTuple, Int... VTupleIndex, Int... VElementIndex>(Forwarding<TTuple> tuple, Templates::Sequence<VTupleIndex...>, Templates::Sequence<VElementIndex...>)
        {
            return MakeTuple((Get<VElementIndex>(Get<VTupleIndex>(Forward<TTuple>(tuple))))...);
        };

        return tuple_cat(ForwardAsTuple(tuples...), EnumerateTupleIndexes<Templates::RemoveConstReference<TTuples>...>{}, EnumerateTupleElementIndexes<Templates::RemoveConstReference<TTuples>...>{});
    }

    template <Concepts::NTupleReference TTuple>
    constexpr decltype(auto) TupleFlat(Forwarding<TTuple> tuple) noexcept
    {
        // The argument is a tuple: flatten each element recursively and return their concatenation.

        using Syntropy::TupleCat;
        
        auto flat = [&]<Int... VTupleIndex>(Forwarding<TTuple> tuple, Templates::Sequence<VTupleIndex...>)
        {
            return TupleCat(Details::TupleFlat(Get<VTupleIndex>(Forward<TTuple>(tuple)))...);
        };

        return flat(Forward<TTuple>(tuple), Templates::TupleSequenceFor<Templates::RemoveConstReference<TTuple>>{});
    }

    template <typename TElement>
    constexpr decltype(auto) TupleFlat(Forwarding<TElement> element) noexcept
    {
        // The argument is not a tuple: wrap it in a 1-tuple and end recursion.

        return MakeTuple(Forward<TElement>(element));
    }

}

// ===========================================================================
