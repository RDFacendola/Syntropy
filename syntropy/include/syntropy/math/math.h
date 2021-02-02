
/// \file math.h
///
/// \brief This header is part of the Syntropy math module.
///        It contains generic mathematical functions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Math
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Get the minimum element among first and rest.
    template <typename TValue, typename... TValues>
    [[nodiscard]] constexpr Templates::CommonType<TValue, TValues...>
    Min(Immutable<TValue> first, Immutable<TValues>... rest) noexcept;

    /// \brief Get the maximum element among first and rest.
    template <typename TValue, typename... TValues>
    [[nodiscard]] constexpr Templates::CommonType<TValue, TValues...>
    Max(Immutable<TValue> first, Immutable<TValues>... rest) noexcept;

    /// \brief Get the smallest integral number greater than or equal to rhs.
    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Ceil(TNumber rhs);

    /// \brief Get the largest integral number smaller than or equal to rhs.
    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Floor(TNumber rhs);

    /// \brief Ceil a number to a multiple of another value.
    /// \return Returns the first number equal or greater than number which is
    ///         multiple of multiple.
    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Ceil(TNumber rhs, TNumber multiple);

    /// \brief Floor a number to a multiple.
    /// \return Returns the first number equal or lesser than number which is
    ///         multiple of multiple.
    template <typename TNumber>
    [[nodiscard]] constexpr Number
    Floor(TNumber rhs, TNumber multiple);

    /// \brief Divide lhs by rhs and ceil the result to the next integer value.
    /// \return Returns lhs divided by rhs, rounded up to the next
    ///        integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr auto
    DivCeil(TNumber lhs, TNumber rhs);

    /// \brief Divide lhs by rhs and floor the result to the previous integer
    ///        value.
    /// \return Returns lhs divided by rhs, rounded down to the previous
    ///         integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr auto
    DivFloor(TNumber lhs, TNumber rhs);

}

// ===========================================================================


namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Math.
    // =====

    template <typename TValue, typename... TValues>
    [[nodiscard]] constexpr Templates::CommonType<TValue, TValues...>
    Math::Min(Immutable<TValue> first, Immutable<TValues>... rest) noexcept
    {
        auto min = Templates::CommonType<TValue, TValues...>{ first };

        auto fold = [&min]<typename UValue>(Immutable<UValue> value)
        {
            min = (min < value) ? min : value;
        };

        (fold(rest), ...);

        return min;
    }

    template <typename TValue, typename... TValues>
    [[nodiscard]] constexpr Templates::CommonType<TValue, TValues...>
    Math::Max(Immutable<TValue> first, Immutable<TValues>... rest) noexcept
    {
        auto max = Templates::CommonType<TValue, TValues...>{ first };

        auto fold = [&max]<typename UValue>(Immutable<UValue> value)
        {
            max = (max > value) ? max : value;
        };

        (fold(rest), ...);

        return max;
    }

    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Math::Ceil(TNumber rhs)
    {
        return std::ceil(rhs);
    }

    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Math::Floor(TNumber rhs)
    {
        return std::floor(rhs);
    }

    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Math::Ceil(TNumber rhs, TNumber multiple)
    {
        return DivCeil(rhs, multiple) * multiple;
    }

    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Math::Floor(TNumber rhs, TNumber multiple)
    {
        return DivFloor(rhs, multiple) * multiple;
    }

    template <typename TNumber>
    [[nodiscard]] constexpr auto
    Math::DivCeil(TNumber lhs, TNumber rhs)
    {
        return DivFloor(lhs + rhs - TNumber{ 1 }, rhs);
    }

    template <typename TNumber>
    [[nodiscard]] constexpr auto
    Math::DivFloor(TNumber lhs, TNumber rhs)
    {
        auto quotient = lhs / rhs;
        auto remainder = lhs % rhs;

        if (   (remainder != decltype(remainder){ 0 })
            && ((lhs < TNumber{ 0 }) ^ (rhs < TNumber{ 0 })))
        {
            quotient -= decltype(quotient){ 1 };
        }

        return quotient;
    }
}

// ===========================================================================
