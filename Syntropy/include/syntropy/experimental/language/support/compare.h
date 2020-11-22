
/// \file compare.h
/// \brief This header is part of the Syntropy language module. It contains definitions for comparisons.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/experimental/language/support/details/compare_details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    class StrongOrdering;
    class WeakOrdering;
    class PartialOrdering;

    /************************************************************************/
    /* STRONG ORDERING                                                      */
    /************************************************************************/

    /// \brief Defines a strong ordering relationship between objects.
    /// In strong order relationships equivalent elements are indistinguishable. Incomparable elements are not allowed.
    /// \author Raffaele D. Facendola - November 2020.
    class StrongOrdering
    {
        friend constexpr Bool operator==(Reference<StrongOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator==(Reference<StrongOrdering> lhs, Reference<StrongOrdering> rhs) noexcept;
        friend constexpr Bool operator<(Reference<StrongOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator>(Reference<StrongOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator<=(Reference<StrongOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator>=(Reference<StrongOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator<(Null lhs, Reference<StrongOrdering> rhs) noexcept;
        friend constexpr Bool operator>(Null lhs, Reference<StrongOrdering> rhs) noexcept;
        friend constexpr Bool operator<=(Null lhs, Reference<StrongOrdering> rhs) noexcept;
        friend constexpr Bool operator>=(Null lhs, Reference<StrongOrdering> rhs) noexcept;
        friend constexpr StrongOrdering operator<=>(Reference<StrongOrdering> lhs, Null rhs) noexcept;
        friend constexpr StrongOrdering operator<=>(Null lhs, Reference<StrongOrdering> rhs) noexcept;

    public:

        /// \brief Indicates that an element is less-than another element.
        static const StrongOrdering Less;

        /// \brief Indicates that an element is equivalent-to another element. Same as Equal.
        static const StrongOrdering Equivalent;

        /// \brief Indicates that an element is equal-to another element. Same as Equivalent.
        static const StrongOrdering Equal;

        /// \brief Indicates that an element is greater-than another element.
        static const StrongOrdering Greater;

        /// \brief Implicit conversion to weak ordering relationship.
        constexpr operator WeakOrdering() const noexcept;

        /// \brief Implicit conversion to partial ordering relationship.
        constexpr operator PartialOrdering() const noexcept;

    private:

        /// \brief Create a new strong ordering result value.
        constexpr explicit StrongOrdering(Reference<Details::ComparisonResult> value) noexcept;

        /// \brief Underlying compare result.
        Details::ComparisonResult value_;

    };

    /************************************************************************/
    /* WEAK ORDERING                                                        */
    /************************************************************************/

    /// \brief Defines a weak ordering relationship between objects.
    /// In weak order relationships, equivalent elements may be distinguishable. Incomparable elements are not allowed.
    /// \author Raffaele D. Facendola - November 2020.
    class WeakOrdering
    {
        friend class StrongOrdering;

        friend constexpr Bool operator==(Reference<WeakOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator==(Reference<WeakOrdering> lhs, Reference<WeakOrdering> rhs) noexcept;
        friend constexpr Bool operator<(Reference<WeakOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator>(Reference<WeakOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator<=(Reference<WeakOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator>=(Reference<WeakOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator<(Null, Reference<WeakOrdering> rhs) noexcept;
        friend constexpr Bool operator>(Null, Reference<WeakOrdering> rhs) noexcept;
        friend constexpr Bool operator<=(Null, Reference<WeakOrdering> rhs) noexcept;
        friend constexpr Bool operator>=(Null, Reference<WeakOrdering> rhs) noexcept;
        friend constexpr WeakOrdering operator<=>(Reference<WeakOrdering> lhs, Null rhs) noexcept;
        friend constexpr WeakOrdering operator<=>(Null lhs, Reference<WeakOrdering> rhs) noexcept;

    public:

        /// \brief Indicates that an element is less-than another element.
        static const WeakOrdering Less;

        /// \brief Indicates that an WeakOrdering is equivalent-to another element.
        static const WeakOrdering Equivalent;

        /// \brief Indicates that an element is greater-than another element.
        static const WeakOrdering Greater;

        /// \brief Implicit conversion to partial ordering relationship.
        constexpr operator PartialOrdering() const noexcept;

    private:

        /// \brief Create a new weak ordering result value.
        constexpr explicit WeakOrdering(Reference<Details::ComparisonResult> value) noexcept;

        /// \brief Underlying compare result.
        Details::ComparisonResult value_;

    };

    /************************************************************************/
    /* PARTIAL ORDERING                                                     */
    /************************************************************************/

    /// \brief Defines a partial ordering relationship between objects.
    /// In partial order relationships, equivalent elements may be distinguishable. Incomparable elements are allowed.
    /// \author Raffaele D. Facendola - November 2020.
    class PartialOrdering
    {
        friend class StrongOrdering;
        friend class WeakOrdering;

        friend constexpr Bool operator==(Reference<PartialOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator==(Reference<PartialOrdering> lhs, Reference<PartialOrdering> rhs) noexcept;
        friend constexpr Bool operator<(Reference<PartialOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator>(Reference<PartialOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator<=(Reference<PartialOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator>=(Reference<PartialOrdering> lhs, Null rhs) noexcept;
        friend constexpr Bool operator<(Null, Reference<PartialOrdering> rhs) noexcept;
        friend constexpr Bool operator>(Null, Reference<PartialOrdering> rhs) noexcept;
        friend constexpr Bool operator<=(Null, Reference<PartialOrdering> rhs) noexcept;
        friend constexpr Bool operator>=(Null, Reference<PartialOrdering> rhs) noexcept;
        friend constexpr PartialOrdering operator<=>(Reference<PartialOrdering> lhs, Null rhs) noexcept;
        friend constexpr PartialOrdering operator<=>(Null lhs, Reference<PartialOrdering> rhs) noexcept;

    public:

        /// \brief Indicates that an element is less-than another element.
        static const PartialOrdering Less;

        /// \brief Indicates that an element is equivalent-to another element.
        static const PartialOrdering Equivalent;

        /// \brief Indicates that an element is greater-than another element.
        static const PartialOrdering Greater;

        /// \brief Indicates that two elements are incomparable.
        static const PartialOrdering Unordered;

    private:

        /// \brief Create a new partial ordering result value.
        constexpr explicit PartialOrdering(Reference<Details::ComparisonResult> value) noexcept;

        /// \brief Underlying compare result.
        Details::ComparisonResult value_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether rhs represents equal-to compare result.
    constexpr Bool IsEqual(Reference<PartialOrdering> rhs) noexcept;

    /// \brief Check whether rhs represents not-equal-to compare result.
    constexpr Bool IsNotEqual(Reference<PartialOrdering> rhs) noexcept;

    /// \brief Check whether rhs represents less-than compare result.
    constexpr Bool IsLessThan(Reference<PartialOrdering> rhs) noexcept;

    /// \brief Check whether rhs represents less-than or equal-to compare result.
    constexpr Bool IsLessEqualTo(Reference<PartialOrdering> rhs) noexcept;

    /// \brief Check whether rhs represents greater-than compare result.
    constexpr Bool IsGreaterThan(Reference<PartialOrdering> rhs) noexcept;

    /// \brief Check whether rhs represents greater-than or equal-to compare result.
    constexpr Bool IsGreaterEqualTo(Reference<PartialOrdering> rhs) noexcept;

    /************************************************************************/
    /* COMPARISON OPERATORS                                                 */
    /************************************************************************/

    // Strong ordering.
    // ================

    /// \brief Compare lhs to the null literal for equality-comparison.
    constexpr Bool operator==(Reference<StrongOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to rhs for equality-comparison.
    constexpr Bool operator==(Reference<StrongOrdering> lhs, Reference<StrongOrdering> rhs) noexcept = default;

    /// \brief Compare lhs to the null literal for less-than comparison.
    constexpr Bool operator<(Reference<StrongOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than comparison.
    constexpr Bool operator>(Reference<StrongOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for less-than or equal-to comparison.
    constexpr Bool operator<=(Reference<StrongOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than or equal-to comparison.
    constexpr Bool operator>=(Reference<StrongOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than comparison.
    constexpr Bool operator<(Null lhs, Reference<StrongOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than comparison.
    constexpr Bool operator>(Null lhs, Reference<StrongOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than or equal-to comparison.
    constexpr Bool operator<=(Null lhs, Reference<StrongOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than or equal-to comparison.
    constexpr Bool operator>=(Null lhs, Reference<StrongOrdering> rhs) noexcept;

    /// \brief Three-way-comparison of lhs against the null literal.
    constexpr StrongOrdering operator<=>(Reference<StrongOrdering> lhs, Null rhs) noexcept;

    /// \brief Three-way-comparison of the null literal against rhs.
    constexpr StrongOrdering operator<=>(Null lhs, Reference<StrongOrdering> rhs) noexcept;

    // Weak ordering.
    // ==============

    /// \brief Compare lhs to the null literal for equality-comparison.
    constexpr Bool operator==(Reference<WeakOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to rhs for equality-comparison.
    constexpr Bool operator==(Reference<WeakOrdering> lhs, Reference<WeakOrdering> rhs) noexcept = default;

    /// \brief Compare lhs to the null literal for less-than comparison.
    constexpr Bool operator<(Reference<WeakOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than comparison.
    constexpr Bool operator>(Reference<WeakOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for less-than or equal-to comparison.
    constexpr Bool operator<=(Reference<WeakOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than or equal-to comparison.
    constexpr Bool operator>=(Reference<WeakOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than comparison.
    constexpr Bool operator<(Null lhs, Reference<WeakOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than comparison.
    constexpr Bool operator>(Null lhs, Reference<WeakOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than or equal-to comparison.
    constexpr Bool operator<=(Null lhs, Reference<WeakOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than or equal-to comparison.
    constexpr Bool operator>=(Null lhs, Reference<WeakOrdering> rhs) noexcept;

    /// \brief Three-way-comparison of lhs against the null literal.
    constexpr WeakOrdering operator<=>(Reference<WeakOrdering> lhs, Null rhs) noexcept;

    /// \brief Three-way-comparison of the null literal against rhs.
    constexpr WeakOrdering operator<=>(Null lhs, Reference<WeakOrdering> rhs) noexcept;

    // Partial ordering.
    // =================

    /// \brief Compare lhs to the null literal for equality-comparison.
    constexpr Bool operator==(Reference<PartialOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to rhs for equality-comparison.
    constexpr Bool operator==(Reference<PartialOrdering> lhs, Reference<PartialOrdering> rhs) noexcept = default;

    /// \brief Compare lhs to the null literal for less-than comparison.
    constexpr Bool operator<(Reference<PartialOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than comparison.
    constexpr Bool operator>(Reference<PartialOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for less-than or equal-to comparison.
    constexpr Bool operator<=(Reference<PartialOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than or equal-to comparison.
    constexpr Bool operator>=(Reference<PartialOrdering> lhs, Null rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than comparison.
    constexpr Bool operator<(Null lhs, Reference<PartialOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than comparison.
    constexpr Bool operator>(Null lhs, Reference<PartialOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than or equal-to comparison.
    constexpr Bool operator<=(Null lhs, Reference<PartialOrdering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than or equal-to comparison.
    constexpr Bool operator>=(Null lhs, Reference<PartialOrdering> rhs) noexcept;

    /// \brief Three-way-comparison of lhs against the null literal.
    constexpr PartialOrdering operator<=>(Reference<PartialOrdering> lhs, Null rhs) noexcept;

    /// \brief Three-way-comparison of the null literal against rhs.
    constexpr PartialOrdering operator<=>(Null lhs, Reference<PartialOrdering> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TRAITS                                                               */
    /************************************************************************/

    // \brief Determine the strongest ordering category to which all instances in TOrdering can be implicitly converted to.
    template <typename... TOrderings>
    using CommonOrdering = Details::CommonOrdering<TOrderings...>;
}

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    /// \brief Models an ordering category TOrdering which is at least as strong as UOrdering.
    template <typename TOrdering, typename UOrdering>
    concept CompareAs = Details::CompareAs<TOrdering, UOrdering>;

    /// \brief Models a type TType for which the equality and inequality operators are defined.
    template <typename TType>
    concept EqualityComparable = Details::EqualityComparable<TType>;

    /// \brief Models a type TType for which the equality and inequality operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWith = Details::EqualityComparableWith<TType, UType>;

    /// \brief Models a type TType for which the less-than, greater-than, less-than-or-equal-to and greater-than-or-equal-to operators are defined.
    template <typename TType>
    concept PartiallyOrdered = Details::PartiallyOrdered<TType>;

    /// \brief Models a type TType for which the less-than, greater-than, less-than-or-equal-to and greater-than-or-equal-to operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWith = Details::PartiallyOrderedWith<TType, UType>;

    /// \brief Models a class TType which is both equality-comparable and partially-ordered.
    template <typename TType>
    concept TotallyOrdered = Details::TotallyOrdered<TType>;

    /// \brief Models a class TType which is both equality-comparable and partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWith = Details::TotallyOrderedWith<TType, UType>;

    /// \brief Models a totally-ordered class TType whose comparison results are consistent with a ordering category implied by TOrdering.
    template <typename TType, typename TOrdering = PartialOrdering>
    concept ThreeWayComparable = Details::ThreeWayComparable<TType, TOrdering>;

    /// \brief Models a totally-ordered class TType whose comparison results against the (possibly different) type UType are consistent with a ordering category implied by TOrdering.
    template <typename TType, typename UType, typename TOrdering = PartialOrdering>
    concept ThreeWayComparableWith = Details::ThreeWayComparableWith<TType, UType, TOrdering>;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Strong ordering.
    // ================

    inline constexpr StrongOrdering StrongOrdering::Less{ Details::ComparisonResult::kLess };
    inline constexpr StrongOrdering StrongOrdering::Equivalent{ Details::ComparisonResult::kEquivalent };
    inline constexpr StrongOrdering StrongOrdering::Equal{ Details::ComparisonResult::kEqual };
    inline constexpr StrongOrdering StrongOrdering::Greater{ Details::ComparisonResult::kGreater };

    constexpr StrongOrdering::StrongOrdering(Reference<Details::ComparisonResult> value) noexcept
        : value_(value)
    {

    }

    constexpr StrongOrdering::operator WeakOrdering() const noexcept
    {
        return WeakOrdering{ value_ };
    }

    constexpr StrongOrdering::operator PartialOrdering() const noexcept
    {
        return PartialOrdering{ value_ };
    }

    // Weak ordering.
    // ==============

    inline constexpr WeakOrdering WeakOrdering::Less{ Details::ComparisonResult::kLess };
    inline constexpr WeakOrdering WeakOrdering::Equivalent{ Details::ComparisonResult::kEquivalent };
    inline constexpr WeakOrdering WeakOrdering::Greater{ Details::ComparisonResult::kGreater };

    constexpr WeakOrdering::WeakOrdering(Reference<Details::ComparisonResult> value) noexcept
        : value_(value)
    {

    }

    constexpr WeakOrdering::operator PartialOrdering() const noexcept
    {
        return PartialOrdering{ value_ };
    }

    // Partial ordering.
    // =================

    inline constexpr PartialOrdering PartialOrdering::Less{ Details::ComparisonResult::kLess };
    inline constexpr PartialOrdering PartialOrdering::Equivalent{ Details::ComparisonResult::kEquivalent };
    inline constexpr PartialOrdering PartialOrdering::Greater{ Details::ComparisonResult::kGreater };
    inline constexpr PartialOrdering PartialOrdering::Unordered{ Details::ComparisonResult::kIncomparable };

    constexpr PartialOrdering::PartialOrdering(Reference<Details::ComparisonResult> value) noexcept
        : value_(value)
    {

    }

    // Non-member functions.
    // =====================

    constexpr Bool IsEqual(Reference<PartialOrdering> rhs) noexcept
    {
        return rhs == 0;
    }

    constexpr Bool IsNotEqual(Reference<PartialOrdering> rhs) noexcept
    {
        return rhs != 0;
    }

    constexpr Bool IsLessThan(Reference<PartialOrdering> rhs) noexcept
    {
        return rhs < 0;
    }

    constexpr Bool IsLessEqualTo(Reference<PartialOrdering> rhs) noexcept
    {
        return rhs <= 0;
    }

    constexpr Bool IsGreaterThan(Reference<PartialOrdering> rhs) noexcept
    {
        return rhs > 0;
    }

    constexpr Bool IsGreaterEqualTo(Reference<PartialOrdering> rhs) noexcept
    {
        return rhs >= 0;
    }

    // Strong ordering.

    constexpr Bool operator==(Reference<StrongOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kEqual) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator<(Reference<StrongOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess);
    }

    constexpr Bool operator>(Reference<StrongOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater);
    }

    constexpr Bool operator<=(Reference<StrongOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator>=(Reference<StrongOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator<(Null lhs, Reference<StrongOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater);
    }

    constexpr Bool operator>(Null lhs, Reference<StrongOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess);
    }

    constexpr Bool operator<=(Null lhs, Reference<StrongOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater) || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator>=(Null lhs, Reference<StrongOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess) || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr StrongOrdering operator<=>(Reference<StrongOrdering> lhs, Null rhs) noexcept
    {
        return lhs;
    }

    constexpr StrongOrdering operator<=>(Null lhs, Reference<StrongOrdering> rhs) noexcept
    {
        if (rhs.value_ == Details::ComparisonResult::kLess)
        {
            return StrongOrdering{ Details::ComparisonResult::kGreater };
        }

        if (rhs.value_ == Details::ComparisonResult::kGreater)
        {
            return StrongOrdering{ Details::ComparisonResult::kLess };
        }

        return rhs;
    }

    // Weak ordering.

    constexpr Bool operator==(Reference<WeakOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator<(Reference<WeakOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess);
    }

    constexpr Bool operator>(Reference<WeakOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater);
    }

    constexpr Bool operator<=(Reference<WeakOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator>=(Reference<WeakOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator<(Null lhs, Reference<WeakOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater);
    }

    constexpr Bool operator>(Null, Reference<WeakOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess);
    }

    constexpr Bool operator<=(Null lhs, Reference<WeakOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater) || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator>=(Null, Reference<WeakOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess) || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr WeakOrdering operator<=>(Reference<WeakOrdering> lhs, Null rhs) noexcept
    {
        return lhs;
    }

    constexpr WeakOrdering operator<=>(Null lhs, Reference<WeakOrdering> rhs) noexcept
    {
        if (rhs.value_ == Details::ComparisonResult::kLess)
        {
            return WeakOrdering{ Details::ComparisonResult::kGreater };
        }

        if (rhs.value_ == Details::ComparisonResult::kGreater)
        {
            return WeakOrdering{ Details::ComparisonResult::kLess };
        }

        return rhs;
    }

    // Partial ordering.

    constexpr Bool operator==(Reference<PartialOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator<(Reference<PartialOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess);
    }

    constexpr Bool operator>(Reference<PartialOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater);
    }

    constexpr Bool operator<=(Reference<PartialOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator>=(Reference<PartialOrdering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater) || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator<(Null lhs, Reference<PartialOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater);
    }

    constexpr Bool operator>(Null lhs, Reference<PartialOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess);
    }

    constexpr Bool operator<=(Null lhs, Reference<PartialOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater) || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr Bool operator>=(Null lhs, Reference<PartialOrdering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess) || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    constexpr PartialOrdering operator<=>(Reference<PartialOrdering> lhs, Null rhs) noexcept
    {
        return lhs;
    }

    constexpr PartialOrdering operator<=>(Null lhs, Reference<PartialOrdering> rhs) noexcept
    {
        if (rhs.value_ == Details::ComparisonResult::kLess)
        {
            return PartialOrdering{ Details::ComparisonResult::kGreater };
        }

        if (rhs.value_ == Details::ComparisonResult::kGreater)
        {
            return PartialOrdering{ Details::ComparisonResult::kLess };
        }

        return rhs;
    }

}

// ===========================================================================