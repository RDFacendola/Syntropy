
/// \file relationships_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of relationships-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/type_list.h"
#include "syntropy/language/templates/logic.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS CONVERTIBLE                                                       */
    /************************************************************************/

    /// \brief Constant equal to true if TFrom is implicitly convertible to TTo, equal to false otherwise.
    template <typename TFrom, typename TTo>
    inline constexpr Bool IsConvertible = std::is_convertible_v<TFrom, TTo>;

    /************************************************************************/
    /* ALL CONVERTIBLE                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TFromList and TToList have the same rank and each type in the former is convertible to its respective type in the latter, equal to false otherwise.
    template <Bool VSame, typename TFromList, typename TToList>
    inline constexpr Bool PairwiseConvertible = false;

    /// \brief Specialization for same rank type lists.
    template <typename... TFromList, typename... TToList>
    inline constexpr Bool PairwiseConvertible <true, TypeList<TFromList...>, TypeList<TToList...>> = !Templates::Conjunction<IsConvertible<TFromList, TToList>...>;

    /// \brief Constant equal to true if all element in TFromList are implicitly convertible to their respective element in TToList, equal to false otherwise.
    /// TFromList and TToList are expected to be same-ranked type lists, otherwise this constant is equal to false.
    template <typename TFromList, typename TToList>
    struct AreConvertible
    {
        static_assert(AlwaysFalse<TFromList, TToList>, "TFromList and TToList must be type lists.");
    };
    
    /// \brief Partial template specialization for type lists.
    template <typename... TFroms, typename... TTos>
    struct AreConvertible<TypeList<TFroms...>, TypeList<TTos...>>
    {
        static constexpr Bool kValue = PairwiseConvertible<sizeof...(TFroms) == sizeof...(TTos), TypeList<TFroms...>, TypeList<TTos...>>;
    };

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