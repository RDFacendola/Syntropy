
/// \file traits_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of type-traits machinery.
///
/// \author Raffaele D. Facendola - Nov 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/foundation/enumerations.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename... TTypes>
    struct TypeList;
}

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* META                                                                 */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename... TTypes>
    using Void = void;

    // ===========================================================================

    /// \brief Type equal to TTrue if VCondition is true, equal to TFalse otherwise.
    template <Bool VCondition, typename TTrue, typename TFalse>
    using Conditional = std::conditional_t<VCondition, TTrue, TFalse>;

    // ===========================================================================

    /// \brief Type equal to TType if VEnable is true, otherwise there's no such type.
    template <Bool VEnable>
    using EnableIf = std::enable_if_t<VEnable>;

    // ===========================================================================

    /// \brief If TExpression<TTypes...> is a valid expression, exposes a member value equal to true, otherwise exposes a member value equal to false.
    /// Default trait for invalid expressions.
    /// \see Based on std::experimental::is_detected.
    template <typename TVoid, template<typename...> typename TExpression, typename... TTypes>
    struct IsValidExpressionHelper
    {
        static constexpr Bool kValue = false;
    };

    /// \brief Partial specialization for valid expressions.
    template <template<typename...> typename TExpression, typename... TTypes>
    struct IsValidExpressionHelper<Void<TExpression<TTypes...>>, TExpression, TTypes...>
    {
        static constexpr Bool kValue = true;
    };

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a valid expression, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpression = IsValidExpressionHelper<void, TExpression, TTypes...>::kValue;

    // ===========================================================================

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression = EnableIf<IsValidExpression<TExpression, TTypes...>>;

    // ===========================================================================

    /// \brief Discards the first element of a type list until the first element is equal to TType or the list is exhausted.
    template <typename TType, typename TFirst, typename... UTypes>
    struct TypeListIndexHelper : TypeListIndexHelper<TType, UTypes...>
    {
    
    };

    /// \brief Specialization for type lists starting with TType.
    template <typename TType, typename... UTypes>
    struct TypeListIndexHelper<TType, TType, UTypes...>
    {
        static_assert((!std::is_same_v<TType, UTypes> && ...), "TType must appear exactly once in the type list.");

        static constexpr Int kValue = sizeof...(UTypes);
    };

    /// \brief Integer constant equal to the index of the first occurrence TType in TTypeList.
    /// \remarks If TType doesn't appear in TTypeList the program is ill-formed.
    template <typename TType, typename TTypeList>
    inline constexpr Int TypeListIndex;

    /// \brief Specialization for type lists.
    template <typename TType, typename... UTypes>
    inline constexpr Int TypeListIndex<TType, TypeList<UTypes...>> = sizeof...(UTypes) - TypeListIndexHelper<TType, UTypes...>::kValue - 1;

    /// \brief Integer constant equal to the number of types in a type list.
    /// \remarks If TTypeList isn't a TypeList the program is ill-formed.
    template <typename TTypeList>
    inline constexpr Int TypeListRank;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Int TypeListRank<TypeList<TTypes...>> = sizeof...(TTypes);

    // ===========================================================================

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    struct TypeListElementHelper;

    /// \brief Specialization for type lists.
    template <Int VIndex, typename TElement, typename... TElements>
    struct TypeListElementHelper<VIndex, TypeList<TElement, TElements...>> : TypeListElementHelper<VIndex - 1, TypeList<TElements...>>
    {

    };

    /// \brief End of recursion.
    template <typename TElement, typename... TElements>
    struct TypeListElementHelper<0, TypeList<TElement, TElements...>>
    {
        using Type = TElement;
    };

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    using TypeListElement = typename TypeListElementHelper<VIndex, TTypeList>::Type;

    // ===========================================================================

    /// \brief Discards the first VCount elements in a type list and provides a type alias equal to a type list with the remaining elements.
    template <Int VCount, typename TTypeList>
    struct TypeListPopFrontHelper;

    /// \brief Specialization for type lists.
    template <Int VCount, typename TElement, typename... TElements>
    struct TypeListPopFrontHelper<VCount, TypeList<TElement, TElements...>> : TypeListPopFrontHelper<VCount - 1, TypeList<TElements...>>
    {

    };

    /// \brief End of recursion.
    template <typename TTypeList>
    struct TypeListPopFrontHelper<0, TTypeList>
    {
        using Type = TTypeList;
    };

    /// \brief Discards the first VCount elements in a type list and provides a type alias equal to a type list with the remaining elements.
    template <Int VCount, typename TTypeList>
    using TypeListPopFront = typename Details::TypeListPopFrontHelper<VCount, TTypeList>::Type;

    /************************************************************************/
    /* MISCELLANEOUS                                                        */
    /************************************************************************/

    /// \brief Type equal to the type all types among TTypes can be converted to.
    template <typename... TTypes>
    using CommonType = std::common_type_t<TTypes...>;

    /// \brief Type equal to the type all types among TTypes can be converted or bound to.
    template <typename... TTypes>
    using CommonReference = std::common_reference_t<TTypes...>;

    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Exposes a member type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    struct IdentityHelper
    {
        using Type = TType;
    };

    /// \brief Type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    using Identity = typename IdentityHelper<TType>::Type;

    // ===========================================================================

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and function-to-pointer implicit conversions to the type TType,
    /// removes cv-qualifiers, and defines the resulting type as the member typedef type.
    template <typename TType>
    struct DecayHelper
    {
        using Type = std::decay_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct DecayHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::decay_t<TTypes>...>;
    };

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and function-to-pointer implicit conversions to the type TType,
    /// removes cv-qualifiers, and defines the resulting type as the member typedef type.
    template <typename TType>
    using Decay = typename DecayHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    struct RemoveConstHelper
    {
        using Type = std::remove_const_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct RemoveConstHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::remove_const_t<TTypes>...> ;
    };

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using RemoveConst = typename RemoveConstHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    struct AddConstHelper
    {
        using Type = std::add_const_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddConstHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::add_const_t<TTypes>...>;
    };

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using AddConst = typename AddConstHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type equal to TType without top-most reference if present, or equal to TType otherwise.
    template <typename TType>
    struct RemoveReferenceHelper
    {
        using Type = std::remove_reference_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct RemoveReferenceHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::remove_reference_t<TTypes>...>;
    };

    /// \brief Type equal to TType without top-most reference if present, or equal to TType otherwise.
    template <typename TType>
    using RemoveReference = typename RemoveReferenceHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    struct RemoveConstReferenceHelper
    {
        using Type = std::remove_cvref_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct RemoveConstReferenceHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::remove_cvref_t<TTypes>...>;
    };

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    using RemoveConstReference = typename RemoveConstReferenceHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type of an lvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    struct AddLValueReferenceHelper
    {
        using Type = std::add_lvalue_reference_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddLValueReferenceHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::add_lvalue_reference_t<TTypes>...>;
    };

    /// \brief Type of an lvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    using AddLValueReference = typename AddLValueReferenceHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type of an rvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    struct AddRValueReferenceHelper
    {
        using Type = std::add_rvalue_reference_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddRValueReferenceHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::add_rvalue_reference_t<TTypes>...>;
    };

    /// \brief Type of an rvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    using AddRValueReference = typename AddRValueReferenceHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type equal to TType with const lvalue reference applied.
    template <typename TType>
    struct AddLValueConstReferenceHelper
    {
        using Type = std::add_lvalue_reference_t <std::add_const_t<TType>>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddLValueConstReferenceHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::add_lvalue_reference_t<std::add_const_t<TTypes>>...>;
    };

    /// \brief Type equal to TType with const lvalue reference applied.
    template <typename TType>
    using AddLValueConstReference = typename AddLValueConstReferenceHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type equal to a pointer to TType if possible, or equal to TType otherwise.
    template <typename TType>
    struct AddPointerHelper
    {
        using Type = std::add_pointer_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddPointerHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::add_pointer_t<TTypes>...>;
    };

    /// \brief Type equal to a pointer to TType if possible, or equal to TType otherwise.
    template <typename TType>
    using AddPointer = typename AddPointerHelper<TType>::Type;

    // ===========================================================================

    /// \brief Type equal the pointee's type of TType if TType is a pointer type, or equal to TType otherwise.
    template <typename TType>
    struct RemovePointerHelper
    {
        using Type = std::remove_pointer_t<TType>;
    };

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct RemovePointerHelper<TypeList<TTypes...>>
    {
        using Type = TypeList<std::remove_pointer_t<TTypes>...>;
    };

    /// \brief Type equal the pointee's type of TType if TType is a pointer type, or equal to TType otherwise.
    template <typename TType>
    using RemovePointer = typename RemovePointerHelper<TType>::Type;

    /************************************************************************/
    /* TYPE CATEGORIES                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is void-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsVoid = std::is_void_v<TType>;

    /// \brief Constant equal to true if TType is a null-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsNull = std::is_null_pointer_v<TType>;

    /// \brief Constant equal to true if TType is an enumeration type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsEnum = std::is_enum_v<TType>;

    /// \brief Constant equal to true if TType is a non-union class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsClass = std::is_class_v<TType>;

    /// \brief Constant equal to true if TType is a lvalue reference, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsLValueReference = std::is_lvalue_reference_v<TType>;

    /// \brief Constant equal to true if TType is a rvalue reference, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsRValueReference = std::is_rvalue_reference_v<TType>;

    /************************************************************************/
    /* COMPOSITE TYPE CATEGORIES                                            */
    /************************************************************************/

    /// \brief Constant equal to true if TType is an object type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsObject = std::is_object_v<TType>;

    /************************************************************************/
    /* TYPE PROPERTIES                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is constant, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsConst = std::is_const_v<TType>;

    /// \brief Constant equal to true if TType is constant (read-only), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadOnly = std::is_const_v<TType>;

    /// \brief Constant equal to true if TType is non-constant (read-write), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadWrite = !std::is_const_v<TType>;

    /// \brief Constant equal to true if TType is trivial, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTrivial = std::is_trivial_v<TType>;

    /// \brief Constant equal to true if TType is trivially copyable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyable = std::is_trivially_copyable_v<TType>;

    /// \brief Constant equal to true if TType is a standard-layout class, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsStandardLayout = std::is_standard_layout_v<TType>;

    /// \brief Constant equal to true if TType is polymorphic class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsPolymorphic = std::is_polymorphic_v<TType>;

    /// \brief Constant equal to true if TType is final class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsFinal = std::is_final_v<TType>;

    /************************************************************************/
    /* TYPE OPERATIONS                                                      */
    /************************************************************************/

    /// \brief Get a rvalue-reference to TType without calling any constructor.
    template <typename TType>
    AddRValueReference<TType> Declval() noexcept;

    /// \brief Dummy method used to copy construct an instance.
    template <typename TType>
    void CopyConstruct(const TType&);

    /// \brief Detect whether TType is implicitly default constructible from an empty list.
    template <typename TType>
    using DetectImplicitDefaultConstructor = decltype(CopyConstruct<TType>({}));

    /// \brief Detect whether TType is implicitly direct-constructible from a list of arguments.
    template <typename TType, typename... TArguments>
    using DetectImplicitDirectConstructor = decltype(CopyConstruct<TType>({ Declval<TArguments>()... }));

    /// \brief Detect whether an instance of TType can be compared equal to an instance of type UType.
    template <typename TType, typename UType>
    using DetectEqualityComparison = decltype(Declval<TType>() == Declval<UType>());

    // ===========================================================================

    /// \brief Constant equal to true if TType is default-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsDefaultConstructible = std::is_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDefaultConstructible = std::is_trivially_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsImplicitlyDefaultConstructible = IsValidExpression<DetectImplicitDefaultConstructor, TType>;

    // ===========================================================================

    /// \brief Constant equal to true if TType is copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyConstructible = std::is_copy_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyConstructible = std::is_trivially_copy_constructible_v<TType>;

    // ===========================================================================

    /// \brief Constant equal to true if TType is move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveConstructible = std::is_move_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveConstructible = std::is_trivially_move_constructible_v<TType>;

    // ===========================================================================

    /// \brief Constant equal to true if TType is copy-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyAssignable = std::is_copy_assignable_v<TType>;

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyAssignable = std::is_trivially_copy_assignable_v<TType>;

    // ===========================================================================

    /// \brief Constant equal to true if TType is move-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveAssignable = std::is_move_assignable_v<TType>;

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveAssignable = std::is_trivially_move_assignable_v<TType>;

    // ===========================================================================

    /// \brief Constant equal to true if TType is assignable from UType in unevaluated context, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsAssignable = std::is_assignable_v<TType, UType>;

    // ===========================================================================

    /// \brief Helper constant used to unwrap a type list to a variable number of template arguments.
    template <Bool VEnabled, typename TType, typename... TArguments>
    inline constexpr Bool IsConstructibleHelper;

    /// \brief Specialization for arguments wrapped in type lists.
    /// Tests the first type with the first argument set and the remaining types with remaining argument sets recursively.
    template <typename TType, typename... TTypes, typename... TArguments, typename... TArgumentLists>
    inline constexpr Bool IsConstructibleHelper<true, TypeList<TType, TTypes...>, TypeList<TArguments...>, TArgumentLists...> = std::is_constructible_v<TType, TArguments...> && IsConstructibleHelper<true, TypeList<TTypes...>, TArgumentLists...>;

    // \brief Specialization for empty type list.
    template <>
    inline constexpr Bool IsConstructibleHelper<true, TypeList<>> = true;

    /// \brief Constant equal to true if TType can be constructed by TArguments... arguments, equal to false otherwise.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsConstructible = std::is_constructible_v<TType, TArguments...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... TArgumentLists>
    inline constexpr Bool IsConstructible<TypeList<TTypes...>, TArgumentLists...> = IsConstructibleHelper<sizeof...(TTypes) == sizeof...(TArgumentLists), TypeList<TTypes...>, TArgumentLists...>;

    /// \brief Helper constant used to unwrap a type list to a variable number of template arguments.
    template <Bool VEnabled, typename TType, typename... TArguments>
    inline constexpr Bool IsImplicitlyConstructibleHelper;

    /// \brief Specialization for arguments wrapped in type lists.
    /// Tests the first type with the first argument set and the remaining types with remaining argument sets recursively.
    template <typename TType, typename... TTypes, typename... TArguments, typename... TArgumentLists>
    inline constexpr Bool IsImplicitlyConstructibleHelper<true, TypeList<TType, TTypes...>, TypeList<TArguments...>, TArgumentLists...> = IsValidExpression<DetectImplicitDirectConstructor, TType, TArguments...> && IsImplicitlyConstructibleHelper<true, TypeList<TTypes...>, TArgumentLists...>;

    // \brief Specialization for empty type list.
    template <>
    inline constexpr Bool IsImplicitlyConstructibleHelper<true, TypeList<>> = true;

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsImplicitlyConstructible = IsValidExpression<DetectImplicitDirectConstructor, TType, TArguments...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... TArgumentLists>
    inline constexpr Bool IsImplicitlyConstructible<TypeList<TTypes...>, TArgumentLists...> = IsImplicitlyConstructibleHelper<sizeof...(TTypes) == sizeof...(TArgumentLists), TypeList<TTypes...>, TArgumentLists...>;

    // ===========================================================================

    /// \brief Constant equal to true if TType is destructible, equal to false otherwise.
    /// A type is destructible if it is a reference type or, if equal to an object-type, if calling its destructor by means of t.~TType() in unevaluated context is well-formed.
    /// A type is *not* destructible if it is equal to void, a function type or an array type of unknown bounds. It is not destructible also if its destructor is ill-formed in unevaluated context.
    template <typename TType>
    inline constexpr Bool IsDestructible = std::is_destructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially-destructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDestructible = std::is_trivially_destructible_v<TType>;

    // ===========================================================================

    /// \brief Constant equal to true if an instance of type TType can be compared equal to an instance of type UType, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsComparableForEquality = IsValidExpression<DetectEqualityComparison, TType, UType>;

    /************************************************************************/
    /* TYPE RELATIONSHIPS                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsTemplateSpecializationOf = false;

    /// \brief Partial template specialization for template specializations (duh...).
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr Bool IsTemplateSpecializationOf<TTemplate<TTypes...>, TTemplate> = true;

}

// ===========================================================================