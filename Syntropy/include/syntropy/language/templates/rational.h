
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

    /// \brief A reduced rational number of the form Numerator / Denominator.
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

    /// \brief ALias type for the reduced value of a rational number.
    template <typename TRational>
    using RationalReduce = typename Details::RationalReduce<TRational>::Type;

    /// \brief Alias type for the sum of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalSum = typename Details::RationalSum<T0Rational, T1Rational>::Type;

    /// \brief Alias type for the difference of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalDifference = typename Details::RationalDifference<T0Rational, T1Rational>::Type;

    /// \brief Alias type for the product of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalProduct = typename Details::RationalProduct<T0Rational, T1Rational>::Type;

    /// \brief Alias type for the quotient of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalQuotient = typename Details::RationalQuotient<T0Rational, T1Rational>::Type;


}