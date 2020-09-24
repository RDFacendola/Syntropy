
/// \file manipulation.h
/// \brief This header is part of Syntropy language module. It contains manipulation-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* REMOVE CONST                                                         */
    /************************************************************************/

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using RemoveConst = std::remove_const_t<TType>;

    /************************************************************************/
    /* ADD CONST                                                            */
    /************************************************************************/

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using AddConst = std::add_const_t<TType>;

    /************************************************************************/
    /* REMOVE REFERENCE                                                     */
    /************************************************************************/

    /// \brief Type equal to TType without top-most reference if present, or equal to TType otherwise.
    template <typename TType>
    using RemoveReference = std::remove_reference_t<TType>;

    /************************************************************************/
    /* REMOVE CONST REFERENCE                                               */
    /************************************************************************/

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    using RemoveConstReference = RemoveConst<RemoveReference<TType>>;

    /************************************************************************/
    /* ADD LVALUE REFERENCE                                                 */
    /************************************************************************/

    /// \brief Type of an lvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    /// This trait behave as TType&, except for the type "void" which is preserved.
    template <typename TType>
    using AddLValueReference = std::add_lvalue_reference_t<TType>;

    /************************************************************************/
    /* ADD RVALUE REFERENCE                                                 */
    /************************************************************************/

    /// \brief Type of an rvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    /// This trait behave as TType&&, except for the type "void" which is preserved.
    template <typename TType>
    using AddRValueReference = std::add_rvalue_reference_t<TType>;

    /************************************************************************/
    /* ADD POINTER                                                          */
    /************************************************************************/

    /// \brief Type equal to a pointer of TType if possible, or equal to TType otherwise.
    template <typename TType>
    using AddPointer = std::add_pointer_t<TType>;

    /************************************************************************/
    /* REMOVE POINTER                                                       */
    /************************************************************************/

    /// \brief Type equal the pointee's type of TType if TType is a pointer type, or equal to TType otherwise.
    template <typename TType>
    using RemovePointer = std::remove_pointer_t<TType>;

    /************************************************************************/
    /* COMMON TYPE OF                                                       */
    /************************************************************************/

    /// \brief Determine the common type among all TTypes, that is the type to which all TTypes can be converted to.
    template <typename... TTypes>
    using CommonTypeOf = std::common_type_t<TTypes...>;

    /************************************************************************/
    /* DECAY                                                                */
    /************************************************************************/

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and function-to-pointer implicit conversions to the type TType,
    /// removes cv-qualifiers, and defines the resulting type as the member typedef type.
    template <typename TType>
    using Decay = std::decay_t<TType>;

}