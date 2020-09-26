
/// \file ratio.h
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
    /* RATIO                                                                */
    /************************************************************************/

    template <Int VNumerator, Int VDenominator>
    struct Ratio;
}

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS RATIO                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TRatio is a Ratio<N,D> type, equal to false otherwise.
    template <typename TRatio>
    constexpr bool IsRatio = false;

    /// \brief Specialization for rational types.
    template <Int VNumerator, Int VDenominator>
    constexpr bool IsRatio<Ratio<VNumerator, VDenominator>> = true;

    /************************************************************************/
    /* IS STL RATIO                                                         */
    /************************************************************************/

    /// \brief Constant equal to true if TRatio is a std::ratio<> type, equal to false otherwise.
    template <typename TRatio>
    constexpr bool IsSTLRatio = false;

    /// \brief Specialization for std::ratio.
    template <Int VNumerator, Int VDenominator>
    constexpr bool IsSTLRatio<std::ratio<VNumerator, VDenominator>> = true;

    /************************************************************************/
    /* RATIO WRAPPER / UNWRAPPER                                            */
    /************************************************************************/

    /// \brief Exposes a member type Type equal to the std::ratio type represented by TRatio.
    template <typename TRatio>
    struct RatioWrapper
    {
        static_assert(IsRatio<TRatio>, "TRatio must be a Ratio<> type.");

        using Type = std::ratio<TRatio::kNumerator, TRatio::kDenominator>;
    };

    /// \brief Exposes a member type Type equal to the Ratio type represented by TRatio.
    template <typename TRatio>
    struct RatioUnwrapper
    {
        static_assert(IsSTLRatio<TRatio>, "TRatio must be a std::ratio<> type.");

        using Type = Ratio<TRatio::num, TRatio::den>;
    };

    /// \brief Wraps a rational number type to standard's std::ratio.
    template <typename TRatio>
    using WrapRatio = typename RatioWrapper<TRatio>::Type;

    /// \brief Unwrap a rational number type from standard's std::ratio.
    template <typename TRatio>
    using UnwrapRatio = typename RatioUnwrapper<TRatio>::Type;

    /************************************************************************/
    /* COMMON RATIO                                                         */
    /************************************************************************/

    /// \brief Exposes a member type Type equal to the common ratio type that can convert both T0Ratio and T1Ratio without loss.
    /// \author Raffaele D. Facendola - September 2020.
    template <typename T0Ratio, typename T1Ratio>
    struct CommonRatio
    {
        static_assert(IsRatio<T0Ratio>, "T0Ratio must be a Ratio<> type.");
        static_assert(IsRatio<T1Ratio>, "T1Ratio must be a Ratio<> type.");

    private:

        static constexpr Int kNumerator = Templates::GCD<T0Ratio::kNumerator, T1Ratio::kNumerator>;

        static constexpr Int kDenominator = Templates::LCM<T0Ratio::kDenominator, T1Ratio::kDenominator>;

    public:

        using Type = Ratio<kNumerator, kDenominator>;

    };

}