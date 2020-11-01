
/// \file sfinae.h
/// \brief This header is part of Syntropy language module. It contains SFINAE-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once


#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/sfinae_details.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* ENABLE IF                                                            */
    /************************************************************************/

    /// \brief Type equal to TType if VEnable is true, otherwise there's no such type.
    template <Bool VEnable>
    using EnableIf = Details::EnableIf<VEnable>;

    /************************************************************************/
    /* IS VALID EXPRESSION                                                  */
    /************************************************************************/

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a valid expression, false otherwise.
    template <template<typename...> typename TExpression, typename... TTypes>
    inline constexpr Bool IsValidExpression = Details::IsValidExpression<TExpression, TTypes...>;

    /************************************************************************/
    /* ENABLE IF VALID EXPRESSION                                           */
    /************************************************************************/

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression = Details::EnableIfValidExpression<TExpression, TTypes...>;

}