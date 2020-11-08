
/// \file traits_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of type-traits machinery.
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* RANK                                                                 */
    /************************************************************************/

    /// \brief Constant equal to true if all TTypes have the same rank, false otherwise.
    template <typename TType, typename... TTypes>
    inline constexpr Bool SameRank = (SameRank<TType, TTypes> && ...);

    /// \brief Specialization for two types.
    template <typename TType, typename UType>
    inline constexpr Bool SameRank<TType, UType> = (Rank<TType> == Rank<UType>);

    /// \brief Specialization for one type. Always true.
    template <typename TType>
    inline constexpr Bool SameRank<TType> = true;

    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Represents a ordered sequence of types.
    /// \author Raffaele D. Facendola.
    template <typename... TTypes>
    struct TypeList {};

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

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename... TTypes>
    using Void = void;

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

    /// \brief Exposes a member type TType all types among TTypes can be converted to.
    template <typename... TTypes>
    using CommonType = std::common_type_t<TTypes...>;

    /************************************************************************/
    /* TYPE MANIPULATION                                                    */
    /************************************************************************/

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

    /// \brief Constant equal to true if TType is boolean, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsBoolean = std::is_same_v<TType, Bool>;

    /// \brief Constant equal to true if TType is integral, equal to false otherwise.
    /// \remarks Note that this trait is fundamentally different than std::is_integral as it only yields true for int and fixed-size ints. Booleans, characters and bytes are not considered to be integrals.
    template <typename TType>
    inline constexpr Bool IsIntegral = std::is_same_v<TType, Int> || std::is_same_v<TType, Fix8> || std::is_same_v<TType, Fix16> || std::is_same_v<TType, Fix32> || std::is_same_v<TType, Fix64>;

    /// \brief Constant equal to true if TType is a real number, equal to false otherwise.
    /// \remarks Note that this traits is different than std::is_floating_point as it only yields true for floats. Doubles are not supported.
    template <typename TType>
    inline constexpr Bool IsReal = std::is_same_v<TType, Float>;

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
    /* CONCEPTS                                                             */
    /************************************************************************/

    /// \brief Get a rvalue-reference to TType without calling any constructor.
    template <typename TType>
    AddRValueReference<TType> Declval() noexcept;

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasEqualityComparison = decltype(Declval<TType>() == Declval<UType>());

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasInequalityComparison = decltype(Declval<TType>() != Declval<UType>());

    /// \brief Self-contained namespace used to determine whether a type has an ADL-aware Swap implementation.
    namespace HasSwapDetector
    {
        // Undefined, triggers ADL.
        void Swap();

        /// \brief Check whether exists an ADL-aware function Swap for TType.
        template <typename TType>
        using Detect = decltype(Swap(Declval<TType&>(), Declval<TType&>));
    }

    /// \brief Check whether exists an ADL-aware function Swap for TType.
    template <typename TType>
    using HasSwap = HasSwapDetector::Detect<TType>;

    /************************************************************************/
    /* TYPE OPERATIONS                                                      */
    /************************************************************************/

    /// \brief Dummy method used to copy construct an instance.
    template <typename TType>
    void CopyConstruct(const TType&);

    /// \brief Check if TType is implicitly default constructible from an empty list.
    template <typename TType>
    using TestIsImplicitlyDefaultConstructible = decltype(CopyConstruct<TType>({}));

    /// \brief Check if TType is implicitly direct-constructible from a list of arguments.
    template <typename TType, typename... TArguments>
    using TestIsImplicitlyDirectConstructible = decltype(CopyConstruct<TType>({std::declval<TArguments>()...}));

    /// \brief Constant equal to true if TType is default-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsDefaultConstructible = std::is_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDefaultConstructible = std::is_trivially_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsImplicitlyDefaultConstructible = IsValidExpression<TestIsImplicitlyDefaultConstructible, TType>;

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
    inline constexpr Bool IsConstructibleHelper = IllFormed<TType, TArguments...>::kValue;

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
    inline constexpr Bool IsImplicitlyConstructibleHelper = IllFormed<TType, TArguments...>::kValue;

    /// \brief Specialization for arguments wrapped in type lists.
    /// Tests the first type with the first argument set and the remaining types with remaining argument sets recursively.
    template <typename TType, typename... TTypes, typename... TArguments, typename... TArgumentLists>
    inline constexpr Bool IsImplicitlyConstructibleHelper<true, TypeList<TType, TTypes...>, TypeList<TArguments...>, TArgumentLists...> = IsValidExpression<TestIsImplicitlyDirectConstructible, TType, TArguments...> && IsImplicitlyConstructibleHelper<true, TypeList<TTypes...>, TArgumentLists...>;

    // \brief Specialization for empty type list.
    template <>
    inline constexpr Bool IsImplicitlyConstructibleHelper<true, TypeList<>> = true;

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsImplicitlyConstructible = IsValidExpression<TestIsImplicitlyDirectConstructible, TType, TArguments...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... TArgumentLists>
    inline constexpr Bool IsImplicitlyConstructible<TypeList<TTypes...>, TArgumentLists...> = IsImplicitlyConstructibleHelper<sizeof...(TTypes) == sizeof...(TArgumentLists), TypeList<TTypes...>, TArgumentLists...>;

    // ===========================================================================

    /// \brief Constant equal to true if TType is trivially-destructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDestructible = std::is_trivially_destructible_v<TType>;

    // ===========================================================================

    /// \brief Constant equal to true if swapping two instances of TType results in trivial operations only, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallySwappable = (IsTriviallyDestructible<TType> && IsTriviallyMoveConstructible<TType> && IsTriviallyMoveAssignable<TType> && (!IsValidExpression<HasSwap, TType>));

    /************************************************************************/
    /* TYPE RELATIONSHIPS                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is equal to UType, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsSame = std::is_same_v<TType, UType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool IsSame<TypeList<TTypes...>, TypeList<UTypes...>> = (IsSame<TTypes, UTypes> && ...);

    /// \brief Constant equal to true if TBase is a base class for TDerived or both are the same non-union class, equal to false otherwise.
    template <typename TBase, typename TDerived>
    inline constexpr Bool IsBaseOf = std::is_base_of_v<TBase, TDerived>;

    /// \brief Constant equal to true if TFrom is implicitly convertible to TTo, equal to false otherwise.
    template <typename TFrom, typename TTo>
    inline constexpr Bool IsConvertible = std::is_convertible_v<TFrom, TTo>;

    /// \brief Specialization for type lists.
    template <typename TFrom, typename... TFroms, typename TTo, typename... TTos>
    inline constexpr Bool IsConvertible<TypeList<TFrom, TFroms...>, TypeList<TTo, TTos...>> = std::is_convertible_v<TFrom, TTo> && IsConvertible<TypeList<TFroms...>, TypeList<TTos...>>;

    /// \brief Specialization for empty type-lists.
    template <>
    inline constexpr Bool IsConvertible<TypeList<>, TypeList<>> = true;

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsTemplateSpecializationOf = false;

    /// \brief Partial template specialization for template specializations (duh...).
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr Bool IsTemplateSpecializationOf<TTemplate<TTypes...>, TTemplate> = true;

    /************************************************************************/
    /* FUNCTIONAL                                                           */
    /************************************************************************/

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for lambdas and callable objects.
    template <typename TCallable>
    struct FunctionArguments : FunctionArguments<decltype(&Decay<TCallable>::operator())>
    {
    
    };

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for non-const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct FunctionArguments<TReturn(TCallable::*)(TArguments...)>
    {
        using Type = TypeList<TArguments...>;
    };

    /// \brief Provides the a type alias Type, which is the argument types a callable object can be called with.
    /// Partial specialization for const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct FunctionArguments<TReturn(TCallable::*)(TArguments...) const>
    {
        using Type = TypeList<TArguments...>;
    };

    /************************************************************************/
    /* META                                                                 */
    /************************************************************************/

    /// \brief Type equal to TType if VEnable is true, otherwise there's no such type.
    template <Bool VEnable>
    using EnableIf = std::enable_if_t<VEnable>;

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

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression = EnableIf<IsValidExpression<TExpression, TTypes...>>;

}

// ===========================================================================