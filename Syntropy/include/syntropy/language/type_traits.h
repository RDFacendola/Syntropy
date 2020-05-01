
/// \file type_traits.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <type_traits>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/syntropy.h"

#include <type_traits>

namespace syntropy
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief If TType is a specialization of TTemplate IsSpecializationV is true, false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr bool IsSpecializationV = false;

    /// \brief If TType is a specialization of TTemplate IsSpecializationV is true, false otherwise.
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr bool IsSpecializationV<TTemplate<TTypes...>, TTemplate> = true;

    /// \brief Evaluates to false.
    /// This value can be used to trigger static_assert that evaluates to false.
    template <typename...>
    inline constexpr bool AlwaysFalseV = false;

}