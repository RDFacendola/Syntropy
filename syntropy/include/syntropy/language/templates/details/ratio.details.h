
/// \file ratio_details.h
///
/// \author Raffaele D. Facendola - September 2020.

// ===========================================================================

#pragma once

#include <ratio>

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/math.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* RATIO                                                                */
    /************************************************************************/

    // Ratio.
    // ======

    /// \brief Reduced ratio numerator.
    template <Int TNumerator, Int TDenominator>
    inline constexpr
    Int NumeratorOf
        = std::ratio<TNumerator, TDenominator>::num;

    /// \brief Reduced ratio denominator.
    template <Int TNumerator, Int TDenominator>
    inline constexpr
    Int DenominatorOf
        = std::ratio<TNumerator, TDenominator>::den;

    // IsRatio.
    // ========

    /// \brief True if TRatio is a specialization of URatio, false otherwise.
    template <typename TRatio, template <Int, Int> typename URatio>
    inline constexpr
    Bool IsRatio
        = false;

    /// \brief Specialization for ratios.
    template <template <Int, Int> typename TRatio,
              template <Int, Int> typename URatio,
              Int TNumerator,
              Int TDenominator>
    inline constexpr
    Bool IsRatio<TRatio<TNumerator, TDenominator>, URatio>
        = IsSame<TRatio<TNumerator, TDenominator>,
                 URatio<TNumerator, TDenominator>>;

    // CommonRatio.
    // ============

    /// \brief Common type two ratios can be losslessy-converted to.
    template <typename TRatio, typename URatio>
    struct CommonRatioHelper{};

    /// \brief Common type two ratios can be losslessy-converted to.
    template <template <Int, Int> typename TRatio,
              Int TNumerator,
              Int TDenominator,
              Int UNumerator,
              Int UDenominator>
    struct CommonRatioHelper<TRatio<TNumerator, TDenominator>,
                             TRatio<UNumerator, UDenominator>>
       : Alias<TRatio<GCD<TNumerator, UNumerator>,
                      LCM<TDenominator, UDenominator>>> {};

    /// \brief Common type two ratios can be losslessy-converted to.
    template <typename TRatio, typename URatio>
    using CommonRatio
        = typename CommonRatioHelper<TRatio, URatio>::Type;

    // Ratio adapters.
    // ===============

    /// \brief Convert a ratio type to std::ratio.
    template <typename TRatio>
    using ToRatio
        = std::ratio<TRatio::kNumerator, TRatio::kDenominator>;

    /// \brief Convert a std::ratio to a ratio type.
    template <typename TRatioFrom, typename TRatioTo>
    struct FromRatioHelper{};

    /// \brief Convert a std::ratio to a ratio type.
    template <typename TRatioFrom,
              template <Int, Int> typename TRatioTo,
              Int TNumerator,
              Int TDenominator>
    struct FromRatioHelper<TRatioFrom, TRatioTo<TNumerator, TDenominator>>
        : Alias<TRatioTo<TRatioFrom::num, TRatioFrom::den>> {};

    /// \brief Convert a ratio type to std::ratio.
    template <typename TRatioFrom, typename TRatioTo>
    using FromRatio
        = typename FromRatioHelper<TRatioFrom, TRatioTo>::Type;

    /************************************************************************/
    /* RATIO ARITHMETIC                                                     */
    /************************************************************************/

    /// \brief Alias for a ratio obtained by applying a function to two
    ///        ratios.
    template <template <typename, typename> typename TOperation,
              typename TRatio,
              typename URatio>
    using RatioOperation
        = FromRatio<TOperation<ToRatio<TRatio>, ToRatio<URatio>>, TRatio>;

    /// \brief Add two ratios together.
    template <typename TRatio, typename URatio>
    using RatioAdd
        = RatioOperation<std::ratio_add, TRatio, URatio>;

    /// \brief Alias type for the difference of two ratios.
    template <typename TRatio, typename URatio>
    using RatioSubtract
        = RatioOperation<std::ratio_subtract, TRatio, URatio>;

    /// \brief Alias type for the product of two ratios.
    template <typename TRatio, typename URatio>
    using RatioMultiply
        = RatioOperation<std::ratio_multiply, TRatio, URatio>;

    /// \brief Alias type for the quotient of two ratios.
    template <typename TRatio, typename URatio>
    using RatioDivide
        = RatioOperation<std::ratio_divide, TRatio, URatio>;

    /************************************************************************/
    /* RATIO COMPARISON                                                     */
    /************************************************************************/

    /// \brief True if TRatio is equivalent to URatio, false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool
    RatioEqual
        = std::ratio_equal_v<ToRatio<TRatio>, ToRatio<URatio>>;

    /// \brief True if TRatio and URatio are not equal, false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool
    RatioNotEqual
        = std::ratio_not_equal_v<ToRatio<TRatio>, ToRatio<URatio>>;

    /// \brief True if TRatio is less than URatio, false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool
    RatioLess
        = std::ratio_less_v<ToRatio<TRatio>, ToRatio<URatio>>;

    /// \brief True if TRatio is less or equal to URatio, false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool
    RatioLessEqual
        = std::ratio_less_equal_v<ToRatio<TRatio>, ToRatio<URatio>>;

    /// \brief True if TRatio is greater than URatio, false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool
    RatioGreater
        = std::ratio_greater_v<ToRatio<TRatio>, ToRatio<URatio>>;

    /// \brief True if TRatio is greater or equal to URatio, false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool
    RatioGreaterEqual
        = std::ratio_greater_equal_v<ToRatio<TRatio>, ToRatio<URatio>>;

}

// ===========================================================================
