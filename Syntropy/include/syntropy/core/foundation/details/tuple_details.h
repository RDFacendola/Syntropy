
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
    template <Int VCount, typename TTuple>
    struct TupleBaseHelper;

    /// \brief Specialization for tuples.
    template <Int VCount, typename TElement, typename... TElements>
    struct TupleBaseHelper<VCount, Tuple<TElement, TElements...>> : TupleBaseHelper<VCount - 1, Tuple<TElements...>> {};

    /// \brief End of recursion.
    template <typename TTuple>
    struct TupleBaseHelper<0, TTuple>
    {
        using Type = TTuple;
    };

    /// \brief Access a tuple base type by index.
    template <Int VCount, typename TTuple>
    using TupleBase = typename TupleBaseHelper<VCount, TTuple>::Type;

}

// ===========================================================================