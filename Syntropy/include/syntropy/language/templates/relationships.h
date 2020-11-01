
/// \file relationship.h
/// \brief This header is part of Syntropy language module. It contains relationship-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/details/relationships_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* IS SAME                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is equal to UType, equal to false otherwise.
    template <typename TType, typename UType>
    inline constexpr Bool IsSame = Details::IsSame<TType, UType>;

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
    /// This traits support TypeLists in the form IsConvertible<TypeList<A,B,C>, E, F, G>.
    /// TTos size is expected to have the same rank as the number of elements in TFrom, otherwise the program is ill-formed.
    template <typename TFrom, typename... TTos>
    inline constexpr Bool IsConvertible = Details::IsConvertible<TFrom, TTos...>;

    /************************************************************************/
    /* IS TEMPLATE SPECIALIZATION OF                                        */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a specialization of TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool IsTemplateSpecializationOf = Details::IsTemplateSpecializationOf<TType, TTemplate>;

}

// ===========================================================================