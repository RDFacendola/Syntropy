
/// \file math.h
/// \brief This header is part of the Syntropy math module. It contains generic mathematical functions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>

#include "syntropy/language/templates/traits.h"

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes arithmetic functions.
    /// \author Raffaele D. Facendola - May 2020.
    namespace Math
    {
        /// \brief Get the minimum element.
        template <typename TValue, typename... TValues>
        constexpr Templates::CommonType<TValue, TValues...> Min(const TValue& value, const TValues&... values) noexcept;

        /// \brief Get the maximum element.
        template <typename TValue, typename... TValues>
        constexpr Templates::CommonType<TValue, TValues...> Max(const TValue& value, const TValues&... values) noexcept;

        /// \brief Clamp an element between a minimum and a maximum.
        template <typename TValue, typename TMin, typename TMax>
        constexpr Templates::CommonType<TValue, TMin, TMax> Clamp(const TValue& value, const TMin& min, const TMax& max) noexcept;

        /// \brief Get the smallest integral number greater than or equal to rhs.
        template <typename TNumber>
        constexpr TNumber Ceil(TNumber rhs);

        /// \brief Get the largest integral number smaller than or equal to rhs.
        template <typename TNumber>
        constexpr TNumber Floor(TNumber rhs);

        /// \brief Get the smallest integral number greater than or equal to rhs and convert the result.
        template <typename TNumber, typename UNumber>
        constexpr TNumber CeilTo(UNumber rhs);

        /// \brief Get the largest integral number smaller than or equal to rhs and convert the result.
        template <typename TNumber, typename UNumber>
        constexpr TNumber FloorTo(UNumber rhs);

        /// \brief Ceil a number to a multiple of another value.
        /// \return Returns the first number equal or greater than number which is multiple of multiple.
        template <typename TNumber>
        constexpr TNumber Ceil(TNumber rhs, TNumber multiple);

        /// \brief Floor a number to a multiple.
        /// \return Returns the first number equal or lesser than number which is multiple of multiple.
        template <typename TNumber>
        constexpr TNumber Floor(TNumber rhs, TNumber multiple);

        /// \brief Divide lhs by rhs and ceil the result to the next integer value.
        /// \return Returns lhs divided by rhs, rounded up to the next integer value.
        template <typename TNumber>
        constexpr auto DivCeil(TNumber lhs, TNumber rhs);

        /// \brief Divide lhs by rhs and floor the result to the previous integer value.
        /// \return Returns lhs divided by rhs, rounded down to the previous integer value.
        template <typename TNumber>
        constexpr auto DivFloor(TNumber lhs, TNumber rhs);

        /// \brief Get the remainder after the division of lhs by rhs.
        /// \remarks The sign of the remainder is the same as lhs's.
        template <typename TNumber>
        constexpr TNumber Mod(TNumber lhs, TNumber rhs);

        /// \brief Wrap lhs around in the range [0; rhs)
        template <typename TNumber>
        constexpr TNumber Wrap(TNumber lhs, TNumber rhs);

        /// \brief Get the absolute value of number.
        template <typename TNumber>
        constexpr TNumber Abs(TNumber rhs);

        /// \brief Get the natural logarithm of rhs.
        template <typename TNumber>
        constexpr TNumber Ln(TNumber rhs);

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Math.

    template <typename TValue, typename... TValues>
    constexpr Templates::CommonType<TValue, TValues...> Math::Min(const TValue& value, const TValues&... values) noexcept
    {
        if constexpr (sizeof...(values) == 0)
        {
            return value;
        }
        else
        {
            auto min = Min(values...);

            return (value < min) ? value : min;
        }
    }

    template <typename TValue, typename... TValues>
    constexpr Templates::CommonType<TValue, TValues...> Math::Max(const TValue& value, const TValues&... values) noexcept
    {
        if constexpr (sizeof...(values) == 0)
        {
            return value;
        }
        else
        {
            auto max = Max(values...);

            return (value > max) ? value : max;
        }
    }

    template <typename TValue, typename TMin, typename TMax>
    constexpr Templates::CommonType<TValue, TMin, TMax> Math::Clamp(const TValue& value, const TMin& min, const TMax& max) noexcept
    {
        return std::clamp(value, min, max);
    }

    template <typename TNumber>
    constexpr TNumber Math::Ceil(TNumber rhs)
    {
        return std::ceil(rhs);
    }

    template <typename TNumber>
    constexpr TNumber Math::Floor(TNumber rhs)
    {
        return std::floor(rhs);
    }

    template <typename TNumber>
    constexpr TNumber Math::Ceil(TNumber rhs, TNumber multiple)
    {
        return DivCeil(rhs, multiple) * multiple;
    }

    template <typename TNumber>
    constexpr TNumber Math::Floor(TNumber rhs, TNumber multiple)
    {
        return rhs;
        //return DivFloor(rhs, multiple) * multiple;
    }

    template <typename TNumber, typename UNumber>
    constexpr TNumber Math::CeilTo(UNumber rhs)
    {
        return static_cast<TNumber>(Ceil(rhs));
    }

    template <typename TNumber, typename UNumber>
    constexpr TNumber Math::FloorTo(UNumber rhs)
    {
        return static_cast<TNumber>(Floor(rhs));
    }

    template <typename TNumber>
    constexpr auto Math::DivCeil(TNumber lhs, TNumber rhs)
    {
        return DivFloor(lhs + rhs - TNumber{ 1 }, rhs);
    }

    template <typename TNumber>
    constexpr auto Math::DivFloor(TNumber lhs, TNumber rhs)
    {
        auto quotient = lhs / rhs;
        auto remainder = lhs % rhs;

//         if ((remainder != decltype(remainder){ 0 }) && ((lhs < TNumber{ 0 }) ^ (rhs < TNumber{ 0 })))
//         {
//             quotient -= decltype(quotient){ 1 };
//         }

        return quotient;
    }

    template <typename TNumber>
    constexpr TNumber Math::Mod(TNumber lhs, TNumber rhs)
    {
        if constexpr (std::is_floating_point_v<TNumber>)
        {
            return std::fmod(lhs, rhs);
        }
        else
        {
            return lhs % rhs;
        }
    }

    template <typename TNumber>
    constexpr TNumber Math::Wrap(TNumber lhs, TNumber rhs)
    {
        lhs = Mod(lhs, rhs);
        lhs = (lhs >= TNumber{ 0 }) ? lhs : (lhs + rhs);

        return lhs;
    }

    template <typename TNumber>
    constexpr TNumber Math::Abs(TNumber rhs)
    {
        return (rhs > TNumber{ 0 }) ? rhs : -rhs;
    }

    template <typename TNumber>
    constexpr TNumber Math::Ln(TNumber rhs)
    {
        return std::log(rhs);
    }

}