
/// \file constants.h
/// \brief This header is part of Syntropy language module. It contains constant-related template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* CONSTANT                                                             */
    /************************************************************************/

    /// \brief Wraps a static constant of type TType.
    template <typename TType, TType kValue>
    struct Constant
    {
        static constexpr TType value_ = kValue;
    };

    /************************************************************************/
    /* ALIAS                                                                */
    /************************************************************************/

    /// \brief Wraps an alias type Type.
    template <typename TType>
    struct Alias
    {
        using Type = typename TType;
    };

    /************************************************************************/
    /* TRUE                                                                 */
    /************************************************************************/

    /// \brief Boolean constant equal to true.
    using True = Constant<Bool, true>;

    /************************************************************************/
    /* FALSE                                                                */
    /************************************************************************/

    /// \brief Boolean constant equal to false.
    using False = Constant<Bool, false>;

    /************************************************************************/
    /* ALWAYS TRUE                                                          */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to true.
    template <typename...>
    inline constexpr Bool AlwaysTrue = false;

    /************************************************************************/
    /* ALWAYS FALSE                                                        */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to false.
    template <typename...>
    inline constexpr Bool AlwaysFalse = false;

    /************************************************************************/
    /* VOID                                                                 */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename...>
    using Void = void;
}