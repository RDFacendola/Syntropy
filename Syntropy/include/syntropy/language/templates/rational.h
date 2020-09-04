
/// \file rational.h
/// \brief This header is part of Syntropy language module. It contains rational numbers template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/math.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RATIONAL                                                             */
    /************************************************************************/

    /// \brief A rational number of the form Numerator / Denominator
    template <Int kNumerator, Int kDenominator>
    struct Rational
    {
        /// \brief Rational number numerator.
        static constexpr Int numerator_ = kNumerator;

        /// \brief Rational number denominator.
        static constexpr Int denominator_ = kDenominator;
    };

    /************************************************************************/
    /* RATIONAL ARITHMETIC                                                  */
    /************************************************************************/

    /// \brief Add two rational numbers together.
    template <typename TRatio, typename URatio>
    using RationalAdd = Details::RationalAdd<TRatio, URatio>;

}