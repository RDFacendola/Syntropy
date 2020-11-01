
/// \file tuple_details.h
/// \brief This header is part of Syntropy core module. It contains implementation details of tuple classes and definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/type_list.h"

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/operations.h"
#include "syntropy/language/traits.h"

// ===========================================================================

namespace Syntropy::Experimental
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename... TTypes>
    struct Tuple;

    template <Int VIndex, typename TFunction, typename... TTuples>
    constexpr decltype(auto) ProjectionApply(TFunction&& function, TTuples&&... tuples) noexcept;
}

// ===========================================================================

namespace Syntropy::Experimental::Templates::Details
{
    /************************************************************************/
    /* TUPLE ELEMENT LIST                                                   */
    /************************************************************************/

    /// \brief Type alias equal to a type list of all elements in TTuple.
    template <typename TTuple>
    struct TupleElementListHelper : Syntropy::Templates::IllFormed<TTuple> {};

    /// \brief Type alias equal to a type list of all elements in TTuple.
    template <typename... TElements>
    struct TupleElementListHelper<Tuple<TElements...>> : Syntropy::Templates::Alias<Syntropy::Templates::TypeList<TElements...>> {};

    /// \brief Type alias equal to a type list of all elements in TTuple.
    template <typename TTuple>
    using TupleElementList = typename TupleElementListHelper<TTuple>::Type;

    /************************************************************************/
    /* TUPLE ELEMENT                                                        */
    /************************************************************************/

    /// \brief Provides indexed access to tuple elements' types.
    template <Int VIndex, typename TTuple>
    using TupleElement = typename Syntropy::Templates::TypeListElement<VIndex, TupleElementList<TTuple>>;

    /************************************************************************/
    /* TUPLE POP FRONT                                                      */
    /************************************************************************/

    /// \brief Discards the first VCount elements in a tuple and provides a type alias equal to a tuple with the remaining elements.
    template <Int VCount, typename TTuple>
    struct TuplePopFrontHelper;

    /// \brief Specialization for tuples.
    template <Int VCount, typename TElement, typename... TElements>
    struct TuplePopFrontHelper<VCount, Tuple<TElement, TElements...>> : TuplePopFrontHelper<VCount - 1, Tuple<TElements...>> {};

    /// \brief End of recursion.
    template <typename TTuple>
    struct TuplePopFrontHelper<0, TTuple> : Syntropy::Templates::Alias<TTuple> {};

    /// \brief Discards the first VCount elements in a tuple and provides a type alias equal to a tuple with the remaining elements.
    template <Int VCount, typename TTuple>
    using TuplePopFront = typename Details::TuplePopFrontHelper<VCount, TTuple>::Type;

    /************************************************************************/
    /* TUPLE SIZE                                                           */
    /************************************************************************/

    /// \brief Constant equal to the rank (size) of a tuple.
    template <typename TTuple>
    inline constexpr Int TupleSize = Syntropy::Templates::TypeListSize<TupleElementList<TTuple>>;

}

// ===========================================================================

namespace Syntropy::Experimental::Details
{
    /************************************************************************/
    /* EXPLICIT IF - TUPLE CONSTRUCTOR                                      */
    /************************************************************************/

    // (1)

    /// \brief False if all types in TTypeList are copy-list-initializable from {}, true otherwise.
    template <typename... TTypes>
    inline constexpr Bool ExplicitIfTupleDefaultConstructor = !Syntropy::Templates::IsImplicitlyDefaultConstructible<Syntropy::Templates::TypeList<TTypes...>>;

    // (2)

    /// \brief False if all types in TTypeList are copy-constructible, true otherwise.
    template <typename... TTypes>
    inline constexpr Bool ExplicitIfTupleDirectConstructor = !Syntropy::Templates::IsConvertible<Syntropy::Templates::TypeList<const TTypes&...>, TTypes...>;

    // (3)


    /// \brief False if each type in TTypeList can be constructed via their respective type in UTypeList, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingConstructor = Syntropy::Templates::IllFormed<TTypeList, UTypes...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingConstructor<Syntropy::Templates::TypeList<TTypes...>, UTypes...> = !Syntropy::Templates::IsConvertible<Syntropy::Templates::TypeList<UTypes&&...>, TTypes...>;

