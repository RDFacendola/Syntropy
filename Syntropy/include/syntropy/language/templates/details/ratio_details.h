
/// \file ratio_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details about rational numbers template machinery.
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include <ratio>

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/math.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <Int VNumerator, Int VDenominator>
    struct Ratio;
}
// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* RATIO HELPER                                                         */
    /************************************************************************/

    /// \brief Ratio helper type that wraps STL templates.
    template <typename TRatio>
    using RatioHelper = std::ratio<TRatio::kNumerator, TRatio::kDenominator>;

    /************************************************************************/
    /* REDUCED RATIO NUMERATOR \ DENOMINATOR                                */
    /************************************************************************/

    /// \brief Reduced ratio numerator.
    template <Int VNumerator, Int VDenominator>
    inline constexpr Int ReducedRatioNumerator = std::ratio<VNumerator, VDenominator>::num;

    /// \brief Reduced ratio denominator.
    template <Int VNumerator, Int VDenominator>
    inline constexpr Int ReducedRatioDenominator = std::ratio<VNumerator, VDenominator>::den;

    /************************************************************************/
    /* IS RATIO                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a specialization of Ratio, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsRatio = false;

    /// \brief Specialization for Ratio<>.
    template <Int VNumerator, Int VDenominator>
    inline constexpr Bool IsRatio<Templates::Ratio<VNumerator, VDenominator>> = true;

    /************************************************************************/
    /* COMMON RATIO                                                         */
    /************************************************************************/

    /// \brief Common ratio numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int CommonRatioNumerator = Templates::GCD<TRatio::kNumerator, URatio::kNumerator>;

    /// \brief Common ratio denominator.
    template <typename TRatio, typename URatio>
    inline constexpr Int CommondRatioDenominator = Templates::LCM<TRatio::kDenominator, URatio::kDenominator>;

    /************************************************************************/
    /* RATIO ARITHMETIC                                                     */
    /************************************************************************/

    /// \brief Ratio sum numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int AddRatioNumerator = std::ratio_add<RatioHelper<TRatio>, RatioHelper<URatio>>::num;

    /// \brief Ratio sum numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int AddRatioDenominator = std::ratio_add<RatioHelper<TRatio>, RatioHelper<URatio>>::den;

    /// \brief Ratio difference numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int SubtractRatioNumerator = std::ratio_subtract<RatioHelper<TRatio>, RatioHelper<URatio>>::num;

    /// \brief Ratio difference numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int SubtractRatioDenominator = std::ratio_subtract<RatioHelper<TRatio>, RatioHelper<URatio>>::den;

    /// \brief Ratio product numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int MultiplyRatioNumerator = std::ratio_multiply<RatioHelper<TRatio>, RatioHelper<URatio>>::num;

    /// \brief Ratio product numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int MultiplyRatioDenominator = std::ratio_multiply<RatioHelper<TRatio>, RatioHelper<URatio>>::den;

    /// \brief Ratio quotient numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int DivideRatioNumerator = std::ratio_divide<RatioHelper<TRatio>, RatioHelper<URatio>>::num;

    /// \brief Ratio quotient numerator.
    template <typename TRatio, typename URatio>
    inline constexpr Int DivideRatioDenominator = std::ratio_divide<RatioHelper<TRatio>, RatioHelper<URatio>>::den;

    /************************************************************************/
    /* RATIO COMPARISON                                                     */
    /************************************************************************/

    /// \brief Boolean constant equal to true if TRatio and URatio represent the same amount, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioEqual = std::ratio_equal_v<RatioHelper<TRatio>, RatioHelper<URatio>>;

    /// \brief Boolean constant equal to true if TRatio and URatio don't represent the same amount, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioNotEqual = std::ratio_not_equal_v<RatioHelper<TRatio>, RatioHelper<URatio>>;

    /// \brief Boolean constant equal to true if TRatio represents an amount smaller than URatio, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioLess = std::ratio_less_v<RatioHelper<TRatio>, RatioHelper<URatio>>;

    /// \brief Boolean constant equal to true if TRatio represents an amount smaller-than or equal-to URatio, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioLessEqual = std::ratio_less_equal_v<RatioHelper<TRatio>, RatioHelper<URatio>>;

    /// \brief Boolean constant equal to true if TRatio represents an amount greater than URatio, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioGreater = std::ratio_greater_v<RatioHelper<TRatio>, RatioHelper<URatio>>;

    /// \brief Boolean constant equal to true if TRatio represents an amount greater-than or equal-to URatio, equal to false otherwise.
    template <typename TRatio, typename URatio>
    inline constexpr Bool RatioGreaterEqual = std::ratio_greater_equal_v<RatioHelper<TRatio>, RatioHelper<URatio>>;
}

// ===========================================================================
