
/// \file rational.h
/// \brief This header is part of Syntropy language module. It contains implementation details about rational numbers template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>
#include <ratio>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/math.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RATIONAL                                                             */
    /************************************************************************/

    template <Int VNumerator, Int VDenominator>
    struct Rational;
}

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS STD RATIO                                                         */
    /************************************************************************/

    /// \brief Constant equal to true if TRational is a std::ratio< type, equal to false otherwise.
    template <typename TRational>
    constexpr bool IsStdRatio = false;

    /// \brief Specialization for std::ratio.
    template <Int VNumerator, Int VDenominator>
    constexpr bool IsStdRatio<std::ratio<VNumerator, VDenominator>> = true;

    /************************************************************************/
    /* WRAP \ UNWRAP RATIONAL                                               */
    /************************************************************************/

    /// \brief Exposes a member type Type equal to the std::ratio type represented by TRational.
    template <typename TRational>
    struct RationalWrapper
    {
        static_assert(IsRational<TRational>, "TRational must be a rational type.");

        using Type = std::ratio<TRational::kNumerator, TRational::kDenominator>;
    };

    /// \brief Exposes a member type Type equal to the Rational type represented by TRatio.
    template <typename TRatio>
    struct RationalUnwrapper
    {
        static_assert(IsStdRatio<TRatio>, "TRatio must be a std::ratio type.");

        using Type = Rational<TRatio::num, TRatio::den>;
    };

    /// \brief Wraps a rational number type to standard's std::ratio.
    template <typename TRational>
    using WrapRational = typename RationalWrapper<TRational>::Type;

    /// \brief Unwrap a rational number type from standard's std::ratio.
    template <typename TRatio>
    using UnwrapRational = typename RationalUnwrapper<TRatio>::Type;

    /************************************************************************/
    /* COMMON RATIONAL                                                      */
    /************************************************************************/

    /// \brief Exposes a member type Type equal to the common rational type that can convert both T0Ratio and T1Ratio without loss.
    /// \author Raffaele D. Facendola - September 2020.
    template <typename T0Rational, typename T1Rational>
    struct CommonRational
    {
        static_assert(IsRational<T0Rational>, "T0Rational must be a rational type.");
        static_assert(IsRational<T1Rational>, "T1Rational must be a rational type.");

    private:

        static constexpr Int kNumerator = Templates::GCD<T0Rational::kNumerator, T1Rational::kNumerator>;

        static constexpr Int kDenominator = Templates::LCM<T0Rational::kDenominator, T1Rational::kDenominator>;

    public:

        using TType = Rational<kNumerator, kDenominator>;

    };

}