    // (4)

    /// \brief False if each type in TTypeList can be constructed via their respective const-referenced-qualified type in UTypeList, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingCopyConstructor = Syntropy::Templates::IllFormed<TTypeList, UTypes...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingCopyConstructor<Syntropy::Templates::TypeList<TTypes...>, UTypes...> = !Syntropy::Templates::IsConvertible<Syntropy::Templates::TypeList<const UTypes&...>, TTypes...>;

    // (5)

    /// \brief False if each type in TTypeList can be constructed via their respective type in UTypeList, true otherwise.
    template <typename TTypeList, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingMoveConstructor = Syntropy::Templates::IllFormed<TTypeList, UTypes...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool ExplicitIfTupleConvertingMoveConstructor<Syntropy::Templates::TypeList<TTypes...>, UTypes...> = !Syntropy::Templates::IsConvertible<Syntropy::Templates::TypeList<UTypes&&...>, TTypes...>;

    /************************************************************************/
    /* ENABLE IF - TUPLE CONSTRUCTOR                                        */
    /************************************************************************/

    // (1)

    /// \brief Enable (implicit) default tuple constructor if all types TTypes are default constructible and all types have implicit default constructor.
    template <typename TTypeList>
    using EnableIfTupleDefaultConstructor = typename Syntropy::Templates::EnableIf<Syntropy::Templates::IsDefaultConstructible<TTypeList>>*;

    // (2)

    /// \brief Enable direct tuple constructor if all types TTypes are implicitly copy-constructible.
    template <typename TTypeList>
    using EnableIfTupleDirectConstructor = typename Syntropy::Templates::EnableIf<Syntropy::Templates::IsCopyConstructible<TTypeList>>*;

    // (3)

    /// \brief Constant equal to true if TTypeList and UTypeLists have the same rank and all types in TTypeList can be member-wise constructed from the respective types in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingConstructorHelper<true, TTypeList, UTypeLists...> = Syntropy::Templates::IsConstructible<TTypeList, Syntropy::Templates::AddRValueReference<UTypeLists>...>;

    /// \brief Enable converting tuple constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingConstructor = typename Syntropy::Templates::EnableIf<EnableIfTupleConvertingConstructorHelper<Syntropy::Templates::TemplateArgumentsRank<TTypeList> == sizeof...(UTypes), TTypeList, Syntropy::Templates::TypeList<UTypes>...>>*;

    // (4)

    /// \brief True if all types in TTypeList can be member-wise converting-copy-constructed from the respective types in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingCopyConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingCopyConstructorHelper<true, TTypeList, UTypeLists...> = Syntropy::Templates::IsConstructible<TTypeList, Syntropy::Templates::AddLValueConstReference<UTypeLists>...>;

    /// \brief Specialization for 1-tuples. True if TType can be constructed from UType and the overload doesn't reduce to a copy-constructor.
    template <typename TType, typename UType>
    inline constexpr Bool EnableIfTupleConvertingCopyConstructorHelper<true, Syntropy::Templates::TypeList<TType>, Syntropy::Templates::TypeList<UType>> = Syntropy::Templates::IsConstructible<TType, const UType&>
        && !Syntropy::Templates::IsConvertible<const Tuple<UType>&, TType>
        && !Syntropy::Templates::IsConstructible<TType, const Tuple<UType>&>
        && !Syntropy::Templates::IsSame<TType, UType>;

    /// \brief Enable converting tuple copy-constructor if both TTypeList and UTypes have the same rank, all types in TTypeList can be member-wise converting-copy-constructed from the respective UType and the overload does not reduce to a copy-constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingCopyConstructor = typename Syntropy::Templates::EnableIf<EnableIfTupleConvertingCopyConstructorHelper<Syntropy::Templates::TemplateArgumentsRank<TTypeList> == sizeof...(UTypes), TTypeList, Syntropy::Templates::TypeList<UTypes>...>>*;

    // (5)

    /// \brief Enable converting tuple converting-move-constructor if all types in TTypeList can be member-wise converting-move-constructed from the respective types in UTypeLists.
    template <Bool VEnable, typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingMoveConstructorHelper = false;

