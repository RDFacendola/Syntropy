
/// \file compare_details.h
/// \brief This header is part of the Syntropy language module. It contains implementation details for comparisons.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/concepts/concepts.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    class StrongOrdering;
    class WeakOrdering;
    class PartialOrdering;
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

        /// \brief Equal-to result.
        kEqual = 0,

        /// \brief Equivalent-to result.
        kEquivalent = kEqual,

        /// \brief Greater-than result.
        kGreater = +1,

        /// \brief Incomparable-with result.
        kIncomparable = -128,
    };

    /************************************************************************/
    /* CONVERSIONS                                                          */
    /************************************************************************/

    /// \brief Convert a std::strong_ordering value to a ComparisonResult.
    constexpr ComparisonResult ToComparisonResult(Reference<std::strong_ordering>) noexcept;

    /// \brief Convert a std::strong_ordering value to a ComparisonResult.
    constexpr ComparisonResult ToComparisonResult(Reference<std::weak_ordering>) noexcept;

    /// \brief Convert a std::strong_ordering value to a ComparisonResult.
    constexpr ComparisonResult ToComparisonResult(Reference<std::partial_ordering>) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* TRAITS                                                               */
    /************************************************************************/

    /// \brief Constant boolean equal to true if at least one in TOrderings is a StrongOrdering.
    template <typename... TOrderings>
    constexpr Bool IsStrongOrdering = (Templates::IsSame<TOrderings, StrongOrdering> | ...);

    /// \brief Constant boolean equal to true if at least one in TOrderings is a WeakOrdering.
    template <typename... TOrderings>
    constexpr Bool IsWeakOrdering = (Templates::IsSame<TOrderings, WeakOrdering> | ...);

    /// \brief Constant boolean equal to true if at least one in TOrderings is a PartialOrdering.
    template <typename... TOrderings>
    constexpr Bool IsPartialOrdering = (Templates::IsSame<TOrderings, PartialOrdering> | ...);

    // \brief Type equal to the strongest comparison category to which all TOrderings can be converted to.
    template <typename... TOrderings>
    using CommonOrdering = Templates::Conditional<IsStrongOrdering<TOrderings...>, StrongOrdering,
        Templates::Conditional<IsWeakOrdering<TOrderings...>, WeakOrdering,
        Templates::Conditional<IsPartialOrdering<TOrderings...>, PartialOrdering, void>>>;
}

// ===========================================================================

