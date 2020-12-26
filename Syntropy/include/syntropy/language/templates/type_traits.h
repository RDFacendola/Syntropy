
/// \file type_traits.h
/// \brief This header is part of Syntropy language module. It contains definitions for type traits.
///
/// \author Raffaele D. Facendola - Nov 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/type_traits_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FOUNDAMENTALS                                                        */
    /************************************************************************/

    /// \brief Exposes a member Type equal to TType.
    template <typename TType>
    struct Alias
    {
        using Type = TType;
    };

    /// \brief Exposes a member Value equal to VValue.
    template <typename TType, TType VValue>
    struct Constant
    {
        static constexpr TType kValue = VValue;
    };

    /// \brief Boolean constant.
    template <Bool VValue>
    using BoolConstant = Constant<Bool, VValue>;

    /// \brief True boolean constant.
    using True = BoolConstant<true>;

    /// \brief False boolean constant.
    using False = BoolConstant<false>;

    /// \brief Integer constant.
    template <Int VValue>
    using IntConstant = Constant<Int, VValue>;

    /************************************************************************/
    /* META                                                                 */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to false. Useful for static_assert that should always trigger a compilation error.
    template <typename... TTypes>
    inline constexpr Bool AlwaysFalse = false;

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename... TTypes>
    using Void = Details::Void<TTypes...>;

    /// \brief Type equal to TTrue if VCondition is true, equal to TFalse otherwise.
    template <Bool VCondition, typename TTrue, typename TFalse>
    using Conditional = Details::Conditional<VCondition, TTrue, TFalse>;

    /// \brief Type equal to TType if VEnable is true, otherwise there's no such type.
    template <Bool VEnable>
    using EnableIf = Details::EnableIf<VEnable>;

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a valid expression, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpression = Details::IsValidExpression<TExpression, TTypes...>;

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression = Details::EnableIfValidExpression<TExpression, TTypes...>;

    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Represents a ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    struct TypeList {};

    /// \brief Integer constant equal to the index a type TType in TTypeList.
    /// \remarks If TType doesn't appear exactly once in TTypeList the program is ill-formed.
    template <typename TType, typename TTypeList>
    inline constexpr Int TypeListIndex = Details::TypeListIndex<TType, TTypeList>;

    /// \brief Integer constant equal to the number of types in a type list.
    /// \remarks If TTypeList isn't a TypeList the program is ill-formed.
    template <typename TTypeList>
    inline constexpr Int TypeListRank = Details::TypeListRank<TTypeList>;

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    using TypeListElement = Details::TypeListElement<VIndex, TTypeList>;

    /// \brief Discards the first VCount elements in a type list and provides a type list with the remaining ones.
    template <Int VCount, typename TTypeList>
    using TypeListPopFront = Details::TypeListPopFront<VCount, TTypeList>;

    /************************************************************************/
    /* MISCELLANEOUS                                                        */
    /************************************************************************/

    /// \brief Type equal to the type all types among TTypes can be converted to.
    template <typename... TTypes>
    using CommonType = Details::CommonType<TTypes...>;

    /// \brief Type equal to the type all types among TTypes can be converted or bound to.
    template <typename... TTypes>
    using CommonReference = Details::CommonReference<TTypes...>;

    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Identity transform meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    using Identity = Details::Identity<TType>;

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and function-to-pointer implicit conversions to the type TType,
    /// removes cv-qualifiers, and defines the resulting type as the member typedef type.
    template <typename TType>
    using Decay = Details::Decay<TType>;

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using RemoveConst = Details::RemoveConst<TType>;

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using AddConst = Details::AddConst<TType>;

    /// \brief Type equal to TType without top-most reference if present, or equal to TType otherwise.
    template <typename TType>
    using RemoveReference = Details::RemoveReference<TType>;

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    using RemoveConstReference = Details::RemoveConstReference<TType>;

    /// \brief Type of an lvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    using AddLValueReference = Details::AddLValueReference<TType>;

    /// \brief Type of an rvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    using AddRValueReference = Details::AddRValueReference<TType>;

    /// \brief Type equal to TType with const lvalue reference applied.
    template <typename TType>
    using AddLValueConstReference = Details::AddLValueConstReference<TType>;

    /// \brief Type equal to a pointer to TType if possible, or equal to TType otherwise.
    template <typename TType>
    using AddPointer = Details::AddPointer<TType>;

    /// \brief Type equal the pointee's type to TType if TType is a pointer type, or equal to TType otherwise.
    template <typename TType>
    using RemovePointer = Details::RemovePointer<TType>;

    /************************************************************************/
    /* TYPE CATEGORIES                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is void-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsVoid = Details::IsVoid<TType>;

    /// \brief Constant equal to true if TType is a null-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsNull = Details::IsNull<TType>;

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
    /* COMPOSITE TYPE CATEGORIES                                            */
    /************************************************************************/

    /// \brief Constant equal to true if TType is an object type, equal  o false otherwise.
    template <typename TType>
    inline constexpr Bool IsObject = Details::IsObject<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsObject<TypeList<TTypes...>> = (IsObject<TTypes> && ...);

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

    /// \brief Constant equal to true if TType is destructible, equal to false otherwise.
    /// A type is destructible if it is a reference type or, if equal to an object-type, if calling its destructor by means of t.~TType() in unevaluated context is well-formed.
    /// A type is *not* destructible if it is equal to void, a function type or an array type of unknown bounds. It is not destructible also if its destructor is ill-formed in unevaluated context.
    template <typename TType>
    inline constexpr Bool IsDestructible = Details::IsDestructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsDestructible<TypeList<TTypes...>> = (IsDestructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is both destructible and trivially-destructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDestructible = Details::IsTriviallyDestructible<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyDestructible<TypeList<TTypes...>> = (IsTriviallyDestructible<TTypes> && ...);

    /// \brief Constant equal to true if an instance of type TType can be compared equal to an instance of type UType, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsComparableForEquality = Details::IsComparableForEquality<TType, UType>;

    /************************************************************************/
    /* TYPE RELATIONSHIPS                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsTemplateSpecializationOf = Details::IsTemplateSpecializationOf<TType, TTemplate>;

    /************************************************************************/
    /* DECLVAL                                                              */
    /************************************************************************/

    /// \brief Convert TType as a reference type, without calling any constructor.
    /// \remarks this function shall never be evaluated as it has no definition.
    template <typename TType>
    Templates::AddRValueReference<TType> Declval() noexcept;

}

// ===========================================================================
