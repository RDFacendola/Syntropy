
/// \file type_transform.h
/// \brief This header is part of Syntropy language module. It contains definitions for fundamental type-transform type traits.
///
/// \author Raffaele D. Facendola - Nov 2020.

#pragma once

#include "syntropy/language/templates/details/type_transform_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Represents a ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    struct TypeList {};

    /************************************************************************/
    /* TYPE MANIPULATION                                                    */
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

}

// ===========================================================================
