
/// \file templates.h
///
/// \brief This header is part of Syntropy language module.
///        It contains fundamental templates definitions.
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/diagnostics.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* ALIAS                                                                */
    /************************************************************************/

    /// \brief Exposes a member Type equal to TType.
    template <typename TType>
    struct Alias
    {
        using Type = TType;
    };

    /************************************************************************/
    /* CONSTANTS                                                            */
    /************************************************************************/

    /// \brief Exposes a member kValue equal to TValue.
    template <typename TType, TType TValue>
    struct Constant
    {
        static constexpr TType
        kValue = TValue;
    };

    /// \brief Boolean constant.
    template <Bool TValue>
    using BoolConstant
        = Constant<Bool, TValue>;

    /// \brief Integer constant.
    template <Int TValue>
    using IntConstant
        = Constant<Int, TValue>;

    /// \brief True boolean constant.
    using True
        = BoolConstant<true>;

    /// \brief False boolean constant.
    using False
        = BoolConstant<false>;

    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief An ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    struct TypeList;

    /// \brief Specialization for non-empty type lists.
    template <typename TType, typename... TTypes>
    struct TypeList<TType, TTypes...>
    {
        /// \brief Number of elements in a type list.
        static constexpr Int
        kCount = 1 + sizeof...(TTypes);

        /// \brief Type of the type-list itself.
        using SelfType = TypeList<TType, TTypes...>;

        /// \brief Type of the first type in the list.
        using HeadType = TType;

        /// \brief Type of the remaining types in the list.
        using RestTypes = TypeList<TTypes...>;
    };

    /// \brief Specialization for empty lists.
    template <>
    struct TypeList<>
    {
        /// \brief Number of elements in a type list.
        static constexpr Int
        kCount = 0;

        /// \brief Type of the type-list itself.
        using SelfType = TypeList<>;
    };

    /// \brief Exposes a member Type equal to TypeList<TTypes...>.
    template <typename... TTypes>
    struct AliasList
        : Alias<TypeList<TTypes...>> {};

    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief A compile-time sequence of integers.
    template <Int... TSequence>
    struct Sequence {};

    /// \brief Exposes a member Type equal to Sequence<TSequence...>.
    template <Int... TSequence>
    struct AliasSequence
        : Alias<Sequence<TSequence...>> {};

}

// ===========================================================================
