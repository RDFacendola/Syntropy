
/// \file ratio.h
/// \brief This header is part of Syntropy language module. It contains rational numbers (ratios) template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>
#include <ratio>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/math.h"
#include "syntropy/language/templates/manipulation.h"
#include "syntropy/language/templates/details/ratio_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RATIO                                                                */
    /************************************************************************/

    /// \brief A reduced rational number of the form Numerator / Denominator.
    template <Int VNumerator, Int VDenominator>
    struct Ratio
    {
        /// \brief Ratio numerator.
        static constexpr Int kNumerator = std::ratio<VNumerator, VDenominator>::num;

        /// \brief Ratio denominator.
        static constexpr Int kDenominator = std::ratio<VNumerator, VDenominator>::den;
    };

    /************************************************************************/
    /* IS RATIO                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TRatio is a Ratio<T> type, equal to false otherwise.
    template <typename TRatio>
    constexpr bool IsRatio = Details::IsRatio<TRatio>;

    /************************************************************************/
    /* COMMON TYPE                                                          */
    /************************************************************************/

    /// \brief Common type two ratios can be losslessy-converted to.
    template <typename T0Ratio, typename T1Ratio, typename = EnableIf<Templates::IsRatio<T0Ratio>&& Templates::IsRatio<T1Ratio>>>
    using CommonRatio = typename Details::CommonRatio<T0Ratio, T1Ratio>::Type;

    /************************************************************************/
    /* RATIO ARITHMETIC                                                     */
    /************************************************************************/

    /// \brief Alias type for the sum of two ratios.
    template <typename T0Ratio, typename T1Ratio>
    using RatioSum = Details::UnwrapRatio<std::ratio_add<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>>;

    /// \brief Alias type for the difference of two ratios.
    template <typename T0Ratio, typename T1Ratio>
    using RatioDifference = Details::UnwrapRatio<std::ratio_subtract<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>>;

    /// \brief Alias type for the product of two ratios.
    template <typename T0Ratio, typename T1Ratio>
    using RatioProduct = Details::UnwrapRatio<std::ratio_multiply<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>>;

    /// \brief Alias type for the quotient of two ratios.
    template <typename T0Ratio, typename T1Ratio>
    using RatioQuotient = Details::UnwrapRatio<std::ratio_divide<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>>;

    /************************************************************************/
    /* RATIO COMPARISON                                                     */
    /************************************************************************/

    /// \brief Boolean constant equal to true if T0Ratio and T1Ratio represent the same amount, equal to false otherwise.
    template <typename T0Ratio, typename T1Ratio>
    inline constexpr Bool RatioEqual = std::ratio_equal_v<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>;

    /// \brief Boolean constant equal to true if T0Ratio and T1Ratio don't represent the same amount, equal to false otherwise.
    template <typename T0Ratio, typename T1Ratio>
    inline constexpr Bool RatioNotEqual = std::ratio_not_equal_v<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>;

    /// \brief Boolean constant equal to true if T0Ratio represents an amount smaller than T1Ratio, equal to false otherwise.
    template <typename T0Ratio, typename T1Ratio>
    inline constexpr Bool RatioLess = std::ratio_less_v<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>;

    /// \brief Boolean constant equal to true if T0Ratio represents an amount smaller-than or equal-to T1Ratio, equal to false otherwise.
    template <typename T0Ratio, typename T1Ratio>
    inline constexpr Bool RatioLessEqual = std::ratio_less_equal_v<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>;

    /// \brief Boolean constant equal to true if T0Ratio represents an amount greater than T1Ratio, equal to false otherwise.
    template <typename T0Ratio, typename T1Ratio>
    inline constexpr Bool RatioGreater = std::ratio_greater_v<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>;

    /// \brief Boolean constant equal to true if T0Ratio represents an amount greater-than or equal-to T1Ratio, equal to false otherwise.
    template <typename T0Ratio, typename T1Ratio>
    inline constexpr Bool RatioGreaterEqual = std::ratio_greater_equal_v<Details::WrapRatio<T0Ratio>, Details::WrapRatio<T1Ratio>>;

    /************************************************************************/
    /* RATIO UNITS                                                          */
    /************************************************************************/

    /// \brief "Nano" IS ratio.
    using Nano = Details::UnwrapRatio<std::nano>;

    /// \brief "Micro" IS ratio.
    using Micro = Details::UnwrapRatio<std::micro>;

    /// \brief "Milli" IS ratio.
    using Milli = Details::UnwrapRatio<std::milli>;

    /// \brief "Centi" IS ratio.
    using Centi = Details::UnwrapRatio<std::centi>;

    /// \brief "Deci" IS ratio.
    using Deci = Details::UnwrapRatio<std::deci>;

    /// \brief "Deca" IS ratio.
    using Deca = Details::UnwrapRatio<std::deca>;

    /// \brief "Hecto" IS ratio.
    using Hecto = Details::UnwrapRatio<std::hecto>;

    /// \brief "Kilo" IS ratio.
    using Kilo = Details::UnwrapRatio<std::kilo>;

    /// \brief "Mega" IS ratio.
    using Mega = Details::UnwrapRatio<std::mega>;

    /// \brief "Giga" IS ratio.
    using Giga = Details::UnwrapRatio<std::giga>;

    /// \brief "Tera" IS ratio.
    using Tera = Details::UnwrapRatio<std::tera>;

    /// \brief "Peta" IS ratio.
    using Peta = Details::UnwrapRatio<std::peta>;

    /// \brief "Exa" IS ratio.
    using Exa = Details::UnwrapRatio<std::exa>;

    /// \brief "Kibi" binary ratio.
    using Kibi = Ratio<0x400LL, 1>;

    /// \brief "Mebi" binary ratio.
    using Mebi = Ratio<0x100000LL, 1>;

    /// \brief "Gibi" binary ratio.
    using Gibi = Ratio<0x40000000LL, 1>;

    /// \brief "Tebi" binary ratio.
    using Tebi = Ratio<0x10000000000LL, 1>;

}

// ===========================================================================