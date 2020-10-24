
/// \file relationship.h
/// \brief This header is part of Syntropy language module. It contains relationship-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/details/relationships_details.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* IS SAME                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is equal to UType, equal to false otherwise.
    template <typename T0Type, typename T1Type>
    inline constexpr Bool IsSame = std::is_same_v<T0Type, T1Type>;

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
    inline constexpr Bool IsConvertible = Details::IsConvertible<TFrom, TTo>;

    /************************************************************************/
    /* IS TEMPLATE SPECIALIZATION OF                                        */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsTemplateSpecializationOf = Details::IsTemplateSpecializationOf<TType, TTemplate>;

}