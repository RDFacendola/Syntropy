
/// \file compare.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* ORDERING                                                             */
    /************************************************************************/

    inline constexpr
    Ordering Ordering::kLess{ Details::ComparisonResult::kLess };

    inline constexpr
    Ordering Ordering::kEquivalent{ Details::ComparisonResult::kEquivalent };

    inline constexpr
    Ordering Ordering::kGreater{ Details::ComparisonResult::kGreater };

    constexpr
    Ordering::Ordering(Immutable<std::strong_ordering> value) noexcept
        : value_(Details::ToComparisonResult(value))
    {

    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    [[nodiscard]] constexpr
    Bool IsEqual(Immutable<Ordering> rhs) noexcept
    {
        return rhs == 0;
    }

    [[nodiscard]] constexpr
    Bool IsNotEqual(Immutable<Ordering> rhs) noexcept
    {
        return rhs != 0;
    }

    [[nodiscard]] constexpr
    Bool IsLessThan(Immutable<Ordering> rhs) noexcept
    {
        return rhs < 0;
    }

    [[nodiscard]] constexpr
    Bool IsLessEqualTo(Immutable<Ordering> rhs) noexcept
    {
        return rhs <= 0;
    }

    [[nodiscard]] constexpr
    Bool IsGreaterThan(Immutable<Ordering> rhs) noexcept
    {
        return rhs > 0;
    }

    [[nodiscard]] constexpr
    Bool IsGreaterEqualTo(Immutable<Ordering> rhs) noexcept
    {
        return rhs >= 0;
    }

    [[nodiscard]] constexpr
    Ordering Flip(Immutable<Ordering> rhs) noexcept
    {
        if (rhs == Ordering::kLess)
        {
            return Ordering::kGreater;
        }

        if (rhs == Ordering::kGreater)
        {
            return Ordering::kLess;
        }

        return Ordering::kEquivalent;
    }

    /************************************************************************/
    /* COMPARISON OPERATORS                                                 */
    /************************************************************************/

    // Ordering.
    // =========

    [[nodiscard]] constexpr
    Bool operator==(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    [[nodiscard]] constexpr
    Bool operator==(Immutable<Ordering> lhs, Immutable<Ordering> rhs) noexcept
    {
        return (lhs.value_ == rhs.value_);
    }

    [[nodiscard]] constexpr
    Bool operator<(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess);
    }

    [[nodiscard]] constexpr
    Bool operator>(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater);
    }

    [[nodiscard]] constexpr
    Bool operator<=(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kLess)
            || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    [[nodiscard]] constexpr
    Bool operator>=(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Details::ComparisonResult::kGreater)
            || (lhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    [[nodiscard]] constexpr
    Bool operator<(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater);
    }

    [[nodiscard]] constexpr
    Bool operator>(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess);
    }

    [[nodiscard]] constexpr
    Bool operator<=(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kGreater)
            || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    [[nodiscard]] constexpr
    Bool operator>=(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Details::ComparisonResult::kLess)
            || (rhs.value_ == Details::ComparisonResult::kEquivalent);
    }

    [[nodiscard]] constexpr
    Ordering operator<=>(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return lhs;
    }

    [[nodiscard]] constexpr
    Ordering operator<=>(Null lhs, Immutable<Ordering> rhs) noexcept
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
