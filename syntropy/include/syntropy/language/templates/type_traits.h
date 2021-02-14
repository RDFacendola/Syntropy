
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

    /// \brief Get the type of the TIndex-th element in TTypeList.
    template <Int TIndex, typename TTypeList>
    using ElementOf = Details::ElementOf<TIndex, TTypeList>;

    /// \brief Drops the first TCount elements in a type list and return the
    ///        remaining ones;
    template <Int TCount, typename TTypeList>
    using Drop = Details::Drop<TCount, TTypeList>;

    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief IdentityOf transform meant to establish non-deduced contexts in
    ///        template argument deduction.
    template <typename TType>
    using IdentityOf = Details::IdentityOf<TType>;

    /// \brief Removes qualifiers and indirections and obtain the plain type
    ///        name.
    template <typename TType>
    using PlainOf = Details::PlainOf<TType>;

    /// \brief Obtain a reference to a mutable instance of TType.
    template <typename TType>
    using MutableOf = Details::MutableOf<TType>;

    /// \brief Obtain a reference to an immutable instance of TType.
    template <typename TType>
    using ImmutableOf = Details::ImmutableOf<TType>;

    /// \brief Obtain a reference to a mutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using MovableOf = Details::MovableOf<TType>;

    /// \brief Obtain a reference to an immutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using ImmovableOf = Details::ImmovableOf<TType>;

    /// \brief Obtain a pointer to an immutable instance of TType.
    template <typename TType>
    using ReadOnlyOf = Details::ReadOnlyOf<TType>;

    /// \brief Obtain a pointer to a mutable instance of TType.
    template <typename TType>
    using ReadWriteOf = Details::ReadWriteOf<TType>;

    /// \brief Convert a function type to a function pointer.
    template <typename TFunction>
    using FunctionOf = Details::FunctionOf<TFunction>;








    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and
    ///        function-to-pointer implicit conversions to the type TType,
    ///        removes cv-qualifiers, and defines the resulting type as the
    ///        member typedef type.
    template <typename TType>
    using Decay = Details::Decay<TType>;

    /// \brief Type equal to TType without top-most reference if present, or
    ///        equal to TType otherwise.
    template <typename TType>
    using RemoveReference = Details::RemoveReference<TType>;

    /// \brief Type equal to TType without top-most reference and qualifiers.
    template <typename TType>
    using RemoveConstReference = Details::RemoveConstReference<TType>;

    /// \brief Type of an lvalue reference to TType if possible, or equal to
    ///        TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    using AddLValueReference = Details::AddLValueReference<TType>;

    /// \brief Type of an rvalue reference to TType if possible, or equal to
    ///        TType otherwise.
    /// This trait honors reference collapsing rule.
    template <typename TType>
    using AddRValueReference = Details::AddRValueReference<TType>;

    /// \brief Type equal to TType with const lvalue reference applied.
    template <typename TType>
    using AddLValueConstReference = Details::AddLValueConstReference<TType>;

    /// \brief Type equal to a pointer to TType if possible, or equal to TType
    ///        otherwise.
    template <typename TType>
    using AddPointer = Details::AddPointer<TType>;

    /************************************************************************/
    /* DECLVAL                                                              */
    /************************************************************************/

    /// \brief Convert TType as a reference type, without calling any
    ///        constructor.
    ///
    /// \remarks this function shall never be evaluated as it has no definition.
    template <typename TType>
    AddRValueReference<TType> Declval() noexcept;

    /************************************************************************/
    /* COMMON TYPE                                                          */
    /************************************************************************/

    /// \brief Type equal to the type all types among TTypes can be
    ///        converted to.
    template <typename... TTypes>
    using CommonType = Details::CommonType<TTypes...>;

    /// \brief Type equal to the type all types among TTypes can be converted
    ///        or bound to.
    template <typename... TTypes>
    using CommonReference = Details::CommonReference<TTypes...>;

    /************************************************************************/
    /* META                                                                 */
    /************************************************************************/

    /// \brief Type equal to TTrue if VCondition is true, equal to
    ///        TFalse otherwise.
    template <Bool VCondition, typename TTrue, typename TFalse>
    using Conditional = Details::Conditional<VCondition, TTrue, TFalse>;

}

// ===========================================================================
