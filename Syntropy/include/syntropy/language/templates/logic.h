
/// \file logic.h
/// \brief This header is part of Syntropy language module. It contains logic-related template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* CONJUNCTION                                                          */
    /************************************************************************/

    /// \brief Boolean value equal to the conjunction (logical AND) on a sequence of traits.
    template <typename... TPredicates>
    inline constexpr Bool Conjunction = std::conjunction_v<TPredicates...>;

    /************************************************************************/
    /* DISJUNCTION                                                          */
    /************************************************************************/

    /// \brief Boolean value equal to the disjunction (logical OR) on a sequence of traits.
    template <typename... TPredicates>
    inline constexpr Bool Disjunction = std::disjunction_v<TPredicates...>;

    /************************************************************************/
    /* NEGATION                                                             */
    /************************************************************************/

    /// \brief Boolean value equal to the negation (logical NOT) of a type traits.
    template <typename TPredicate>
    inline constexpr Bool Negation = std::negation_v<TPredicate>;

}