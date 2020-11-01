
/// \file relationships_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of relationships-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/type_list.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS SAME                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is equal to UType, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsSame = std::is_same_v<TType, UType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    inline constexpr Bool IsSame<TypeList<TTypes...>, TypeList<UTypes...>> = (IsSame<TTypes, UTypes> && ...);

    /************************************************************************/
    /* IS BASE OF                                                           */
    /************************************************************************/

    /// \brief Constant equal to true if TBase is a base class for TDerived or both are the same non-union class, equal to false otherwise.
    template <typename TBase, typename TDerived>
    inline constexpr Bool IsBaseOf = std::is_base_of_v<TBase, TDerived>;

    /************************************************************************/
    /* IS CONVERTIBLE                                                       */
    /************************************************************************/

    /// \brief Constant equal to true if TFrom is implicitly convertible to TTo, equal to false otherwise.
    template <typename TFrom, typename... TTos>
    inline constexpr Bool IsConvertible = std::is_convertible_v<TFrom, TTos...>;

    /// \brief Specialization for type lists.
    template <typename TFrom, typename... TFroms, typename TTo, typename... TTos>
    inline constexpr Bool IsConvertible<TypeList<TFrom, TFroms...>, TTo, TTos...> = std::is_convertible_v<TFrom, TTo> && IsConvertible<TypeList<TFroms...>, TTos...>;

    /// \brief Specialization for empty type-lists.
    template <>
    inline constexpr Bool IsConvertible<TypeList<>> = true;

    /************************************************************************/
    /* IS TEMPLATE SPECIALIZATION OF                                        */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsTemplateSpecializationOf = false;

    /// \brief Partial template specialization for template specializations (duh...).
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr Bool IsTemplateSpecializationOf<TTemplate<TTypes...>, TTemplate> = true;
}

// ===========================================================================
