
/// \file tuple_details.h
/// \brief This header is part of Syntropy core module. It contains implementation details of tuple classes and definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/operations.h"
#include "syntropy/language/traits.h"

namespace Syntropy::Experimental::Details
{
    template <typename... TTypes>
    using TypeList = Templates::TypeList<TTypes...>;

    /************************************************************************/
    /* TUPLE CONSTRUCTOR CONDITIONAL EXPLICIT                               */
    /************************************************************************/

    //// (1)

    /// \brief False if all types in TTypeList are copy-list-initializable from {}, true otherwise.
    template <typename TTypeList>
    inline constexpr Bool IsTupleDefaultConstructorExplicit = !Templates::AreImplicitlyDefaultConstructible<TTypeList>;

    //// (2)

    ///// \brief False if all types in TTypeList are copy-constructible, true otherwise.
    //template <typename TTypeList>
    //inline constexpr Bool IsTupleDirectConstructorExplicit = false;

    ///// \brief Specialization for type lists.
    //template <typename... TTypes>
    //inline constexpr Bool IsTupleDirectConstructorExplicit<TypeList<TTypes...>> = !IsConvertible<TypeList<const TTypes&...>, TypeList<TTypes...>>;

    //// (3)

    ///// \brief False if each type in TTypeList can be constructed via their respective type in UTypeList, true otherwise.
    //template <typename TTypeList, typename UTypeList>
    //inline constexpr Bool IsTupleConvertingConstructorExplicit = false;

    ///// \brief Specialization for type lists.
    //template <typename... TTypes, typename... UTypes>
    //inline constexpr Bool IsTupleConvertingConstructorExplicit<TypeList<TTypes...>, TypeList<UTypes...>> = !IsConvertible<TypeList<UTypes...>, TypeList<TTypes...>>;

    //// (4)

    ///// \brief False if each type in TTypeList can be constructed via their respective const-referenced-qualified type in UTypeList, true otherwise.
    //template <typename TTypeList, typename UTypeList>
    //inline constexpr Bool IsTupleConvertingCopyConstructorExplicit = false;

    ///// \brief Specialization for type lists.
    //template <typename... TTypes, typename... UTypes>
    //inline constexpr Bool IsTupleConvertingCopyConstructorExplicit<TypeList<TTypes...>, TypeList<UTypes...>> = IsConvertible<TypeList<const UTypes&...>, TypeList<TTypes...>>;

    //// (5)

    ///// \brief False if each type in TTypeList can be constructed via their respective type in UTypeList, true otherwise.
    //template <typename TTypeList, typename UTypeList>
    //inline constexpr Bool IsTupleConvertingMoveConstructorExplicit = false;

    ///// \brief Specialization for type lists.
    //template <typename... TTypes, typename... UTypes>
    //inline constexpr Bool IsTupleConvertingMoveConstructorExplicit<TypeList<TTypes...>, TypeList<UTypes...>> = IsConvertible<TypeList<UTypes...>, TypeList<TTypes...>>;


    /************************************************************************/
    /* TUPLE CONSTRUCTOR ENABLE IF                                          */
    /************************************************************************/

    //// (1)

    /// \brief Enable explicit default tuple constructor if all types in TTypeList are default constructible and there's at least one type whose default constructor is explicit.
    template <typename TType, typename... TTypes>
    using EnableTupleExplicitDefaultConstructor = Templates::EnableIf<Templates::AreDefaultConstructible<TypeList<TType, TTypes...>> && IsTupleDefaultConstructorExplicit<TypeList<TType, TTypes...>>>*;

    /// \brief Enable (implicit) default tuple constructor if all types in TTypeList are default constructible and all types have implicit default constructor.
    template <typename TType, typename... TTypes>
    using EnableTupleImplicitDefaultConstructor = Templates::EnableIf<Templates::AreDefaultConstructible<TypeList<TType, TTypes...>> && !IsTupleDefaultConstructorExplicit<TypeList<TType, TTypes...>>>*;
    
    //// (2)

    ///// \brief Enable direct tuple constructor if all types in TTypeList are copy-constructible.
    //template <typename TTypeList>
    //using TupleDirectConstructorEnableIf = Templates::EnableIf<Templates::IsCopyConstructible<TTypeList>>;

    //// (3)

    //template <typename TTypeList, typename UTypeList>
    //using TupleConvertingConstructorEnableIf = Templates::EnableIf<Templates::IsConstructible<TTypeList, UTypeList>>;

    //// (4)

    //template <typename TTypeList>
    //using TupleConvertingCopyConstructorEnableIf = Templates::EnableIf<Templates::AlwaysFalse<TTypeList>>;

    //// (5)

    //template <typename TTypeList>
    //using TupleConvertingMoveConstructorEnableIf = Templates::EnableIf<Templates::AlwaysFalse<TTypeList>>;



    ///************************************************************************/
    ///* LOCKSTEP APPLY                                                       */
    ///************************************************************************/

    ///// \brief Apply a function to all tuples generated by projecting the I-th element of all provided tuples for each index in Is.
    //template <typename TFunction, typename... TTuples, Int... Is>
    //inline constexpr void LockstepApply(TFunction&& function, TTuples&&... tuples, std::index_sequence<Is...>)
    //{
    //    (LockstepApplyAt<Is>(Forward<TFunction>(function), Forward<TTuples>(tuples)...), ...);
    //}
}