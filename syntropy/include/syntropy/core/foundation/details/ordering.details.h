
/// \file ordering.details.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains implementation details of ordering.h.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/concepts.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    class Ordering;
}

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

        /// \brief Equivalent-to result.
        kEquivalent = 0,

        /// \brief Greater-than result.
        kGreater = +1,
    };

    /************************************************************************/
    /* CONVERSIONS                                                          */
    /************************************************************************/

    /// \brief Convert a std::strong_ordering value to a ComparisonResult.
    [[nodiscard]] constexpr ComparisonResult
    ToComparisonResult(Immutable<std::strong_ordering> rhs) noexcept;

}

// ===========================================================================

#include "ordering.details.inl"

// ===========================================================================
