
/// \file templates.h
///
/// \brief This header is part of Syntropy language module.
///        It contains fundamental templates definitions.
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/diagnostics.h"

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
    template <typename TType, TType TValue>
    struct Constant
    {
        static constexpr TType kValue = TValue;
    };

    /// \brief Boolean constant.
    template <Bool TValue>
    using BoolConstant = Constant<Bool, TValue>;

    /// \brief Integer constant.
    template <Int TValue>
    using IntConstant = Constant<Int, TValue>;

    /// \brief True boolean constant.
    using True = BoolConstant<true>;

    /// \brief False boolean constant.
    using False = BoolConstant<false>;

    /************************************************************************/
    /* CONSTANT VALUE OF                                                    */
    /************************************************************************/

    /// \brief Constant equal to the value of the constant TConstant.
    /// \remarks If TConstant is not a specialization of Constant<>, the
    ///          program is ill-formed.
    template <typename TConstant>
    inline constexpr auto ConstantValueOf = IllFormed<void, TConstant>::kValue;

    /// \brief Partial specialization for Constants.
    template <typename TType, TType TValue>
    inline constexpr TType ConstantValueOf<Constant<TType,TValue>> = TValue;

}

// ===========================================================================
