
/// \file type_transform_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of fundamental type-transform type traits.
///
/// \author Raffaele D. Facendola - Nov 2020.

#pragma once

#include <type_traits>

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
    /* TYPE MANIPULATION                                                    */
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

}

// ===========================================================================