    /// \brief Partial specialization for same-rank type lists.
    template <typename TTypeList, typename... UTypeLists>
    inline constexpr Bool EnableIfTupleConvertingMoveConstructorHelper<true, TTypeList, UTypeLists...> = Syntropy::Templates::IsConstructible<TTypeList, Syntropy::Templates::AddRValueReference<UTypeLists>...>;

    /// \brief Specialization for 1-tuples. True if TType can be constructed from UType and the overload doesn't reduce to a move-constructor.
    template <typename TType, typename UType>
    inline constexpr Bool EnableIfTupleConvertingMoveConstructorHelper<true, Syntropy::Templates::TypeList<TType>, Syntropy::Templates::TypeList<UType>> = Syntropy::Templates::IsConstructible<TType, UType&&>
        && !Syntropy::Templates::IsConvertible<Tuple<UType>, TType>
        && !Syntropy::Templates::IsConstructible<TType, Tuple<UType>>
        && !Syntropy::Templates::IsSame<TType, UType>;

    /// \brief Enable converting tuple move-constructor if both TTypeList and UTypes have the same rank, all types in TTypeList can be member-wise converting-move-constructed from the respective UType and the overload does not reduce to a move-constructor.
    template <typename TTypeList, typename... UTypes>
    using EnableIfTupleConvertingMoveConstructor = typename Syntropy::Templates::EnableIf<EnableIfTupleConvertingMoveConstructorHelper<Syntropy::Templates::TemplateArgumentsRank<TTypeList> == sizeof...(UTypes), TTypeList, Syntropy::Templates::TypeList<UTypes>...>>*;

    /************************************************************************/
    /* ENABLE IF - TUPLE ASSIGNMENT                                         */
    /************************************************************************/

    // (1)

    /// \brief Enable tuple copy assignment operator if all types in TTypeList are copy-assignable.
    template <typename TTypeList>
    using EnableIfTupleCopyAssignment = typename Syntropy::Templates::EnableIf<Syntropy::Templates::IsCopyAssignable<TTypeList>>*;

    // (2)

    /// \brief Enable tuple copy assignment operator if all types in TTypeList are move-assignable.
    template <typename TTypeList>
    using EnableIfTupleMoveAssignment = typename Syntropy::Templates::EnableIf<Syntropy::Templates::IsMoveAssignable<TTypeList>>*;

    // (3)

    /// \brief Enable tuple converting copy assignment operator if all types in TTypeList can be converting-copy-assigned from the corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingCopyAssignment = typename Syntropy::Templates::EnableIf<!Syntropy::Templates::IsSame<TTypeList, UTypeList> && Syntropy::Templates::IsAssignable<Syntropy::Templates::AddLValueReference<TTypeList>, Syntropy::Templates::AddLValueConstReference<UTypeList>>>*;

    // (4)

    /// \brief Enable tuple converting move assignment operator if all types in TTypeList can be converting-move-assigned from the corresponding element in UTypeList.
    template <typename TTypeList, typename UTypeList>
    using EnableIfTupleConvertingMoveAssignment = typename Syntropy::Templates::EnableIf<!Syntropy::Templates::IsSame<TTypeList, UTypeList> && Syntropy::Templates::IsAssignable<Syntropy::Templates::AddLValueReference<TTypeList>, UTypeList>>*;

    /************************************************************************/
    /* LOCKSTEP APPLY                                                       */
    /************************************************************************/

    /// \brief Apply a function to all tuples generated by projecting the I-th element of all provided tuples for each index in Is.
    template <typename TFunction, typename... TTuples, Int... VIndexes>
    constexpr void LockstepApply(Syntropy::Templates::IntegerSequence<VIndexes...>, TTuples&&... tuples, TFunction&& function) noexcept;

}

// ===========================================================================

namespace Syntropy::Experimental::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Lockstep apply.
    // ===============
    template <typename TFunction, typename... TTuples, Int... VIndexes>
    constexpr void LockstepApply(Syntropy::Templates::IntegerSequence<VIndexes...>, TFunction&& function, TTuples&&... tuples) noexcept
    {
        (ProjectionApply<VIndexes>(Forward<TFunction>(function), Forward<TTuples>(tuples)...), ...);
    }

}

// ===========================================================================