
/// \file traits_details.h
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Obtain the very same type of Type, establishing a non-deduced
    ///        context during template argument deduction.
    template <typename TType>
    using ExactOf = typename Alias<TType>::Type;

    /// \brief Obtain the value-type of TType without qualifiers.
    template <typename TType>
    using UnqualifiedOf = std::remove_cvref_t<TType>;

    /// \brief Obtain the value-type of TType preserving existing qualifiers.
    template <typename TType>
    using QualifiedOf = std::remove_reference_t<TType>;

    /// \brief Obtain a reference-type to a mutable instance of TType.
    template <typename TType>
    using MutableOf
        = std::add_lvalue_reference_t<UnqualifiedOf<TType>>;

    /// \brief Obtain a reference-type to an immutable instance of TType.
    template <typename TType>
    using ImmutableOf
        = std::add_lvalue_reference_t<std::add_const_t<UnqualifiedOf<TType>>>;

    /// \brief Obtain a reference-type to a mutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using MovableOf
        = std::add_rvalue_reference_t<UnqualifiedOf<TType>>;

    /// \brief Obtain a reference-type to an immutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using ImmovableOf
        = std::add_rvalue_reference_t<std::add_const_t<UnqualifiedOf<TType>>>;

    /// \brief Obtain the reference-type of TType preserving existing
    ///        qualifiers.
    template <typename TType>
    using ReferenceOf = std::add_lvalue_reference_t<TType>;

    /// \brief Obtain the forwarding-reference-type of TType preserving
    ///        existing qualifiers.
    ///
    /// This transform honors reference collapse rule.
    template <typename TType>
    using ForwardingOf = std::add_rvalue_reference_t<TType>;


    /// \brief Convert a function type to a function pointer.
    template <typename TFunction>
    using FunctionOf = std::add_pointer_t<TFunction>;

}

// ===========================================================================
