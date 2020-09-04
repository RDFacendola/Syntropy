
/// \file rational.h
/// \brief This header is part of Syntropy language module. It contains implementation details about rational numbers template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/constants.h"
#include "syntropy/language/templates/math.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* RATIONAL ADD                                                         */
    /************************************************************************/

    /// \brief Represents the sum of two rational quantities.
    template <typename TRatio, typename URatio>
    struct RationalAdd
    {
        static_assert(AlwaysFalse<TRatio, URatio>, "Expect Ratio<N, D> type.");
    };

    /// \brief Specialization for rationals.
    template <template<Int, Int> typename TRational, Int kTNumerator, Int kTDenominator, Int kUNumerator, Int kUDenominator>
    struct RationalAdd<TRational<kTNumerator, kTDenominator>, TRational<kUNumerator, kUDenominator>> : Alias<TRational<kTNumerator + kUNumerator, kTDenominator + kUDenominator>>
    {

    };

}