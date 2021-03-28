
/// \file type_list.h
///
/// \brief This header is part of Syntropy language module.
///        It contains type-list related definitions.
///
/// \author Raffaele D. Facendola - March 2021.

#pragma once

#include "syntropy/language/templates/concepts.h"

// ===========================================================================

#include "details/type_list.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    // TypeList.
    // =========

    /// \brief An ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    struct TypeList;

    // TypeList<T, ...>.
    // =================

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

    // TypeList<>
    // ----------

    /// \brief Specialization for empty lists.
    template <>
    struct TypeList<>
    {
        /// \brief Number of elements in a type list.
        static constexpr Int
        kCount = 0;
    };

    /************************************************************************/
    /* CONCEPT                                                              */
    /************************************************************************/

    /// \brief Concept for template arguments that bind to type-lists only.
    template <typename TTypeList>
    concept IsTypeList = IsTemplateSpecializationOf<TTypeList, TypeList>;

    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    // TypeList.
    // =========

    /// \brief Type of the first element in a type list.
    template <IsTypeList TTypes>
    using
    HeadOf = Details::HeadOf<TTypes>;

    /// \brief Drops the first element in a type list and return a list to
    ///        the remaining ones.
    template <IsTypeList TTypes>
    using
    RestOf = Details::RestOf<TTypes>;

    /// \brief Number of elements in a type list.
    template <IsTypeList TTypes>
    inline constexpr Int
    CountOf = TTypes::kCount;

    /// \brief Index of the first element in a type list wih a given type.
    template <typename TType, IsTypeList TTypes>
    inline constexpr Int
    IndexOf = Details::IndexOf<TType, TTypes>;

    /// \brief Type of a type list element, by index.
    template <Int TIndex, IsTypeList TTypes>
    using
    ElementOf = Details::ElementOf<TIndex, TTypes>;

    // Parameter pack.
    // ===============

    /// \brief Type of the first element in a parameter pack.
    template <typename... TTypes>
    using
    PackHeadOf = HeadOf<TypeList<TTypes...>>;

    /// \brief Drop the first element in a parameter pack and return a list to
    ///        the remaining ones.
    template <typename... TTypes>
    using
    PackRestOf = RestOf<TypeList<TTypes...>>;

    /// \brief Number of elements in a parameter pack.
    template <typename... TTypes>
    inline constexpr Int
    PackCountOf = sizeof...(TTypes);

    /// \brief Index of the first element in a parameter pack wih a given type.
    template <typename TType, typename... TTypes>
    inline constexpr Int
    PackIndexOf = IndexOf<TType, TypeList<TTypes...>>;

    /// \brief Type of a parameter pack element, by index.
    template <Int TIndex, typename... TTypes>
    using
    PackElementOf = ElementOf<TIndex, TypeList<TTypes...>>;

}

// ===========================================================================
