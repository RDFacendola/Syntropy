
/// \file compare_details.h
/// \brief This header is part of the Syntropy language module. It contains implementation details for comparisons.
///
/// \author Raffaele D. Facendola - Nov 2020

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
    constexpr ComparisonResult ToComparisonResult(Ref<std::strong_ordering>) noexcept;

}

// ===========================================================================

namespace Syntropy::Concepts::Details
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    // Three-way comparable.
    // =====================

    /// \brief Models a class TType which is both equality-comparable and partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept ThreeWayComparableWithHelper = TotallyOrderedWith<TType, UType>
        && requires(Ref<Templates::RemoveReference<TType>> lhs, Ref<Templates::RemoveReference<UType>> rhs)
    {
        /// \brief Compare lhs with rhs.
        { lhs <=> rhs } -> SameAs<Ordering>;

        /// \brief Compare rhs with lhs.
        { rhs <=> lhs } -> SameAs<Ordering>;
    };

    /// \brief Models a totally-ordered class TType whose comparison results are consistent with a ordering category implied by TOrdering.
    template <typename TType>
    concept ThreeWayComparable = ThreeWayComparableWithHelper<TType, TType>;

    /// \brief Models a totally-ordered class TType whose comparison results against the (possibly different) type UType are consistent with a ordering category implied by TOrdering.
    template <typename TType, typename UType>
    concept ThreeWayComparableWith = ThreeWayComparable<TType>
        && ThreeWayComparable<UType>
        && CommonReferenceWith<Ref<Templates::RemoveReference<TType>>, Ref<Templates::RemoveReference<UType>>>
        && ThreeWayComparable<Templates::CommonReference<Ref<Templates::RemoveReference<TType>>, Ref<Templates::RemoveReference<UType>>>>
        && ThreeWayComparableWithHelper<TType, UType>;

}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Conversions.
    // ============

    constexpr ComparisonResult ToComparisonResult(Ref<std::strong_ordering> rhs) noexcept
    {
        if (rhs == std::strong_ordering::equivalent)
        {
            return Details::ComparisonResult::kEquivalent;
        }

        if (rhs == std::strong_ordering::greater)
        {
            return Details::ComparisonResult::kGreater;
        }
        
        /*if (rhs == std::strong_ordering::less)*/
        {
            return  Details::ComparisonResult::kLess;
        }
    }

}