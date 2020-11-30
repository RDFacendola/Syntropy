
/// \file compare.h
/// \brief This header is part of the Syntropy language module. It contains definitions for comparisons.
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/support/details/compare_details.h"

#include <compare>

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* ORDERING                                                             */
    /************************************************************************/

    /// \brief Defines a strong ordering relationship between objects.
    /// In strong order relationships, equivalent elements are indistinguishable. Incomparable elements are not allowed.
    /// \author Raffaele D. Facendola - Nov 2020.
    class Ordering
    {
        friend constexpr Bool operator==(Ref<Ordering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator==(Ref<Ordering> lhs, Ref<Ordering> rhs) noexcept;
        friend constexpr Bool operator<(Ref<Ordering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator>(Ref<Ordering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator<=(Ref<Ordering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator>=(Ref<Ordering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator<(Null, Ref<Ordering> rhs) noexcept;
        friend constexpr Bool operator>(Null, Ref<Ordering> rhs) noexcept;
        friend constexpr Bool operator<=(Null, Ref<Ordering> rhs) noexcept;
        friend constexpr Bool operator>=(Null, Ref<Ordering> rhs) noexcept;
        friend constexpr Ordering operator<=>(Ref<Ordering> lhs, Null rhs) noexcept;
        friend constexpr Ordering operator<=>(Null lhs, Ref<Ordering> rhs) noexcept;

    public:

        /// \brief Indicates that an element is less-than another element.
        static const Ordering Less;

        /// \brief Indicates that an Ordering is equivalent-to another element.
        static const Ordering Equivalent;

        /// \brief Indicates that an element is greater-than another element.
        static const Ordering Greater;

        /// \brief Convert from std::strong_ordering to Ordering.
        /// \remarks Allows better interoperability with STL.
        constexpr Ordering(Ref<std::strong_ordering> value) noexcept;

    private:

        /// \brief Create a new weak ordering result value.
        constexpr explicit Ordering(Ref<Details::ComparisonResult> value) noexcept
            : value_(value)
        {
            // @rfacendola Apparently in VS2019 using the explicit keyword requires both declaration and definition at the same time, otherwise it will ignore that and zero-initialize the instance.
        }

        /// \brief Underlying compare result.
        Details::ComparisonResult value_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether rhs represents equal-to compare result.
    constexpr Bool IsEqual(Ref<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents not-equal-to compare result.
    constexpr Bool IsNotEqual(Ref<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents less-than compare result.
    constexpr Bool IsLessThan(Ref<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents less-than or equal-to compare result.
    constexpr Bool IsLessEqualTo(Ref<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents greater-than compare result.
    constexpr Bool IsGreaterThan(Ref<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents greater-than or equal-to compare result.
    constexpr Bool IsGreaterEqualTo(Ref<Ordering> rhs) noexcept;

    /************************************************************************/
    /* COMPARISON OPERATORS                                                 */
    /************************************************************************/

    // Ordering.
    // =========

    /// \brief Compare lhs to the null literal for equality-comparison.
    constexpr Bool operator==(Ref<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to rhs for equality-comparison.
    constexpr Bool operator==(Ref<Ordering> lhs, Ref<Ordering> rhs) noexcept;

    /// \brief Compare lhs to the null literal for less-than comparison.
    constexpr Bool operator<(Ref<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than comparison.
    constexpr Bool operator>(Ref<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for less-than or equal-to comparison.
    constexpr Bool operator<=(Ref<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than or equal-to comparison.
    constexpr Bool operator>=(Ref<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than comparison.
    constexpr Bool operator<(Null lhs, Ref<Ordering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than comparison.
    constexpr Bool operator>(Null lhs, Ref<Ordering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than or equal-to comparison.
    constexpr Bool operator<=(Null lhs, Ref<Ordering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than or equal-to comparison.
    constexpr Bool operator>=(Null lhs, Ref<Ordering> rhs) noexcept;

    /// \brief Three-way-comparison of lhs against the null literal.
    constexpr Ordering operator<=>(Ref<Ordering> lhs, Null rhs) noexcept;

    /// \brief Three-way-comparison of the null literal against rhs.
    constexpr Ordering operator<=>(Null lhs, Ref<Ordering> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    /// \brief Models a totally-ordered class TType whose comparison results are consistent with a ordering category implied by TOrdering.
    template <typename TType>
    concept ThreeWayComparable = Details::ThreeWayComparable<TType>;

    /// \brief Models a totally-ordered class TType whose comparison results against the (possibly different) type UType are consistent with a ordering category implied by TOrdering.
    template <typename TType, typename UType>
    concept ThreeWayComparableWith = Details::ThreeWayComparableWith<TType, UType>;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Ordering.
    // =========

    inline constexpr Ordering Ordering::Less{ Details::ComparisonResult::kLess };
    inline constexpr Ordering Ordering::Equivalent{ Details::ComparisonResult::kEquivalent };
    inline constexpr Ordering Ordering::Greater{ Details::ComparisonResult::kGreater };

    constexpr Ordering::Ordering(Ref<std::strong_ordering> value) noexcept
        : value_(Details::ToComparisonResult(value))
    {

    }

    // Non-member functions.
    // =====================

    constexpr Bool IsEqual(Ref<Ordering> rhs) noexcept
    {
        return rhs == 0;
    }

    constexpr Bool IsNotEqual(Ref<Ordering> rhs) noexcept
    {
        return rhs != 0;
    }

    constexpr Bool IsLessThan(Ref<Ordering> rhs) noexcept
    {
        return rhs < 0;
    }

    constexpr Bool IsLessEqualTo(Ref<Ordering> rhs) noexcept
    {
        return rhs <= 0;
    }

    constexpr Bool IsGreaterThan(Ref<Ordering> rhs) noexcept
    {
        return rhs > 0;
    }

    constexpr Bool IsGreaterEqualTo(Ref<Ordering> rhs) noexcept
    {
        return rhs >= 0;
    }

    // Ordering.

    constexpr Bool operator==(Ref<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator==(Ref<Ordering> lhs, Ref<Ordering> rhs) noexcept
    {
        return (lhs.value_ == rhs.value_);
    }

    constexpr Bool operator<(Ref<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess);
    }

    constexpr Bool operator>(Ref<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater);
    }

    constexpr Bool operator<=(Ref<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator>=(Ref<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator<(Null lhs, Ref<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater);
    }

    constexpr Bool operator>(Null lhs, Ref<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess);
    }

    constexpr Bool operator<=(Null lhs, Ref<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater) || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator>=(Null lhs, Ref<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess) || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Ordering operator<=>(Ref<Ordering> lhs, Null rhs) noexcept
    {
        return lhs;
    }

    constexpr Ordering operator<=>(Null lhs, Ref<Ordering> rhs) noexcept
    {
        if (rhs.value_ == Details::ComparisonResult::kLess)
        {
            return Ordering{ Details::ComparisonResult::kGreater };
        }

        if (rhs.value_ == Details::ComparisonResult::kGreater)
        {
            return Ordering{ Details::ComparisonResult::kLess };
        }

        return rhs;
    }

}

// ===========================================================================