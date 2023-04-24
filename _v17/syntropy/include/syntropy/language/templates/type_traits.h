
/// \file type_traits.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for type traits.
///
/// \author Raffaele D. Facendola - November 2020.

// ===========================================================================

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
    using IdentityOf
        = Details::IdentityOf<TType>;

    /// \brief Qualifier-removing value type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using UnqualifiedOf
        = Details::UnqualifiedOf<TType>;

    /// \brief Qualifier-preserving value type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using QualifiedOf
        = Details::QualifiedOf<TType>;

    /// \brief Read-only value type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ReadOnlyOf
        = Details::ReadOnlyOf<TType>;

    /// \brief Read-write value type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ReadWriteOf
        = Details::ReadWriteOf<TType>;

    /// \brief Immutable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ImmutableOf
        = Details::ImmutableOf<TType>;

    /// \brief Mutable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using MutableOf
        = Details::MutableOf<TType>;

    /// \brief Immovable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ImmovableOf
        = Details::ImmovableOf<TType>;

    /// \brief Movable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using MovableOf
        = Details::MovableOf<TType>;

    /// \brief Either mutable or immutable reference type of TType.
    ///
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ReferenceOf
        = Details::ReferenceOf<TType>;

    /// \brief Forwarding reference type of TType.
    ///
    /// \remarks This transform honors reference collapsing rule.
    /// \remarks Providing a TypeList yields a list whose elements have been
    ///          transformed accordingly.
    template <typename TType>
    using ForwardingOf
        = Details::ForwardingOf<TType>;

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

    /// \brief Concatenate and flattens all types in a single type list.
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

    /************************************************************************/
    /* SEQUENCE TRAITS                                                      */
    /************************************************************************/

    /// \brief Generate a contiguous sequence of TCount integers, starting
    ///        from 0.
    template <Int TCount>
    using MakeSequence
        = Details::MakeSequence<TCount>;

    /// \brief Generate a contiguous sequence the same size as a parameter
    ///        pack.
    template <typename... TTypes>
    using SequenceFor
        = Details::SequenceFor<TTypes...>;

    /// \brief Add a value to each element in a sequence.
    template <Int TValue, typename TSequence>
    using SequenceAdd
        = Details::SequenceAdd<TValue, TSequence>;

    /// \brief Concatenate zero or more sequences together.
    template <typename... TSequences>
    using SequenceConcatenate
        = Details::SequenceConcatenate<TSequences...>;

    /// \brief Create a sequence of a repeating value.
    template <Int TValue, Int TCount>
    using SequenceRepeat
        = Details::SequenceRepeat<TValue, TCount>;

    /************************************************************************/
    /* INVOKE TRAITS                                                        */
    /************************************************************************/

    /// \brief List of types needed to perform a callable object invocation.
    ///
    /// \remarks For member-functions, the first argument in the list is the
    ///          (possibly) qualified type of the class the function belongs
    ///          to. For functor objects the calling instance is implicit,
    ///          therefore its type is not present in the argument list.
    template <typename TCallable>
    using InvokeArgumentsOf
        = Details::InvokeArgumentsOf<TCallable>;

    /// \brief Provides indexed access to an invocation argument type.
    template <Int TIndex, typename TCallable >
    using InvokeArgumentOf
        = ElementTypeOf<TIndex, InvokeArgumentsOf<TCallable>>;

    /// \brief Result type of a callable object invocation.
    ///
    /// \remarks In case of member and non-member functions the argument list
    ///          is not necessary to determine the correct return type.
    ///          For functors, however, there might be several overloads,
    ///          therefore the argument types must be specified.
    template <typename TCallable, typename... TArguments>
    using InvokeResultOf
        = Details::InvokeResultOf<TCallable, TArguments...>;

}

// ===========================================================================
