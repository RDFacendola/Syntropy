
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

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    class String;

    /************************************************************************/
    /* ORDERING                                                             */
    /************************************************************************/

    /// \brief Defines a (partial) ordering relationship between objects.
    ///
    /// \author Raffaele D. Facendola - November 2020.
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

        /// \brief Indicates that an element is incomparable with another
        ///        element.
        static const Ordering kIncomparable;

        /// \brief Convert from std::strong_ordering to Ordering.
        constexpr
        Ordering(Immutable<std::strong_ordering> value) noexcept;


        /// \brief Convert from std::weak_ordering to Ordering.
        constexpr
        Ordering(Immutable<std::weak_ordering> value) noexcept;


        /// \brief Convert from std::partial_ordering to Ordering.
        constexpr
        Ordering(Immutable<std::partial_ordering> value) noexcept;

    private:

        /// \brief Result of a comparison.
        enum class Comparison : Enum8
        {
            /// \brief Less-than result.
            kLess,

            /// \brief Equivalent-to result.
            kEquivalent,

            /// \brief Greater-than result.
            kGreater,

            /// \brief Incomparable result.
            kIncomparable
        };

        /// \brief Direct constructor.
        constexpr
        Ordering(Immutable<Comparison> value) noexcept;

        /// \brief Comparison value.
        Comparison value_;

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

    // ToString.
    // =========

    /// \brief Convert an ordering value to a string.
    Immutable<String> ToString(Immutable<Ordering> rhs) noexcept;

}

// ===========================================================================

#include "details/ordering.inl"

// ===========================================================================
