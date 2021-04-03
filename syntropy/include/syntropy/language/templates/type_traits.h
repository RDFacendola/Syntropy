
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

#include "details/type_traits.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Type identical to TType.
    ///        This transform establishesa non-deduced context during template
    ///        argument deduction.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ExactOf
        = typename Details::ExactOfHelper<TType>::Type;

    /// \brief Qualifier-removing value type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using UnqualifiedOf
        = typename Details::UnqualifiedOfHelper<TType>::Type;

    /// \brief Qualifier-preserving value type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using QualifiedOf
        = typename Details::QualifiedOfHelper<TType>::Type;

    /// \brief Mutable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using MutableOf
        = typename Details::MutableOfHelper<TType>::Type;

    /// \brief Immutable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ImmutableOf
        = typename Details::ImmutableOfHelper<TType>::Type;

    /// \brief Movable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using MovableOf
        = typename Details::MovableOfHelper<TType>::Type;

    /// \brief Immovable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using
    ImmovableOf
        = typename Details::ImmovableOfHelper<TType>::Type;

    /// \brief Either mutable or immutable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ReferenceOf
        = typename Details::ReferenceOfHelper<TType>::Type;

    /// \brief Forwarding reference type of TType.
    ///
    /// \remarks This transform honors reference collapsing rule.
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ForwardingOf
        = typename Details::ForwardingOfHelper<TType>::Type;

    /// \brief Function pointer type.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using FunctionOf
        = typename Details::FunctionOfHelper<TType>::Type;

    /************************************************************************/
    /* DECLVAL                                                              */
    /************************************************************************/

    /// \brief Obtain a forwarding-reference to an instance of TType in
    ///        unevaluated context.
    ///
    /// \remarks The function shall never be evaluated as it has no definition.
    template <typename TType>
    ForwardingOf<TType> Declval() noexcept;

    /************************************************************************/
    /* TYPE LIST TRAITS                                                     */
    /************************************************************************/

    /// \brief Concatenate all types in a single type list.
    template <typename... TTypes>
    using TypeListOf
        = Details::TypeListOf<TTypes...>;

    /// \brief Type of the first element in a type list or parameter pack.
    template <typename... TTypes>
    using HeadTypeOf
        = Details::HeadTypeOf<TTypes...>;

    /// \brief Drops the first element in a type list or parameter pack and
    ///        return a type list to the remaining ones.
    template <typename... TTypes>
    using RestTypeOf
        = Details::RestTypeOf<TTypes...>;

    /// \brief Type of a type list or parameter pack element, by index.
    template <Int TIndex, typename... TTypes>
    using ElementTypeOf
        = Details::ElementTypeOf<TIndex, TTypes...>;

    /// \brief Number of elements in a type list or a parameter pack.
    template <typename... TTypes>
    inline constexpr
    Int ElementCountOf
        = Details::ElementCountOf<TTypes...>;

    /// \brief Index of the first element in a type list or parameter pack with
    ///        a given type.
    template <typename TType, typename... TTypes>
    inline constexpr
    Int ElementIndexOf
        = Details::ElementIndexOf<TType, TTypes...>;

}

// ===========================================================================
