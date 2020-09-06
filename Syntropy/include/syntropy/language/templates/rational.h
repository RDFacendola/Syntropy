
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

    /// \brief ALias type for the reduced value of a rational number.
    template <typename TRational>
    using RationalReduce = typename Details::RationalReduce<TRational>::Type;

    /// \brief A reduced rational number of the form Numerator / Denominator.
    template <Int VNumerator, Int VDenominator>
    struct Rational
    {
        /// \brief Alias type for the reduced version of the represented fraction.
        using TReduced = RationalReduce<Rational<VNumerator, VDenominator>>;

        /// \brief Rational number numerator.
        static constexpr Int kNumerator = VNumerator;

        /// \brief Rational number denominator.
        static constexpr Int kDenominator = VDenominator;
    };

    /************************************************************************/
    /* RATIONAL ARITHMETIC                                                  */
    /************************************************************************/

    /// \brief Alias type for the sum of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalSum = typename Details::RationalSum<T0Rational, T1Rational>::Type::TReduced;

    /// \brief Alias type for the difference of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalDifference = typename Details::RationalDifference<T0Rational, T1Rational>::Type::TReduced;

    /// \brief Alias type for the product of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalProduct = typename Details::RationalProduct<T0Rational, T1Rational>::Type::TReduced;

    /// \brief Alias type for the quotient of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalQuotient = typename Details::RationalQuotient<T0Rational, T1Rational>::Type::TReduced;

    /************************************************************************/
    /* RATIONAL UNITS                                                       */
    /************************************************************************/

    /// \brief "nano" IS ratio.
    using Nano = Rational<1, 1000 * 1000 * 1000>;

    /// \brief "micro" IS ratio.
    using Micro = Rational<1, 1000 * 1000>;

    /// \brief "milli" IS ratio.
    using Milli = Rational<1, 1000>;

    /// \brief "centi" IS ratio.
    using Centi = Rational<1, 100>;

    /// \brief "deci" IS ratio.
    using Deci = Rational<1, 10>;

    /// \brief "deca" IS ratio.
    using Deca = Rational<10, 1>;

    /// \brief "hecto" IS ratio.
    using Hecto = Rational<100, 1>;

    /// \brief "kilo" IS ratio.
    using Kilo = Rational<1000, 1>;

    /// \brief "mega" IS ratio.
    using Mega = Rational<1000 * 1000, 1>;

    /// \brief "giga" IS ratio.
    using Giga = Rational<1000 * 1000 * 1000, 1>;

    /// \brief "tera" IS ratio.
    using Tera = Rational<1000 * 1000 * 1000 * 1000, 1>;

    /// \brief "peta" IS ratio.
    using Peta = Rational<1000 * 1000 * 1000 * 1000 * 1000, 1>;

    /// \brief "exa" IS ratio.
    using Exa = Rational<1000 * 1000 * 1000 * 1000 * 1000 * 1000, 1>;

}