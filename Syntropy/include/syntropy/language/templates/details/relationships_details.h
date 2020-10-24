
/// \file relationships_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of relationships-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/templates.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS CONVERTIBLE                                                       */
    /************************************************************************/

    /// \brief Constant equal to true if TFrom is implicitly convertible to TTo, equal to false otherwise.
    template <typename TFrom, typename TTo>
    inline constexpr Bool IsConvertible = std::is_convertible_v<TFrom, TTo>;

    /// \brief Specialization for type lists.
    template <typename TFrom, typename... TFroms, typename TTo, typename... TTos>
    inline constexpr Bool IsConvertible<TypeList<TFrom, TFroms...>, TypeList<TTo, TTos...>> = (IsConvertible<TFrom, TTo> && IsConvertible<TypeList<TFroms...>, TypeList<TTos...>>);

    /// \brief Specialization for empty type lists. Defaults to true.
    template <>
    inline constexpr Bool IsConvertible<TypeList<>, TypeList<>> = true;

    /// \brief Specialization for type lists of different ranks. Defaults to false.
    template <typename TFrom, typename... TFroms>
    inline constexpr Bool IsConvertible<TypeList<TFrom, TFroms...>, TypeList<>> = false;

    /// \brief Specialization for type lists of different ranks. Defaults to false.
    template <typename TTo, typename... TTos>
    inline constexpr Bool IsConvertible<TypeList<>, TypeList<TTo, TTos...>> = false;

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