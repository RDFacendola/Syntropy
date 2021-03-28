
/// \file ratio.h
///
/// \brief This header is part of Syntropy language module.
///        It contains rational numbers (ratios) template machinery.
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

#include "ratio.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /// \brief Templates for ratio types.
    template <typename TType>
    concept RatioType = Details::RatioType<TType>;
}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RATIO                                                                */
    /************************************************************************/

    /// \brief A reduced rational number of the form Numerator / Denominator.
    template <Int VNumerator, Int VDenominator = 1>
    struct Ratio
    {
        /// \brief Ratio numerator.
        static constexpr Int kNumerator
            = Details::ReducedRatioNumerator<VNumerator, VDenominator>;

        /// \brief Ratio denominator.
        static constexpr Int kDenominator
            = Details::ReducedRatioDenominator<VNumerator, VDenominator>;
    };

    /// \brief Common type two ratios can be losslessy-converted to.
    template <typename TRatio, typename URatio>
    using CommonRatio
        = Ratio<Details::CommonRatioNumerator<TRatio, URatio>,
                Details::CommondRatioDenominator<TRatio, URatio>>;

    /************************************************************************/
    /* RATIO ARITHMETIC                                                     */
    /************************************************************************/

    /// \brief Alias type for the sum of two ratios.
    template <typename TRatio, typename URatio>
    using RatioAdd
        = Ratio<Details::AddRatioNumerator<TRatio, URatio>,
                Details::AddRatioDenominator<TRatio, URatio>>;

    /// \brief Alias type for the difference of two ratios.
    template <typename TRatio, typename URatio>
    using RatioSubtract
        = Ratio<Details::SubtractRatioNumerator<TRatio, URatio>,
                Details::SubtractRatioDenominator<TRatio, URatio>>;

    /// \brief Alias type for the product of two ratios.
    template <typename TRatio, typename URatio>
    using RatioMultiply
        = Ratio<Details::MultiplyRatioNumerator<TRatio, URatio>,
                Details::MultiplyRatioDenominator<TRatio, URatio>>;

    /// \brief Alias type for the quotient of two ratios.
    template <typename TRatio, typename URatio>
    using RatioDivide
        = Ratio<Details::DivideRatioNumerator<TRatio, URatio>,
                Details::DivideRatioDenominator<TRatio, URatio>>;

    /************************************************************************/
    /* RATIO COMPARISON                                                     */
    /************************************************************************/

    /// \brief Boolean constant equal to true if TRatio and URatio represent
    ///        the same amount, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioEqual = Details::RatioEqual<TRatio, URatio>;

    /// \brief Boolean constant equal to true if TRatio and URatio don't
    ///        represent the same amount, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioNotEqual
        = Details::RatioNotEqual<TRatio, URatio>;

    /// \brief Boolean constant equal to true if TRatio represents an amount
    ///        smaller than URatio, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioLess = Details::RatioLess<TRatio, URatio>;

    /// \brief Boolean constant equal to true if TRatio represents an amount
    ///        smaller-than or equal-to URatio, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioLessEqual
        = Details::RatioLessEqual<TRatio, URatio>;

    /// \brief Boolean constant equal to true if TRatio represents an amount
    ///        greater than URatio, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioGreater = Details::RatioGreater<TRatio, URatio>;

    /// \brief Boolean constant equal to true if TRatio represents an amount
    ///        greater-than or equal-to URatio, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioGreaterEqual
        = Details::RatioGreaterEqual<TRatio, URatio>;

    /************************************************************************/
    /* RATIO UNITS                                                          */
    /************************************************************************/

    /// \brief "Nano" IS ratio.
    using Nano = Ratio<1, 1000000000>;

    /// \brief "Micro" IS ratio.
    using Micro = Ratio<1, 1000000>;

    /// \brief "Milli" IS ratio.
    using Milli = Ratio<1, 1000>;

    /// \brief "Centi" IS ratio.
    using Centi = Ratio<1, 100>;

    /// \brief "Deci" IS ratio.
    using Deci = Ratio<1, 10>;

    /// \brief "Deca" IS ratio.
    using Deca = Ratio<10, 1>;

    /// \brief "Hecto" IS ratio.
    using Hecto = Ratio<100, 1>;

    /// \brief "Kilo" IS ratio.
    using Kilo = Ratio<1000, 1>;

    /// \brief "Mega" IS ratio.
    using Mega = Ratio<1000000, 1>;

    /// \brief "Giga" IS ratio.
    using Giga = Ratio<1000000000, 1>;

    /// \brief "Tera" IS ratio.
    using Tera = Ratio<1000000000000LL, 1>;

    /// \brief "Peta" IS ratio.
    using Peta = Ratio<1000000000000000LL, 1>;

    /// \brief "Exa" IS ratio.
    using Exa = Ratio<1000000000000000000LL, 1>;

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
