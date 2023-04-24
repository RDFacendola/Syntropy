
/// \file ratio.h
///
/// \brief This header is part of Syntropy language module.
///        It contains rational numbers (ratios) template machinery.
///
/// \author Raffaele D. Facendola - September 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

#include "details/ratio.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RATIO                                                                */
    /************************************************************************/

    // Ratio.
    // ======

    /// \brief A reduced rational number of the form Numerator / Denominator.
    template <Int TNumerator, Int TDenominator = 1>
    struct Ratio
    {
        /// \brief Ratio numerator.
        static constexpr
        Int kNumerator
            = Details::NumeratorOf<TNumerator, TDenominator>;

        /// \brief Ratio denominator.
        static constexpr
        Int kDenominator
            = Details::DenominatorOf<TNumerator, TDenominator>;
    };

    // IsRatio.
    // ========

    /// \brief Concept for ratios.
    template <typename TRatio>
    concept IsRatio
        = Details::IsRatio<TRatio, Ratio>;

    // CommonRatio.
    // ============

    /// \brief Common type two ratios can be losslessy-converted to.
    template <IsRatio TRatio, IsRatio URatio>
    using CommonRatio
        = Details::CommonRatio<TRatio, URatio>;

    /************************************************************************/
    /* RATIO ARITHMETIC                                                     */
    /************************************************************************/

    /// \brief Alias type for the sum of two ratios.
    template <IsRatio TRatio, IsRatio URatio>
    using RatioAdd
        = Details::RatioAdd<TRatio, URatio>;

    /// \brief Alias type for the difference of two ratios.
    template <IsRatio TRatio, IsRatio URatio>
    using RatioSubtract
        = Details::RatioSubtract<TRatio, URatio>;

    /// \brief Alias type for the product of two ratios.
    template <IsRatio TRatio, IsRatio URatio>
    using RatioMultiply
        = Details::RatioMultiply<TRatio, URatio>;

    /// \brief Alias type for the quotient of two ratios.
    template <IsRatio TRatio, IsRatio URatio>
    using RatioDivide
        = Details::RatioDivide<TRatio, URatio>;

    /************************************************************************/
    /* RATIO COMPARISON                                                     */
    /************************************************************************/

    /// \brief True if TRatio is equivalent to URatio, false otherwise.
    template <IsRatio TRatio, IsRatio URatio>
    inline constexpr
    Bool RatioEqual
        = Details::RatioEqual<TRatio, URatio>;

    /// \brief True if TRatio and URatio are not equal, false otherwise.
    template <IsRatio TRatio, IsRatio URatio>
    inline constexpr
    Bool RatioNotEqual
        = Details::RatioNotEqual<TRatio, URatio>;

    /// \brief True if TRatio is less than URatio, false otherwise.
    template <IsRatio TRatio, IsRatio URatio>
    inline constexpr
    Bool RatioLess
        = Details::RatioLess<TRatio, URatio>;

    /// \brief True if TRatio is less or equal to URatio, false otherwise.
    template <IsRatio TRatio, IsRatio URatio>
    inline constexpr
    Bool RatioLessEqual
        = Details::RatioLessEqual<TRatio, URatio>;

    /// \brief True if TRatio is greater than URatio, false otherwise.
    template <IsRatio TRatio, IsRatio URatio>
    inline constexpr
    Bool RatioGreater
        = Details::RatioGreater<TRatio, URatio>;

    /// \brief True if TRatio is greater or equal to URatio, false otherwise.
    template <IsRatio TRatio, IsRatio URatio>
    inline constexpr
    Bool RatioGreaterEqual
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
