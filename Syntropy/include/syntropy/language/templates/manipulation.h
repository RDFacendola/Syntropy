
/// \file manipulation.h
/// \brief This header is part of Syntropy language module. It contains manipulation-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/manipulation_details.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* REMOVE CONST                                                         */
    /************************************************************************/

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using RemoveConst = typename Details::RemoveConst<TType>::Type;

    /************************************************************************/
    /* ADD CONST                                                            */
    /************************************************************************/

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using AddConst = typename Details::AddConst<TType>::Type;

    /************************************************************************/
    /* REMOVE REFERENCE                                                     */
    /************************************************************************/

    /// \brief Type equal to TType without top-most reference if present, or equal to TType otherwise.
    template <typename TType>
    using RemoveReference = typename Details::RemoveReference<TType>::Type;

    /************************************************************************/
    /* REMOVE CONST REFERENCE                                               */
    /************************************************************************/

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    using RemoveConstReference = typename Details::RemoveConstReference<TType>::Type;

    /************************************************************************/
    /* ADD LVALUE REFERENCE                                                 */
    /************************************************************************/

    /// \brief Type of an lvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    /// This trait behave as TType&, except for the type "void" which is preserved.
    template <typename TType>
    using AddLValueReference = typename Details::AddLValueReference<TType>::Type;

    /************************************************************************/
    /* ADD RVALUE REFERENCE                                                 */
    /************************************************************************/

    /// \brief Type of an rvalue reference to TType if possible, or equal to TType otherwise.
    /// This trait honors reference collapsing rule.
    /// This trait behave as TType&&, except for the type "void" which is preserved.
    template <typename TType>
    using AddRValueReference = typename Details::AddRValueReference<TType>::Type;

    /************************************************************************/
    /* ADD LVALUE CONST REFERENCE                                           */
    /************************************************************************/

    /// \brief Type equal to TType with const lvalue reference applied.
    template <typename TType>
    using AddLValueConstReference = typename Details::AddLValueConstReference<TType>::Type;

    /************************************************************************/
    /* ADD POINTER                                                          */
    /************************************************************************/

    /// \brief Type equal to a pointer of TType if possible, or equal to TType otherwise.
    template <typename TType>
    using AddPointer = typename Details::AddPointer<TType>::Type;

    /************************************************************************/
    /* REMOVE POINTER                                                       */
    /************************************************************************/

    /// \brief Type equal the pointee's type of TType if TType is a pointer type, or equal to TType otherwise.
    template <typename TType>
    using RemovePointer = typename Details::RemovePointer<TType>::Type;

    /************************************************************************/
    /* COMMON TYPE                                                          */
    /************************************************************************/

    /// \brief Determine the common type among all TTypes, that is the type to which all TTypes can be converted to.
    template <typename... TTypes>
    using CommonType = typename Details::CommonType<TTypes...>::Type;

    /************************************************************************/
    /* DECAY                                                                */
    /************************************************************************/

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and function-to-pointer implicit conversions to the type TType,
    /// removes cv-qualifiers, and defines the resulting type as the member typedef type.
    template <typename TType>
    using Decay = typename Details::Decay<TType>::Type;

}