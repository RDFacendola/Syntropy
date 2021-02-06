
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

    /// \brief Models a class TType which is both equality-comparable and
    ///        partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept ThreeWayComparableWithHelper
         = TotallyOrderedWith<TType, UType>
        && requires(Immutable<Templates::RemoveReference<TType>> lhs,
                    Immutable<Templates::RemoveReference<UType>> rhs)
    {
        /// \brief Compare lhs with rhs.
        { lhs <=> rhs } -> SameAs<Ordering>;

        /// \brief Compare rhs with lhs.
        { rhs <=> lhs } -> SameAs<Ordering>;
    };

    /// \brief Models a totally-ordered class TType whose comparison results
    ///        are consistent with a ordering category implied by TOrdering.
    template <typename TType>
    concept ThreeWayComparable = ThreeWayComparableWithHelper<TType, TType>;

    /// \brief Models a totally-ordered class TType whose comparison results
    ///        against the (possibly different) type UType are consistent with
    ///        a ordering category implied by TOrdering.
    template <typename TType, typename UType>
    concept ThreeWayComparableWith
         = ThreeWayComparable<TType>
        && ThreeWayComparable<UType>
        && CommonReferenceWith<Immutable<Templates::RemoveReference<TType>>,
                               Immutable<Templates::RemoveReference<UType>>>
        && ThreeWayComparable<Templates::CommonReference<
               Immutable<Templates::RemoveReference<TType>>,
               Immutable<Templates::RemoveReference<UType>>>>
        && ThreeWayComparableWithHelper<TType, UType>;

}

// ===========================================================================

#include "compare.details.inl"

// ===========================================================================
