
/// \file operations_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of operations-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/type_list.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS CONSTRUCTIBLE                                                     */
    /************************************************************************/

    /// \brief Constant equal to true if TType can be constructed by TArguments... arguments, equal to false otherwise.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsConstructible = std::is_constructible_v<TType, TArguments...>;

    /************************************************************************/
    /* IS IMPLICITLY DEFAULT CONSTRUCTIBLE                                  */
    /************************************************************************/

    /// \brief Dummy function which accept a copy-constructed argument of type TType.
    template <typename TType>
    void CopyConstruct(const TType&);

    /// \brief Check whether TType is copy-list-initializable from {}.
    template <typename TType>
    using IsImplicitlyDefaultConstructible = decltype(CopyConstruct<TType>({}));

    /************************************************************************/
    /* ALL CONSTRUCTIBLE                                                    */
    /************************************************************************/

    /// \brief Constant equal to true if TFroms and TTos have the same rank and each type in TTypes is constructible to its respective type in TTos, equal to false otherwise.
    template <Bool VSame, typename TTypes, typename TArguments>
    inline constexpr Bool AllConstructible = false;

    /// \brief Specialization for same size type lists.
    template <typename... TTypes, typename... TArguments>
    inline constexpr Bool AllConstructible<true, TypeList<TTypes...>, TypeList<TArguments...>> = !Templates::Conjunction<IsConstructible<TTypes, TArguments>...>;

}