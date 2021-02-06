
/// \file templates.h
///
/// \brief This header is part of Syntropy language module.
///        It contains fundamental templates definitions.
///
/// \author Raffaele D. Facendola - Nov 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/type_traits_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPES                                                                */
    /************************************************************************/

    /// \brief Exposes a member Type equal to TType.
    template <typename TType>
    struct Alias
    {
        using Type = TType;
    };

    /************************************************************************/
    /* CONSTANTS                                                            */
    /************************************************************************/

    /// \brief Exposes a member Value equal to VValue.
    template <typename TType, TType VValue>
    struct Constant
    {
        static constexpr TType kValue = VValue;
    };

    /// \brief Boolean constant.
    template <Bool TValue>
    using BoolConstant = Constant<Bool, TValue>;

    /// \brief True boolean constant.
    using True = BoolConstant<true>;

    /// \brief False boolean constant.
    using False = BoolConstant<false>;

    /// \brief Integer constant.
    template <Int TValue>
    using IntConstant = Constant<Int, TValue>;
}

// ===========================================================================
