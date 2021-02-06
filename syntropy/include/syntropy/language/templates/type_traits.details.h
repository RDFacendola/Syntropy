
/// \file traits_details.h
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/templates.h"
#include "syntropy/language/templates/diagnostics.h"
#include "syntropy/language/templates/sfinae.h"

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

    /// \brief Type equal to TTrue if VCondition is true, equal to TFalse
    ///        otherwise.
    template <Bool VCondition, typename TTrue, typename TFalse>
    using Conditional
        = std::conditional_t<VCondition, TTrue, TFalse>;

    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Discards the first VCount elements in a type list and provides
    ///        a type alias equal to a type list with the remaining elements.
    template <Int VCount, typename TTypeList>
    struct TypeListPopFrontHelper;

    /// \brief Specialization for type lists.
    template <Int VCount, typename TElement, typename... TElements>
    struct TypeListPopFrontHelper<VCount, TypeList<TElement, TElements...>>
        : TypeListPopFrontHelper<VCount - 1, TypeList<TElements...>>
    {

    };

    /// \brief End of recursion.
    template <typename TTypeList>
    struct TypeListPopFrontHelper<0, TTypeList>
    {
        using Type = TTypeList;
    };

    /// \brief Discards the first element of a type list until the first
    ///        element is equal to TType or the list is exhausted.
    template <typename TType, typename TFirst, typename... UTypes>
    struct TypeListIndexHelper : TypeListIndexHelper<TType, UTypes...> {};

    /// \brief Specialization for type lists starting with TType.
    template <typename TType, typename... UTypes>
    struct TypeListIndexHelper<TType, TType, UTypes...>
    {
        static_assert((!std::is_same_v<TType, UTypes> && ...),
                      "TType must appear exactly once in the type list.");

        static constexpr Int kValue = sizeof...(UTypes);
    };

    /// \brief Integer constant equal to the index of the first occurrence
    ///        TType in TTypeList.
    ///
    /// \remarks If TType doesn't appear in TTypeList the program
    ///          is ill-formed.
    template <typename TType, typename TTypeList>
    inline constexpr Int
    TypeListIndex = IllFormed<Int, TType, TTypeList>::kValue;

    /// \brief Specialization for type lists.
    template <typename TType, typename... UTypes>
    inline constexpr Int
    TypeListIndex<TType, TypeList<UTypes...>>
         = sizeof...(UTypes)
         - TypeListIndexHelper<TType, UTypes...>::kValue - 1;

    /// \brief Integer constant equal to the number of types in a type list.
    /// \remarks If TTypeList isn't a TypeList the program is ill-formed.
    template <typename TTypeList>
    inline constexpr Int
    TypeListRank = IllFormed<Int, TTypeList>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Int TypeListRank<TypeList<TTypes...>>
        = sizeof...(TTypes);

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    struct TypeListElementHelper;

    /// \brief Specialization for type lists.
    template <Int VIndex, typename TElement, typename... TElements>
    struct TypeListElementHelper<VIndex, TypeList<TElement, TElements...>>
        : TypeListElementHelper<VIndex - 1, TypeList<TElements...>>
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
    using TypeListElement
        = typename TypeListElementHelper<VIndex, TTypeList>::Type;

    /// \brief Discards the first VCount elements in a type list and provides
    ///        a type alias equal to a type list with the remaining elements.
    template <Int VCount, typename TTypeList>
    using TypeListPopFront
        = typename Details::TypeListPopFrontHelper<VCount, TTypeList>::Type;

    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Exposes a member type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced
    ///          contexts in template argument deduction.
    template <typename TType>
    struct IdentityHelper
    {
        using Type = TType;
    };

    /// \brief Type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced
    ///          contexts in template argument deduction.
    template <typename TType>
    using Identity = typename IdentityHelper<TType>::Type;

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and
    ///        function-to-pointer implicit conversions to the type TType,
    ///        removes cv-qualifiers, and defines the resulting type as the
    ///        member typedef type.
    template <typename TType>
    using Decay = std::decay_t<TType>;

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using RemoveConst = std::remove_const_t<TType>;

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using AddConst = std::add_const_t<TType>;

    /// \brief Type equal to TType without top-most reference if present, or
    ///        equal to TType otherwise.
    template <typename TType>
    using RemoveReference = std::remove_reference_t<TType>;

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    using RemoveConstReference = std::remove_cvref_t<TType>;

    /// \brief Type of an lvalue reference to TType if possible, or equal to
    ///        TType otherwise.
    ///
    /// \remarks This trait honors reference collapsing rule.
    template <typename TType>
    using AddLValueReference = std::add_lvalue_reference_t<TType>;

    /// \brief Type of an rvalue reference to TType if possible, or equal to
    ///        TType otherwise.
    ///
    /// \remarks This trait honors reference collapsing rule.
    template <typename TType>
    using AddRValueReference = std::add_rvalue_reference_t<TType>;

    /// \brief Type equal to TType with const lvalue reference applied.
    template <typename TType>
    using AddLValueConstReference
        = std::add_lvalue_reference_t<std::add_const_t<TType>>;

    /// \brief Type equal to a pointer to TType if possible, or equal to
    ///        TType otherwise.
    template <typename TType>
    using AddPointer = std::add_pointer_t<TType>;

    /// \brief Type equal the pointee's type of TType if TType is a pointer
    ///        type, or equal to TType otherwise.
    template <typename TType>
    using RemovePointer = std::remove_pointer_t<TType>;

    /************************************************************************/
    /* COMMON TYPE                                                          */
    /************************************************************************/

    // CommonType.
    // ===========

    template <typename TType>
    AddRValueReference<TType> Declval() noexcept;

    /// \brief Helper type trait for CommonType.
    template <typename... TTypes>
    struct CommonTypeHelper {};

    /// \brief Type equal to the type all types among TTypes can be
    ///        converted to.
    template <typename... TTypes>
    using CommonType = typename CommonTypeHelper<TTypes...>::Type;

    // One type.

    /// \brief Partial template specialization of CommonTypeHelper for a
    ///        single type.
    template <typename TType>
    struct CommonTypeHelper<TType> : CommonTypeHelper<TType, TType> {};

    // Two types.

    /// \brief Type two types can be implicitly converted to.
    template <typename TType, typename UType>
    using CommonTypeHelperConvertible
        = decltype(false ? Declval<TType>() : Declval<UType>());

    /// \brief Exposes a member Type equal to the common type between TType
    ///        and UType if such type exists, otherwise there's no such type.
    template <typename TType, typename UType, typename = void>
    struct CommonTypeHelper2{};

    /// \brief Partial template specialization for types convertible to a
    ///        common implicit type.
    template <typename TType, typename UType>
    struct CommonTypeHelper2<TType,
                             UType,
                             Void<CommonTypeHelperConvertible<TType, UType>>>
        : Alias<CommonTypeHelperConvertible<TType, UType>> {};

    /// \brief Partial template specialization of CommonTypeHelper for two
    ///        types.
    template <typename TFirst, typename TSecond>
    struct CommonTypeHelper<TFirst, TSecond>
        : CommonTypeHelper2<Decay<TFirst>, Decay<TSecond>> {};

    // Three or more types.

    template <typename TVoid,
              typename TType,
              typename UType,
              typename... TTypes>
    struct CommonTypeHelperN {};

    template <typename TType, typename UType, typename... TTypes>
    struct CommonTypeHelperN<
        Void<CommonType<TType, UType>>, TType, UType, TTypes...>
            : CommonType<CommonType<TType, UType>, TTypes...> {};

    /// \brief Partial template specialization of CommonTypeHelper for three
    ///        or more types.
    template <typename TType, typename UType, typename... TTypes>
    struct CommonTypeHelper<TType, UType, TTypes...>
        : Alias<CommonTypeHelperN<void, TType, UType, TTypes...>> {};

    // CommonReference.
    // ================

    /// \brief Helper type trait for CommonReference.
    template <typename... TTypes>
    struct CommonRefrenceHelper {};

    /// \brief Type equal to the type all types among TTypes can be converted
    ///        or bound to.
    template <typename... TTypes>
    using CommonReference = typename CommonRefrenceHelper<TTypes...>::Value;

    // One type.

    /// \brief Partial template specialization of CommonReferenceHelper for a
    ///        single type.
    template <typename TType>
    struct CommonRefrenceHelper<TType> : Alias<TType> {};

    // Two types.



    // Three or more types.

    template <typename TVoid,
              typename TType,
              typename UType,
              typename... TTypes>
    struct CommonRefrenceHelperN {};

    template <typename TType, typename UType, typename... TTypes>
    struct CommonRefrenceHelperN<
        Void<CommonReference<TType, UType>>, TType, UType, TTypes...>
            : CommonReference<CommonReference<TType, UType>, TTypes...> {};

    /// \brief Partial template specialization of CommonReferenceHelper for
    ///        three or more types.
    template <typename TType, typename UType, typename... TTypes>
    struct CommonRefrenceHelper<TType, UType, TTypes...>
        : Alias<CommonRefrenceHelperN<void, TType, UType, TTypes...>> {};

}

// ===========================================================================
