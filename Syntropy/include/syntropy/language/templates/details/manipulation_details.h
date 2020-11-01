
/// \file manipulation_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of manipulation-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/templates/type_list.h"
#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/templates.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* REMOVE CONST                                                         */
    /************************************************************************/

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    struct RemoveConst : Alias<std::remove_const_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct RemoveConst<TypeList<TTypes...>> : Alias<TypeList<std::remove_const_t<TTypes>...>>{};

    /************************************************************************/
    /* ADD CONST                                                            */
    /************************************************************************/

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    struct AddConst : Alias<std::add_const_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddConst<TypeList<TTypes...>> : Alias<TypeList<std::add_const_t<TTypes>...>> {};

    /************************************************************************/
    /* REMOVE REFERENCE                                                     */
    /************************************************************************/

    /// \brief Type equal to TType without top-most reference if present, or equal to TType otherwise.
    template <typename TType>
    struct RemoveReference : Alias<std::remove_reference_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct RemoveReference<TypeList<TTypes...>> : Alias<TypeList<std::remove_reference_t<TTypes>...>> {};

    /************************************************************************/
    /* REMOVE CONST REFERENCE                                               */
    /************************************************************************/

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    struct RemoveConstReference : Alias<std::remove_cvref_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct RemoveConstReference<TypeList<TTypes...>> : Alias<TypeList<std::remove_cvref_t<TTypes>...>> {};

    /************************************************************************/
    /* ADD LVALUE REFERENCE                                                 */
    /************************************************************************/

    /// \brief Type of an lvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    /// This trait behave as TType&, except for the type "void" which is preserved.
    template <typename TType>
    struct AddLValueReference : Alias<std::add_lvalue_reference_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddLValueReference<TypeList<TTypes...>> : Alias<TypeList<std::add_lvalue_reference_t<TTypes>...>> {};

    /************************************************************************/
    /* ADD RVALUE REFERENCE                                                 */
    /************************************************************************/

    /// \brief Type of an rvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    /// This trait behave as TType&&, except for the type "void" which is preserved.
    template <typename TType>
    struct AddRValueReference : Alias<std::add_rvalue_reference_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddRValueReference<TypeList<TTypes...>> : Alias<TypeList<std::add_rvalue_reference_t<TTypes>...>> {};

    /************************************************************************/
    /* ADD LVALUE CONST REFERENCE                                           */
    /************************************************************************/

    /// \brief Type equal to TType with const lvalue reference applied.
    template <typename TType>
    struct AddLValueConstReference : Alias<std::add_lvalue_reference_t<std::add_const_t<TType>>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddLValueConstReference<TypeList<TTypes...>> : Alias<TypeList<std::add_lvalue_reference_t<std::add_const_t<TTypes>>...>> {};

    /************************************************************************/
    /* ADD POINTER                                                          */
    /************************************************************************/

    /// \brief Type equal to a pointer of TType if possible, or equal to TType otherwise.
    template <typename TType>
    struct AddPointer : Alias<std::add_pointer_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct AddPointer<TypeList<TTypes...>> : Alias<TypeList<std::add_pointer_t<TTypes>...>> {};

    /************************************************************************/
    /* REMOVE POINTER                                                       */
    /************************************************************************/

    /// \brief Type equal the pointee's type of TType if TType is a pointer type, or equal to TType otherwise.
    template <typename TType>
    struct RemovePointer : Alias<std::remove_pointer_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct RemovePointer<TypeList<TTypes...>> : Alias<TypeList<std::remove_pointer_t<TTypes>...>> {};

    /************************************************************************/
    /* COMMON TYPE                                                          */
    /************************************************************************/

    /// \brief Exposes a member type TType all types among TTypes can be converted to.
    template <typename... TTypes>
    struct CommonType : Alias<std::common_type_t<TTypes...>> {};

    /************************************************************************/
    /* DECAY                                                                */
    /************************************************************************/

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and function-to-pointer implicit conversions to the type TType,
    /// removes cv-qualifiers, and defines the resulting type as the member typedef type.
    template <typename TType>
    struct Decay : Alias<std::decay_t<TType>> {};

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    struct Decay<TypeList<TTypes...>> : Alias<TypeList<std::decay_t<TTypes>...>> {};

}