namespace Syntropy::Concepts::Details
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    /// \brief CompareAs.
    // =================

    /// \brief Models an ordering category TOrdering which is at least as strong as UOrdering.
    template <typename TOrdering, typename UOrdering>
    concept CompareAs = SameAs<Templates::Details::CommonOrdering<TOrdering, UOrdering>, UOrdering>;

    // EqualityComparable.
    // ===================

    /// \brief Models a type TType for which the equality and inequality operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWithHelper = requires(const Templates::RemoveReference<TType>& lhs, const Templates::RemoveReference<UType>& rhs)
    {
        /// \brief Compare lhs and rhs for equality.
        { lhs == rhs } -> Boolean;

        /// \brief Compare lhs and rhs for inequality.
        { lhs != rhs } -> Boolean;

        /// \brief Compare rhs and lhs for equality.
        { rhs == lhs } -> Boolean;

        /// \brief Compare rhs and lhs for inequality.
        { rhs != lhs } -> Boolean;
    };

    /// \brief Models a type TType for which the equality and inequality operators are defined.
    template <typename TType>
    concept EqualityComparable = EqualityComparableWithHelper<TType, TType>;

    /// \brief Models a type TType for which the equality and inequality operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWith = EqualityComparable<TType>
        && EqualityComparable<UType>
        && CommonReferenceWith<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>
        && EqualityComparable<Templates::CommonReference<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>>
        && EqualityComparableWithHelper<TType, UType>;

    // PartiallyOrdered.
    // =================

    /// \brief Models a type TType for which the less-than, greater-than, less-than-or-equal-to and greater-than-or-equal-to operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWithHelper = requires(const Templates::RemoveReference<TType> & lhs, const Templates::RemoveReference<UType> & rhs)
    {
        /// \brief Check whether lhs is less-than rhs.
        { lhs < rhs } -> Boolean;

        /// \brief Check whether lhs is greater-than rhs.
        { lhs > rhs } -> Boolean;

        /// \brief Check whether lhs is less-than or equal-to rhs.
        { lhs <= lhs } -> Boolean;

        /// \brief Check whether lhs is greater-than or equal-to rhs.
        { lhs >= lhs } -> Boolean;

        /// \brief Check whether rhs is less-than lhs.
        { rhs < lhs } -> Boolean;

        /// \brief Check whether rhs is greater-than lhs.
        { rhs > lhs } -> Boolean;

        /// \brief Check whether rhs is less-than or equal-to lhs.
        { rhs <= lhs } -> Boolean;

        /// \brief Check whether rhs is greater-than or equal-to lhs.
        { rhs >= lhs } -> Boolean;
    };

    /// \brief Models a type TType for which the less-than, greater-than, less-than-or-equal-to and greater-than-or-equal-to operators are defined.
    template <typename TType>
    concept PartiallyOrdered = PartiallyOrderedWithHelper<TType, TType>;

    /// \brief Models a type TType for which the less-than, greater-than, less-than-or-equal-to and greater-than-or-equal-to operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWith = PartiallyOrdered<TType>
        && PartiallyOrdered<UType>
        && CommonReferenceWith<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>
        && PartiallyOrdered<Templates::CommonReference<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>>
        && PartiallyOrderedWithHelper<TType, UType>;

    // TotallyOrdered.
    // ===============

    /// \brief Models a class TType which is both equality-comparable and partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWithHelper = EqualityComparableWith<TType, UType> && PartiallyOrderedWith<TType, UType>;

    /// \brief Models a class TType which is both equality-comparable and partially-ordered.
    template <typename TType>
    concept TotallyOrdered = TotallyOrderedWithHelper<TType, TType>;

    /// \brief Models a class TType which is both equality-comparable and partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWith = TotallyOrdered<TType>
        && TotallyOrdered<UType>
        && CommonReferenceWith<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>
        && TotallyOrdered<Templates::CommonReference<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>>
        && TotallyOrderedWithHelper<TType, UType>;

    // Three-way comparable.
    // =====================

    /// \brief Models a class TType which is both equality-comparable and partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType, typename TOrdering>
    concept ThreeWayComparableWithHelper = TotallyOrderedWith<TType, UType>
        && requires(const Templates::RemoveReference<TType> & lhs, const Templates::RemoveReference<UType> & rhs)
    {
        /// \brief Compare lhs with rhs.
        { lhs <=> rhs } -> CompareAs<TOrdering>;

        /// \brief Compare rhs with lhs.
        { rhs <=> lhs } -> CompareAs<TOrdering>;
    };

    /// \brief Models a totally-ordered class TType whose comparison results are consistent with a ordering category implied by TOrdering.
    template <typename TType, typename TOrdering>
    concept ThreeWayComparable = ThreeWayComparableWithHelper<TType, TType, TOrdering>;

    /// \brief Models a totally-ordered class TType whose comparison results against the (possibly different) type UType are consistent with a ordering category implied by TOrdering.
    template <typename TType, typename UType, typename TOrdering>
    concept ThreeWayComparableWith = ThreeWayComparable<TType, TOrdering>
        && ThreeWayComparable<UType, TOrdering>
        && CommonReferenceWith<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>
        && ThreeWayComparable<Templates::CommonReference<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>, TOrdering>
        && ThreeWayComparableWithHelper<TType, UType, TOrdering>;

}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Conversions.
    // ============

    constexpr ComparisonResult ToComparisonResult(Reference<std::strong_ordering> rhs) noexcept
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

    constexpr ComparisonResult ToComparisonResult(Reference<std::weak_ordering> rhs) noexcept
    {
        if (rhs == std::weak_ordering::equivalent)
        {
            return Details::ComparisonResult::kEquivalent;
        }

        if (rhs == std::weak_ordering::greater)
        {
            return Details::ComparisonResult::kGreater;
        }

        /*if (rhs == std::weak_ordering::less)*/
        {
            return  Details::ComparisonResult::kLess;
        }
    }

    constexpr ComparisonResult ToComparisonResult(Reference<std::partial_ordering> rhs) noexcept
    {
        if (rhs == std::partial_ordering::equivalent)
        {
            return Details::ComparisonResult::kEquivalent;
        }

        if (rhs == std::partial_ordering::greater)
        {
            return Details::ComparisonResult::kGreater;
        }

        if (rhs == std::partial_ordering::less)
        {
            return Details::ComparisonResult::kLess;
        }

        /*if (rhs == std::partial_ordering::unordered)*/
        {
            return Details::ComparisonResult::kIncomparable;
        }
    }
}