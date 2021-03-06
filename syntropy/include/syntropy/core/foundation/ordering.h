
/// \file ordering.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ordering.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include <compare>

// ===========================================================================

#include "details/ordering.details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* ORDERING                                                             */
    /************************************************************************/

    /// \brief Defines a strong ordering relationship between objects.
    ///
    /// In strong order relationships, equivalent elements are
    /// indistinguishable. Incomparable elements are not allowed.
    /// \author Raffaele D. Facendola - Nov 2020.
    class Ordering
    {
        friend constexpr
        Bool operator==(Immutable<Ordering> lhs, Null rhs) noexcept;

        friend constexpr
        Bool operator==(Immutable<Ordering> lhs,
                        Immutable<Ordering> rhs) noexcept;

        friend constexpr
        Bool operator<(Immutable<Ordering> lhs, Null rhs) noexcept;

        friend constexpr
        Bool operator>(Immutable<Ordering> lhs, Null rhs) noexcept;

        friend constexpr
        Bool operator<=(Immutable<Ordering> lhs, Null rhs) noexcept;

        friend constexpr
        Bool operator>=(Immutable<Ordering> lhs, Null rhs) noexcept;

        friend constexpr
        Bool operator<(Null, Immutable<Ordering> rhs) noexcept;

        friend constexpr
        Bool operator>(Null, Immutable<Ordering> rhs) noexcept;

        friend constexpr
        Bool operator<=(Null, Immutable<Ordering> rhs) noexcept;

        friend constexpr
        Bool operator>=(Null, Immutable<Ordering> rhs) noexcept;

        friend constexpr
        Ordering operator<=>(Immutable<Ordering> lhs, Null rhs) noexcept;

        friend constexpr
        Ordering operator<=>(Null lhs, Immutable<Ordering> rhs) noexcept;

    public:

        /// \brief Indicates that an element is less-than another element.
        static const Ordering kLess;

        /// \brief Indicates that an Ordering is equivalent-to another element.
        static const Ordering kEquivalent;

        /// \brief Indicates that an element is greater-than another element.
        static const Ordering kGreater;

        /// \brief Convert from std::strong_ordering to Ordering.
        /// \remarks Allows better interoperability with STL.
        constexpr
        Ordering(Immutable<std::strong_ordering> value) noexcept;

    private:

        /// \brief Create a new weak ordering result value.
        constexpr explicit
        Ordering(Immutable<Details::ComparisonResult> value) noexcept
            : value_(value)
        {
            // @rfacendola Apparently in VS2019 using the explicit
            // keyword requires both declaration and definition
            // at the same time, otherwise it will ignore that and
            // zero-initialize the instance.
        }

        /// \brief Underlying compare result.
        Details::ComparisonResult value_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Ordering.
    // =========

    /// \brief Check whether rhs represents equal-to compare result.
    [[nodiscard]] constexpr
    Bool IsEqual(Immutable<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents not-equal-to compare result.
    [[nodiscard]] constexpr
    Bool IsNotEqual(Immutable<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents less-than compare result.
    [[nodiscard]] constexpr
    Bool IsLessThan(Immutable<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents less-than or
    ///        equal-to compare result.
    [[nodiscard]] constexpr
    Bool IsLessEqualTo(Immutable<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents greater-than compare result.
    [[nodiscard]] constexpr
    Bool IsGreaterThan(Immutable<Ordering> rhs) noexcept;

    /// \brief Check whether rhs represents greater-than
    ///        or equal-to compare result.
    [[nodiscard]] constexpr
    Bool IsGreaterEqualTo(Immutable<Ordering> rhs) noexcept;

    /// \brief Obtain the comparison value the expression which resulted
    ///        in rhs would return if its arguments were reversed.
    ///
    /// \remarks This method assumes total ordering, that is a relationship
    ///          which is antisymmetric, transitive
    [[nodiscard]] constexpr
    Ordering Flip(Immutable<Ordering> rhs) noexcept;

    // Comparison operators.
    // =====================

    /// \brief Compare lhs to the null literal for equality-comparison.
    [[nodiscard]] constexpr
    Bool operator==(Immutable<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to rhs for equality-comparison.
    [[nodiscard]] constexpr
    Bool operator==(Immutable<Ordering> lhs,
                    Immutable<Ordering> rhs) noexcept;

    /// \brief Compare lhs to the null literal for less-than comparison.
    [[nodiscard]] constexpr
    Bool operator<(Immutable<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than comparison.
    [[nodiscard]] constexpr
    Bool operator>(Immutable<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for less-than
    ///        or equal-to comparison.
    [[nodiscard]] constexpr
    Bool operator<=(Immutable<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare lhs to the null literal for greater-than
    ///        or equal-to comparison.
    [[nodiscard]] constexpr
    Bool operator>=(Immutable<Ordering> lhs, Null rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than comparison.
    [[nodiscard]] constexpr
    Bool operator<(Null lhs, Immutable<Ordering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than comparison.
    [[nodiscard]] constexpr
    Bool operator>(Null lhs, Immutable<Ordering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for less-than
    ///        or equal-to comparison.
    [[nodiscard]] constexpr
    Bool operator<=(Null lhs, Immutable<Ordering> rhs) noexcept;

    /// \brief Compare the null literal to rhs for greater-than
    ///        or equal-to comparison.
    [[nodiscard]] constexpr
    Bool operator>=(Null lhs, Immutable<Ordering> rhs) noexcept;

    /// \brief Three-way-comparison of lhs against the null literal.
    [[nodiscard]] constexpr
    Ordering operator<=>(Immutable<Ordering> lhs, Null rhs) noexcept;

    /// \brief Three-way-comparison of the null literal against rhs.
    [[nodiscard]] constexpr
    Ordering operator<=>(Null lhs, Immutable<Ordering> rhs) noexcept;

}

// ===========================================================================

#include "details/ordering.inl"

// ===========================================================================
