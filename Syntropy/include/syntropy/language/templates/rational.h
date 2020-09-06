
/// \file rational.h
/// \brief This header is part of Syntropy language module. It contains rational numbers template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/math.h"
#include "syntropy/language/templates/details/rational.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RATIONAL                                                             */
    /************************************************************************/

    /// \brief A rational number of the form Numerator / Denominator
    template <Int VNumerator, Int VDenominator>
    struct Rational
    {
        /// \brief Rational number numerator.
        static constexpr Int kNumerator = VNumerator;

        /// \brief Rational number denominator.
        static constexpr Int kDenominator = VDenominator;
    };

    /************************************************************************/
    /* RATIONAL ARITHMETIC                                                  */
    /************************************************************************/

    /// \brief Alias type for the sum of two rational numbers.
    template <typename T0Ratio, typename T1Ratio>
    using RationalSum = typename Details::RationalSum<T0Ratio, T1Ratio>::Type;

    /// \brief Alias type for the difference of two rational numbers.
    template <typename T0Ratio, typename T1Ratio>
    using RationalDifference = typename Details::RationalDifference<T0Ratio, T1Ratio>::Type;

    /// \brief Alias type for the product of two rational numbers.
    template <typename T0Ratio, typename T1Ratio>
    using RationalProduct = typename Details::RationalProduct<T0Ratio, T1Ratio>::Type;

    /// \brief Alias type for the quotient of two rational numbers.
    template <typename T0Ratio, typename T1Ratio>
    using RationalQuotient = typename Details::RationalQuotient<T0Ratio, T1Ratio>::Type;

}