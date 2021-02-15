
/// \file compare.details.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains implementation details of compare.h.
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

namespace Syntropy::Concepts::Details
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    // Three-way comparable.
    // =====================

    /// \brief Models a totally-ordered class TType whose comparison results
    ///        against the (possibly different) type UType are consistent with
    ///        a ordering category implied by TOrdering.
    template <typename TType, typename UType>
    concept ThreeWayComparableWith
        = TotallyOrderedWith<TType, UType>
       && requires(Templates::ImmutableOf<TType> lhs,
                   Templates::ImmutableOf<UType> rhs)
    {
       /// \brief Compare lhs with rhs.
       { lhs <=> rhs } -> IsSame<Ordering>;

       /// \brief Compare rhs with lhs.
       { rhs <=> lhs } -> IsSame<Ordering>;
    };
}

// ===========================================================================

#include "compare.details.inl"

// ===========================================================================
