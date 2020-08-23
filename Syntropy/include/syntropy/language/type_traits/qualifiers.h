
/// \file qualifiers.h
/// \brief This header is part of Syntropy language module. It contains qualifiers-related type traits.
///
/// This header contains mostly aliases to the standard traits, eventually modified to suite Syntropy needs and to enforce code style consistency.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/fundamentals.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* IS CONST                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is constant, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsConst = std::is_const_v<TType>;

    /************************************************************************/
    /* REMOVE CONST                                                         */
    /************************************************************************/

    /// \brief Type equal to TType without const qualifier.
    template <typename TType>
    using RemoveConst = std::remove_const_t<TType>;

    /************************************************************************/
    /* ADD CONST                                                            */
    /************************************************************************/

    /// \brief Type equal to TType with const qualifiers applied.
    template <typename TType>
    using AddConst = std::add_const_t<TType>;

}