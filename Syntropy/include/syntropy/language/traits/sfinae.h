
/// \file sfinae.h
/// \brief This header is part of Syntropy language module. It contains SFINAE-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/traits/details/sfinae.h"
#include "syntropy/language/templates/constants.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* ENABLE IF                                                            */
    /************************************************************************/

    /// \brief Type equal to TType if kEnable is true, otherwise there's no such type.
    template <Bool kEnable, typename TType = void>
    using EnableIf = std::enable_if_t<kEnable, TType>;

    /************************************************************************/
    /* IS VALID EXPRESSION                                                  */
    /************************************************************************/

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a valid expression, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpression = Details::DetectValidExpression<void, TExpression, TTypes...>::value_;

    /************************************************************************/
    /* ENABLE IF VALID EXPRESSION                                           */
    /************************************************************************/

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression = EnableIf<IsValidExpression<TExpression, TTypes...>>;

}