
/// \file traits.h
/// \brief This header is part of Syntropy language module. It contains definitions for type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/traits_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANK                                                                 */
    /************************************************************************/

    /// \brief Constant equal to the rank of TType. The program is ill-formed if TType's rank is meaningless.
    /// This trait is expected to be specialized by types having a "rank" concept.
    template <typename TType>
    inline constexpr Int Rank;

    /// \brief Constant equal to true if all TTypes have the same rank, false otherwise.
    template <typename TType, typename... TTypes>
    inline constexpr Bool SameRank = Details::SameRank<TType, TTypes...>;

    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Represents a ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    using TypeList = Details::TypeList<TTypes...>;

    /// \brief Integer constant equal to the index a type TType in TTypeList.
    /// \remarks If TType doesn't appear exactly once in TTypeList the program is ill-formed.
    template <typename TType, typename TTypeList>
    inline constexpr Int TypeListIndex = Details::TypeListIndex<TType, TTypeList>;

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    using TypeListElement = Details::TypeListElement<VIndex, TTypeList>;

    /// \brief Discards the first VCount elements in a type list and provides a type list with the remaining ones.
    template <Int VCount, typename TTypeList>
    using TypeListPopFront = Details::TypeListPopFront<VCount, TTypeList>;

    /// \brief Partial template specialization for TypeLists.
    template <typename... TTypes>
    inline constexpr Int Rank<TypeList<TTypes...>> = sizeof...(TTypes);

    /************************************************************************/
    /* MISCELLANEOUS                                                        */
    /************************************************************************/

    /// \brief Type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    using Identity = Details::Identity<TType>;

    /// \brief Boolean constant which consume any template argument and evaluates to false.
    template <typename... TTypes>
    inline constexpr Bool AlwaysFalse = false;

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename... TTypes>
    using Void = Details::Void<TTypes...>;

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and function-to-pointer implicit conversions to the type TType,
    /// removes cv-qualifiers, and defines the resulting type as the member typedef type.
    template <typename TType>
    using Decay = typename Details::Decay<TType>;

    /// \brief Determine the common type among all TTypes, that is the type to which all TTypes can be converted to.
    template <typename... TTypes>
    using CommonType = Details::CommonType<TTypes...>;

    /************************************************************************/
    /* TYPE MANIPULATION                                                    */
    /************************************************************************/

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using RemoveConst = typename Details::RemoveConst<TType>;

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using AddConst = typename Details::AddConst<TType>;

    /// \brief Type equal to TType without top-most reference if present, or equal to TType otherwise.
    template <typename TType>
    using RemoveReference = typename Details::RemoveReference<TType>;

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    using RemoveConstReference = typename Details::RemoveConstReference<TType>;

    /// \brief Type of an lvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    using AddLValueReference = typename Details::AddLValueReference<TType>;

    /// \brief Type of an rvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    using AddRValueReference = typename Details::AddRValueReference<TType>;

    /// \brief Type equal to TType with const lvalue reference applied.
    template <typename TType>
    using AddLValueConstReference = typename Details::AddLValueConstReference<TType>;

    /// \brief Type equal to a pointer to TType if possible, or equal to TType otherwise.
    template <typename TType>
    using AddPointer = typename Details::AddPointer<TType>;

    /// \brief Type equal the pointee's type to TType if TType is a pointer type, or equal to TType otherwise.
    template <typename TType>
    using RemovePointer = typename Details::RemovePointer<TType>;

    /************************************************************************/
    /* TYPE CATEGORIES                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is void-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsVoid = Details::IsVoid<TType>;

    /// \brief Constant equal to true if TType is a null-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsNull = Details::IsNull<TType>;

    /// \brief Constant equal to true if TType is boolean, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsBoolean = Details::IsBoolean<TType>;

    /// \brief Constant equal to true if TType is integral, equal to false otherwise.
    /// \remarks Note that this trait is fundamentally different than std::is_integral as it only yields true for int and fixed-size ints. Booleans, characters and bytes are not considered to be integrals.
    template <typename TType>
    inline constexpr Bool IsIntegral = Details::IsIntegral<TType>;

    /// \brief Constant equal to true if TType is a real number, equal to false otherwise.
    /// \remarks Note that this traits is different than std::is_floating_point as it only yields true for floats. Doubles are not supported.
    template <typename TType>
    inline constexpr Bool IsReal = Details::IsReal<TType>;

    /// \brief Constant equal to true if TType is an enumeration type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsEnum = Details::IsEnum<TType>;

    /// \brief Constant equal to true if TType is a non-union class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsClass = Details::IsClass<TType>;

    /// \brief Constant equal to true if TType is a lvalue reference, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsLValueReference = Details::IsLValueReference<TType>;

    /// \brief Constant equal to true if TType is a rvalue reference, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsRValueReference = Details::IsRValueReference<TType>;

    /************************************************************************/
    /* TYPE PROPERTIES                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is constant, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsConst = Details::IsConst<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsConst<TypeList<TTypes...>> = (IsConst<TTypes> && ...);

    /// \brief Constant equal to true if TType is constant (read-only), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadOnly = Details::IsReadOnly<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsReadOnly<TypeList<TTypes...>> = (IsReadOnly<TTypes> && ...);

    /// \brief Constant equal to true if TType is non-constant (read-write), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadWrite = Details::IsReadWrite<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsReadWrite<TypeList<TTypes...>> = (IsReadWrite<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivial, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTrivial = Details::IsTrivial<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTrivial<TypeList<TTypes...>> = (IsTrivial<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially copyable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyable = Details::IsTriviallyCopyable<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyCopyable<TypeList<TTypes...>> = (IsTriviallyCopyable<TTypes> && ...);

    /// \brief Constant equal to true if TType is a standard-layout class, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsStandardLayout = Details::IsStandardLayout<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsStandardLayout<TypeList<TTypes...>> = (IsStandardLayout<TTypes> && ...);

    /// \brief Constant equal to true if TType is polymorphic class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsPolymorphic = Details::IsPolymorphic<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsPolymorphic<TypeList<TTypes...>> = (IsPolymorphic<TTypes> && ...);

    /// \brief Constant equal to true if TType is final class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsFinal = Details::IsFinal<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsFinal<TypeList<TTypes...>> = (IsFinal<TTypes> && ...);

    /************************************************************************/
    /* TYPE CONCEPTS                                                        */
    /************************************************************************/

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasEqualityComparison = Details::HasEqualityComparison<TType, UType>;

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasInequalityComparison = Details::HasInequalityComparison<TType, UType>;

    /// \brief Check whether exists an ADL-aware function Swap for TType.
    template <typename TType>
    using HasSwap = Details::HasSwap<TType>;

    /************************************************************************/
    /* TYPE OPERATIONS                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is default-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsDefaultConstructible = Details::IsDefaultConstructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsDefaultConstructible<TypeList<TTypes...>> = (IsDefaultConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDefaultConstructible = Details::IsTriviallyDefaultConstructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyDefaultConstructible<TypeList<TTypes...>> = (IsTriviallyDefaultConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsImplicitlyDefaultConstructible = Details::IsImplicitlyDefaultConstructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsImplicitlyDefaultConstructible<TypeList<TTypes...>> = (IsImplicitlyDefaultConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyConstructible = Details::IsCopyConstructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsCopyConstructible<TypeList<TTypes...>> = (IsCopyConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyConstructible = Details::IsTriviallyCopyConstructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyCopyConstructible<TypeList<TTypes...>> = (IsTriviallyCopyConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveConstructible = Details::IsMoveConstructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsMoveConstructible<TypeList<TTypes...>> = (IsMoveConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveConstructible = Details::IsTriviallyMoveConstructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyMoveConstructible<TypeList<TTypes...>> = (IsTriviallyMoveConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is copy-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyAssignable = Details::IsCopyAssignable<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsCopyAssignable<TypeList<TTypes...>> = (IsCopyAssignable<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyAssignable = Details::IsTriviallyCopyAssignable<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyCopyAssignable<TypeList<TTypes...>> = (IsTriviallyCopyAssignable<TTypes> && ...);

    /// \brief Constant equal to true if TType is move-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveAssignable = Details::IsMoveAssignable<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsMoveAssignable<TypeList<TTypes...>> = (IsMoveAssignable<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveAssignable = Details::IsTriviallyMoveAssignable<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyMoveAssignable<TypeList<TTypes...>> = (IsTriviallyMoveAssignable<TTypes> && ...);

    /// \brief Constant equal to true if TType is assignable from UType in unevaluated context, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsAssignable = Details::IsAssignable<TType, UType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool IsAssignable<TypeList<TTypes...>, TypeList<UTypes...>> = (IsAssignable<TTypes, UTypes> && ...);

    /// \brief Constant equal to true if TType can be constructed by TArguments... arguments, equal to false otherwise.
    /// This trait supports TypeLists in the form IsConstructible<TypeList<A,B,C>, TypeList<a>, TypeList<b, bb>, TypeList<>>.
    /// TArguments must have the same exact rank as the number of elements in TType, otherwise the program is ill-formed.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsConstructible = Details::IsConstructible<TType, TArguments...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... TArguments>
    inline constexpr Bool IsConstructible<TypeList<TTypes...>, TypeList<TArguments...>> = Details::IsConstructible<TypeList<TTypes...>, TypeList<TArguments...>>;

    /// \brief Constant equal to true if TType can be constructed from an initializer list of TArguments, equal to false otherwise.
    /// This trait supports TypeLists in the form IsImplicitlyConstructible<TypeList<A,B,C>, TypeList<a>, TypeList<b, bb>, TypeList<>>.
    /// TArguments must have the same exact rank as the number of elements in TType, otherwise the program is ill-formed.
    /// [i] Given a function Foo(const TType&), TType is implicitly constructible if calling Foo({arguments...}) is well-formed.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsImplicitlyConstructible = Details::IsImplicitlyConstructible<TType, TArguments...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... TArguments>
    inline constexpr Bool IsImplicitlyConstructible<TypeList<TTypes...>, TypeList<TArguments...>> = Details::IsImplicitlyConstructible<TypeList<TTypes...>, TypeList<TArguments...>>;

    /// \brief Constant equal to true if TType is trivially-destructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDestructible = Details::IsTriviallyDestructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyDestructible<TypeList<TTypes...>> = (IsTriviallyDestructible<TTypes> && ...);

    /// \brief Constant equal to true if swapping two instances of TType results in trivial operations only, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallySwappable = Details::IsTriviallySwappable<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallySwappable<TypeList<TTypes...>> = (IsTriviallySwappable<TTypes> && ...);

    /************************************************************************/
    /* TYPE RELATIONSHIPS                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is equal to UType, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsSame = Details::IsSame<TType, UType>;

    /// \brief Constant equal to true if TBase is a base class for TDerived or both are the same non-union class, equal to false otherwise.
    template <typename TBase, typename TDerived>
    inline constexpr Bool IsBaseOf = Details::IsBaseOf<TBase, TDerived>;

    /// \brief Constant equal to true if TFrom is implicitly convertible to TTo, equal to false otherwise.
    template <typename TFrom, typename TTo>
    inline constexpr Bool IsConvertible = Details::IsConvertible<TFrom, TTo>;

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsTemplateSpecializationOf = Details::IsTemplateSpecializationOf<TType, TTemplate>;

    /************************************************************************/
    /* FUNCTIONAL                                                           */
    /************************************************************************/

    /// \brief Type alias equal to the argument types a callable object can be called with.
    /// If no matching element could be found, the program is ill-formed.
    template <typename TCallable>
    using FunctionArguments = Details::FunctionArguments<TCallable>;

    /************************************************************************/
    /* META                                                                 */
    /************************************************************************/

    /// \brief Type equal to TTrue if VCondition is true, equal to TFalse otherwise.
    template <Bool VCondition, typename TTrue, typename TFalse>
    using Conditional = std::conditional_t<VCondition, TTrue, TFalse>;

    /// \brief Type equal to TType if VEnable is true, otherwise there's no such type.
    template <Bool VEnable>
    using EnableIf = Details::EnableIf<VEnable>;

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a valid expression, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpression = Details::IsValidExpression<TExpression, TTypes...>;

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression = Details::EnableIfValidExpression<TExpression, TTypes...>;

}

// ===========================================================================
