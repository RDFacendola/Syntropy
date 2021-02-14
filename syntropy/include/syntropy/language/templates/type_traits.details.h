
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

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Get the numer of elements in a type list.
    template <typename TTypeList>
    inline constexpr Int
    CountOf = IllFormed<Int, TTypeList>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Int CountOf<Syntropy::Templates::TypeList<TTypes...>>
        = sizeof...(TTypes);

    // =======================================================================

    /// \brief Discards elements from UTypes until the first element is equal
    ///        to TType or the list is exhausted.
    template <typename TType, typename TFirst, typename... UTypes>
    struct IndexOfHelper : IndexOfHelper<TType, UTypes...> {};

    /// \brief Specialization for type lists starting with TType.
    template <typename TType, typename... UTypes>
    struct IndexOfHelper<TType, TType, UTypes...>
        : Constant<Int, sizeof...(UTypes)> {};

    /// \brief Index of the first occurrence of TType in TTypeList.
    template <typename TType, typename TTypeList>
    inline constexpr Int
    IndexOf = IllFormed<Int, TType, TTypeList>::kValue;

    /// \brief Specialization for type lists.
    template <typename TType, typename... UTypes>
    inline constexpr Int
    IndexOf<TType, TypeList<UTypes...>>
         = sizeof...(UTypes)
         - IndexOfHelper<TType, UTypes...>::kValue - 1;

    // =======================================================================

    /// \brief Get the type of the TIndex-th element in TTypeList.
    template <Int TIndex, typename TTypeList>
    struct ElementOfHelper;

    /// \brief Specialization for type lists. Recursive.
    template <Int TIndex, typename TElement, typename... TElements>
    struct ElementOfHelper<TIndex, TypeList<TElement, TElements...>>
        : ElementOfHelper<TIndex - 1, TypeList<TElements...>>
    {

    };

    /// \brief End of recursion.
    template <typename TElement, typename... TElements>
    struct ElementOfHelper<0, TypeList<TElement, TElements...>>
    {
        using Type = TElement;
    };

    /// \brief Get the type of the TIndex-th element in TTypeList.
    template <Int TIndex, typename TTypeList>
    using ElementOf = typename ElementOfHelper<TIndex, TTypeList>::Type;

    // =======================================================================

    /// \brief Drops the first TCount elements in a type list and return the
    ///        remaining ones;
    template <Int TCount, typename TTypeList>
    struct DropHelper;

    /// \brief Specialization for type lists.
    template <Int TCount, typename TElement, typename... TElements>
    struct DropHelper<TCount, TypeList<TElement, TElements...>>
        : DropHelper<TCount - 1, TypeList<TElements...>>
    {

    };

    /// \brief End of recursion.
    template <typename TTypeList>
    struct DropHelper<0, TTypeList>
    {
        using Type = TTypeList;
    };

    /// \brief Drops the first TCount elements in a type list and return the
    ///        remaining ones;
    template <Int TCount, typename TTypeList>
    using Drop = typename Details::DropHelper<TCount, TTypeList>::Type;

    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief IdentityOf transform is meant to establish non-deduced
    ///        contexts in template argument deduction.
    template <typename TType>
    using IdentityOf = typename Alias<TType>::Type;

    // =======================================================================

    /// \brief Removes qualifiers and indirections and obtain the plain type
    ///        name.
    template <typename TType>
    struct PlainOfHelper
        : Alias<std::remove_cvref_t<TType>> {};

    /// \brief Specialization for pointers. Recursive.
    template <typename TType>
    struct PlainOfHelper<TType*>
        : PlainOfHelper<std::remove_cvref_t<TType>> {};

    /// \brief Removes qualifiers and indirections and obtain the plain type
    ///        name.
    template <typename TType>
    using PlainOf = typename PlainOfHelper<TType>::Type;

    // =======================================================================

    /// \brief Obtain a reference to a mutable instance of TType.
    template <typename TType>
    using MutableOf
        = std::add_lvalue_reference_t<PlainOf<TType>>;

    /// \brief Obtain a reference to an immutable instance of TType.
    template <typename TType>
    using ImmutableOf
        = std::add_lvalue_reference_t<std::add_const_t<PlainOf<TType>>>;

    /// \brief Obtain a reference to a mutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using MovableOf
        = std::add_rvalue_reference_t<PlainOf<TType>>;

    /// \brief Obtain a reference to an immutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using ImmovableOf
        = std::add_rvalue_reference_t<std::add_const_t<PlainOf<TType>>>;

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using ReadOnlyOf = std::add_pointer_t<std::add_const_t<PlainOf<TType>>>;

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using ReadWriteOf = std::add_pointer_t<PlainOf<TType>>;

    /// \brief Convert a function type to a function pointer.
    template <typename TFunction>
    using FunctionOf = std::add_pointer_t<TFunction>;

    // =======================================================================

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and
    ///        function-to-pointer implicit conversions to the type TType,
    ///        removes cv-qualifiers, and defines the resulting type as the
    ///        member typedef type.
    template <typename TType>
    using Decay = std::decay_t<TType>;

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

    /************************************************************************/
    /* META                                                                 */
    /************************************************************************/

    /// \brief Type equal to TTrue if VCondition is true, equal to TFalse
    ///        otherwise.
    template <Bool VCondition, typename TTrue, typename TFalse>
    using Conditional
        = std::conditional_t<VCondition, TTrue, TFalse>;
}

// ===========================================================================
