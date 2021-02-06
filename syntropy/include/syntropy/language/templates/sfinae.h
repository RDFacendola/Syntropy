
/// \file sfinae.h
///
/// \brief This header is part of Syntropy language module.
///        It contains SFINAE definitions.
///
/// \author Raffaele D. Facendola - February 2021.

#pragma once

#include "syntropy/language/foundation/types.h"

// ===========================================================================

#include "sfinae.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* SFINAE                                                               */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename... TTypes>
    using Void = Details::Void<TTypes...>;

    /// \brief Type equal to TType if TEnable is true, otherwise
    ///        there's no such type.
    template <Bool TEnable>
    using EnableIf = Details::EnableIf<TEnable>;

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a
    ///        valid expression, false otherwise.
    template <template<typename...> typename TExpression,
              typename... TTypes>
    inline constexpr Bool IsValidExpression
        = Details::IsValidExpression<TExpression, TTypes...>;

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member
    ///        typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression
        = Details::EnableIfValidExpression<TExpression, TTypes...>;

}

// ===========================================================================
