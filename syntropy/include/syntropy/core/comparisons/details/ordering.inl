
/// \file ordering.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* ORDERING                                                             */
    /************************************************************************/

    constexpr
    Ordering
    ::Ordering(Immutable<std::strong_ordering> value) noexcept
        : Ordering(static_cast<std::partial_ordering>(value))
    {

    }

    constexpr
    Ordering
    ::Ordering(Immutable<std::weak_ordering> value) noexcept
        : Ordering(static_cast<std::partial_ordering>(value))
    {

    }

    constexpr
    Ordering
    ::Ordering(Immutable<std::partial_ordering> value) noexcept
    {
        if(value == std::partial_ordering::less)
        {
            value_ = Comparison::kLess;
        }
        else if(value == std::partial_ordering::equivalent)
        {
            value_ = Comparison::kEquivalent;
        }
        else if(value == std::partial_ordering::greater)
        {
            value_ = Comparison::kGreater;
        }
        else
        {
            value_ = Comparison::kIncomparable;
        }
    }

    constexpr
    Ordering
    ::Ordering(Immutable<Comparison> value) noexcept
        : value_(value)
    {

    }

    inline constexpr
    Ordering Ordering::kLess{ Ordering::Comparison::kLess };

    inline constexpr
    Ordering Ordering::kEquivalent{ Ordering::Comparison::kEquivalent };

    inline constexpr
    Ordering Ordering::kGreater{ Ordering::Comparison::kGreater };

    inline constexpr
    Ordering Ordering::kIncomparable{ Ordering::Comparison::kIncomparable };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Ordering.
    // =========

    [[nodiscard]] constexpr
    Bool
    IsEqual(Immutable<Ordering> rhs) noexcept
    {
        return rhs == nullptr;
    }

    [[nodiscard]] constexpr
    Bool
    IsNotEqual(Immutable<Ordering> rhs) noexcept
    {
        return rhs != nullptr;
    }

    [[nodiscard]] constexpr
    Bool
    IsLessThan(Immutable<Ordering> rhs) noexcept
    {
        return rhs < nullptr;
    }

    [[nodiscard]] constexpr
    Bool
    IsLessEqualTo(Immutable<Ordering> rhs) noexcept
    {
        return rhs <= nullptr;
    }

    [[nodiscard]] constexpr
    Bool
    IsGreaterThan(Immutable<Ordering> rhs) noexcept
    {
        return rhs > nullptr;
    }

    [[nodiscard]] constexpr
    Bool
    IsGreaterEqualTo(Immutable<Ordering> rhs) noexcept
    {
        return rhs >= nullptr;
    }

    [[nodiscard]] constexpr
    Ordering
    Flip(Immutable<Ordering> rhs) noexcept
    {
        if (rhs == Ordering::kLess)
        {
            return Ordering::kGreater;
        }

        if (rhs == Ordering::kGreater)
        {
            return Ordering::kLess;
        }

        return rhs;
    }

    // Comparison operators.
    // =====================

    [[nodiscard]] constexpr
    Bool
    operator==(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Ordering::Comparison::kEquivalent);
    }

    [[nodiscard]] constexpr
    Bool
    operator==(Immutable<Ordering> lhs, Immutable<Ordering> rhs) noexcept
    {
        return (lhs.value_ == rhs.value_);
    }

    [[nodiscard]] constexpr
    Bool
    operator<(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Ordering::Comparison::kLess);
    }

    [[nodiscard]] constexpr
    Bool
    operator>(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Ordering::Comparison::kGreater);
    }

    [[nodiscard]] constexpr
    Bool
    operator<=(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Ordering::Comparison::kLess)
            || (lhs.value_ == Ordering::Comparison::kEquivalent);
    }

    [[nodiscard]] constexpr
    Bool
    operator>=(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return (lhs.value_ == Ordering::Comparison::kGreater)
            || (lhs.value_ == Ordering::Comparison::kEquivalent);
    }

    [[nodiscard]] constexpr
    Bool
    operator<(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Ordering::Comparison::kGreater);
    }

    [[nodiscard]] constexpr
    Bool
    operator>(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Ordering::Comparison::kLess);
    }

    [[nodiscard]] constexpr
    Bool
    operator<=(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Ordering::Comparison::kGreater)
            || (rhs.value_ == Ordering::Comparison::kEquivalent);
    }

    [[nodiscard]] constexpr
    Bool
    operator>=(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return (rhs.value_ == Ordering::Comparison::kLess)
            || (rhs.value_ == Ordering::Comparison::kEquivalent);
    }

    [[nodiscard]] constexpr
    Ordering
    operator<=>(Immutable<Ordering> lhs, Null rhs) noexcept
    {
        return lhs;
    }

    [[nodiscard]] constexpr
    Ordering
    operator<=>(Null lhs, Immutable<Ordering> rhs) noexcept
    {
        return Flip(rhs);
    }

}

// ===========================================================================
