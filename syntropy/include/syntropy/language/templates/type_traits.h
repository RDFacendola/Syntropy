
/// \file type_traits.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for type traits.
///
/// \author Raffaele D. Facendola - Nov 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/templates.h"

// ===========================================================================

#include "type_traits.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Get the numer of elements in a type list.
    template <typename TTypeList>
    inline constexpr Int CountOf = Details::CountOf<TTypeList>;

    /// \brief Get the index of the first occurence of TType in TTypeList.
    template <typename TType, typename TTypeList>
    inline constexpr Int IndexOf = Details::IndexOf<TType, TTypeList>;

    /// \brief Get the type of an element in TTypeList, by index.
    template <Int TIndex, typename TTypeList>
    using ElementOf = Details::ElementOf<TIndex, TTypeList>;

    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Obtain the very same type of Type, establishing a non-deduced
    ///        context during template argument deduction.
    template <typename TType>
    using ExactOf = Details::ExactOf<TType>;

    /// \brief Obtain the value-type of TType without qualifiers.
    template <typename TType>
    using UnqualifiedOf = Details::UnqualifiedOf<TType>;

    /// \brief Obtain the value-type of TType preserving existing qualifiers.
    template <typename TType>
    using QualifiedOf = Details::QualifiedOf<TType>;

    /// \brief Obtain a reference-type to a mutable instance of TType.
    template <typename TType>
    using MutableOf = Details::MutableOf<TType>;

    /// \brief Obtain a reference-type to an immutable instance of TType.
    template <typename TType>
    using ImmutableOf = Details::ImmutableOf<TType>;

    /// \brief Obtain a reference-type to a mutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using MovableOf = Details::MovableOf<TType>;

    /// \brief Obtain a reference-type to an immutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using ImmovableOf = Details::ImmovableOf<TType>;

    /// \brief Obtain the reference-type of TType preserving existing
    ///        qualifiers.
    ///
    /// This type behaves as MutableOf<TType> if TType refers to a mutable
    /// instance of TType or as Immutable<TType> otherwise.
    template <typename TType>
    using ReferenceOf = Details::ReferenceOf<TType>;

    /// \brief Obtain the forwarding-reference-type of TType preserving
    ///        existing qualifiers.
    ///
    /// This type behaves as MovableOf<TType> if TType refers to a mutable
    /// instance of TType or as ImmovableOf<TType> otherwise.
    template <typename TType>
    using ForwardingOf = Details::ForwardingOf<TType>;

    /// \brief Convert a function type to a function pointer.
    template <typename TFunction>
    using FunctionOf = Details::FunctionOf<TFunction>;

    /************************************************************************/
    /* DECLVAL                                                              */
    /************************************************************************/

    /// \brief Obtain a forwarding-reference to an instance of type TType in
    ///        unevaluated context.
    ///
    /// \remarks this function shall never be evaluated as it has no definition.
    template <typename TType>
    ForwardingOf<TType> Declval() noexcept;

}

// ===========================================================================
