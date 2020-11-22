
/// \file compare_details.h
/// \brief This header is part of the Syntropy language module. It contains implementation details for comparisons.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* COMPARE RESULT                                                       */
    /************************************************************************/

    /// \brief Comparison result.
    enum class ComparisonResult : Enum8
    {
        /// \brief Less-than result.
        kLess = -1,

        /// \brief Equal-to result.
        kEqual = 0,

        /// \brief Equivalent-to result.
        kEquivalent = kEqual,

        /// \brief Greater-than result.
        kGreater = +1,

        /// \brief Incomparable-with result.
        kIncomparable = -128,
    };
}

// ===========================================================================