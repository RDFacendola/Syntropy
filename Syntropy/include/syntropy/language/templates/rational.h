
/// \file rational.h
/// \brief This header is part of Syntropy language module. It contains rational numbers template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>
#include <ratio>

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
        /// \brief Rational numerator.
        static constexpr Int kNumerator = std::ratio<VNumerator, VDenominator>::num;

        /// \brief Rational denominator.
        static constexpr Int kDenominator = std::ratio<VNumerator, VDenominator>::den;
    };

    /************************************************************************/
    /* RATIONAL ARITHMETIC                                                  */
    /************************************************************************/

    /// \brief Alias type for the sum of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalSum = Details::UnwrapRational<std::ratio_add<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>>;

    /// \brief Alias type for the difference of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalDifference = Details::UnwrapRational<std::ratio_subtract<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>>;

    /// \brief Alias type for the product of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalProduct = Details::UnwrapRational<std::ratio_multiply<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>>;

    /// \brief Alias type for the quotient of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    using RationalQuotient = Details::UnwrapRational<std::ratio_divide<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>>;

    /************************************************************************/
    /* RATIONAL COMPARISON                                                  */
    /************************************************************************/

    /// \brief Boolean constant equal to true if T0Rational and T1Rational represent the same amount, equal to false otherwise.
    template <typename T0Rational, typename T1Rational>
    inline constexpr Bool RationalEqual = std::ratio_equal_v<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>;

    /// \brief Boolean constant equal to true if T0Rational and T1Rational don't represent the same amount, equal to false otherwise.
    template <typename T0Rational, typename T1Rational>
    inline constexpr Bool RationalNotEqual = std::ratio_not_equal_v<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>;

    /// \brief Boolean constant equal to true if T0Rational represents an amount smaller than T1Rational, equal to false otherwise.
    template <typename T0Rational, typename T1Rational>
    inline constexpr Bool RationalLess = std::ratio_less_v<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>;

    /// \brief Boolean constant equal to true if T0Rational represents an amount smaller-than or equal-to T1Rational, equal to false otherwise.
    template <typename T0Rational, typename T1Rational>
    inline constexpr Bool RationalLessEqual = std::ratio_less_equal_v<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>;

    /// \brief Boolean constant equal to true if T0Rational represents an amount greater than T1Rational, equal to false otherwise.
    template <typename T0Rational, typename T1Rational>
    inline constexpr Bool RationalGreater = std::ratio_greater_v<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>;

    /// \brief Boolean constant equal to true if T0Rational represents an amount greater-than or equal-to T1Rational, equal to false otherwise.
    template <typename T0Rational, typename T1Rational>
    inline constexpr Bool RationalGreaterEqual = std::ratio_greater_equal_v<Details::WrapRational<T0Rational>, Details::WrapRational<T1Rational>>;

    /************************************************************************/
    /* RATIONAL UNITS                                                       */
    /************************************************************************/

    /// \brief "Nano" IS ratio.
    using Nano = Details::UnwrapRational<std::nano>;

    /// \brief "Micro" IS ratio.
    using Micro = Details::UnwrapRational<std::micro>;

    /// \brief "Milli" IS ratio.
    using Milli = Details::UnwrapRational<std::milli>;

    /// \brief "Centi" IS ratio.
    using Centi = Details::UnwrapRational<std::centi>;

    /// \brief "Deci" IS ratio.
    using Deci = Details::UnwrapRational<std::deci>;

    /// \brief "Deca" IS ratio.
    using Deca = Details::UnwrapRational<std::deca>;

    /// \brief "Hecto" IS ratio.
    using Hecto = Details::UnwrapRational<std::hecto>;

    /// \brief "Kilo" IS ratio.
    using Kilo = Details::UnwrapRational<std::kilo>;

    /// \brief "Mega" IS ratio.
    using Mega = Details::UnwrapRational<std::mega>;

    /// \brief "Giga" IS ratio.
    using Giga = Details::UnwrapRational<std::giga>;

    /// \brief "Tera" IS ratio.
    using Tera = Details::UnwrapRational<std::tera>;

    /// \brief "Peta" IS ratio.
    using Peta = Details::UnwrapRational<std::peta>;

    /// \brief "Exa" IS ratio.
    using Exa = Details::UnwrapRational<std::exa>;

}