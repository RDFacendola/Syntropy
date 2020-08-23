
/// \file relationship.h
/// \brief This header is part of Syntropy language module. It contains relationship-related type traits.
///
/// This header contains mostly aliases to the standard traits, eventually modified to suite Syntropy needs and to enforce code style consistency.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/fundamentals.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* IS SAME                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is equal to UType, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsSame = std::is_same_v<TType, UType>;

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
    template <typename TFrom, typename TTo>
    inline constexpr Bool IsConvertible = std::is_convertible_v<TFrom, TTo>;

    /************************************************************************/
    /* IS TEMPLATE SPECIALIZATION OF                                        */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsTemplateSpecializationOf = false;

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr Bool IsTemplateSpecializationOf<TTemplate<TTypes...>, TTemplate> = true;

}