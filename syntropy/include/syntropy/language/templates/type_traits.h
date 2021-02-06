
/// \file type_traits.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for type traits.
///
/// \author Raffaele D. Facendola - Nov 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/templates.h"
#include "syntropy/language/templates/details/type_traits_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* META                                                                 */
    /************************************************************************/

    /// \brief Type equal to TTrue if VCondition is true, equal to
    ///        TFalse otherwise.
    template <Bool VCondition, typename TTrue, typename TFalse>
    using Conditional = Details::Conditional<VCondition, TTrue, TFalse>;

    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Represents a ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    struct TypeList {};

    /// \brief Integer constant equal to the index a type TType in TTypeList.
    ///
    /// \remarks If TType doesn't appear exactly once in TTypeList the
    ///          program is ill-formed.
    template <typename TType, typename TTypeList>
    inline constexpr Int TypeListIndex
        = Details::TypeListIndex<TType, TTypeList>;

    /// \brief Integer constant equal to the number of types in a type list.
    ///
    /// \remarks If TTypeList isn't a TypeList the program is ill-formed.
    template <typename TTypeList>
    inline constexpr Int TypeListRank
        = Details::TypeListRank<TTypeList>;

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    using TypeListElement
        = Details::TypeListElement<VIndex, TTypeList>;

    /// \brief Discards the first VCount elements in a type list and provides
    ///        a type list with the remaining ones.
    template <Int VCount, typename TTypeList>
    using TypeListPopFront
        = Details::TypeListPopFront<VCount, TTypeList>;

    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Identity transform meant to establish non-deduced contexts in
    ///        template argument deduction.
    template <typename TType>
    using Identity = Details::Identity<TType>;

    /// \brief Applies lvalue-to-rvalue, array-to-pointer, and
    ///        function-to-pointer implicit conversions to the type TType,
    ///        removes cv-qualifiers, and defines the resulting type as the
    ///        member typedef type.
    template <typename TType>
    using Decay = Details::Decay<TType>;

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using RemoveConst = Details::RemoveConst<TType>;

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using AddConst = Details::AddConst<TType>;

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

    /// \brief Type equal the pointee's type to TType if TType is a pointer
    ///        type, or equal to TType otherwise.
    template <typename TType>
    using RemovePointer = Details::RemovePointer<TType>;

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

}

// ===========================================================================
