
/// \file math.h
/// \brief This header is part of the syntropy math system. It contains generic math functions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <cmath>
#include <algorithm>

#include "syntropy/platform/builtin.h"

namespace syntropy
{
    /// \brief Get the base 2 logarithm of a number and ceil the result to the next integer value.
    /// If the provided number is 0 or less the result is undefined.
    /// \return Returns the base 2 logarithm of the provided number rounded up to the next integer value.
    template <typename TNumber>
    TNumber CeilLog2(TNumber rhs);

    /// \brief Get the base 2 logarithm of a number and floor the result to the previous integer value.
    /// If the provided number is 0 or less the result is undefined.
    /// \return Returns the base 2 logarithm of the provided number rounded down to the previous integer value.
    template <typename TNumber>
    TNumber FloorLog2(TNumber rhs);

    /// \brief Round the argument up to the next power of 2.
    /// \return Returns the smallest power of 2 greater than or equal to number.
    template <typename TNumber>
    TNumber NextPow2(TNumber number);

    // Utilities.

    /// \brief Get the remainder after the division of lhs by rhs.
    /// \remarks The sign of the remainder is the same as lhs's.
    template <typename TNumber>
    constexpr TNumber Mod(TNumber lhs, TNumber rhs);

    /// \brief Wrap lhs around in the range [0; rhs)
    template <typename TNumber>
    constexpr TNumber Wrap(TNumber lhs, TNumber rhs);

    // Comparison.

    /// \brief Check whether two numbers are approximately equal.
    /// \param epsilon Error percentage relative to the absolute larger number.
    constexpr bool ApproximatelyEqual(float lhs, float rhs, float epsilon = 0.01f);

    /// \brief Check whether two numbers are essentially equal.
    /// \param epsilon Error percentage relative to the absolute smaller number.
    constexpr bool EssentiallyEqual(float lhs, float rhs, float epsilon = 0.01f);

    // Interpolation.

    /// \brief Linearly interpolate lhs and rhs using alpha as blending factor.
    template <typename TType>
    constexpr TType Lerp(const TType& lhs, const TType& rhs, float alpha);

    // Fast approximation functions.

    /// \brief Get the absolute value of number.
    template <typename TNumber>
    constexpr TNumber FastAbs(TNumber rhs);

    /// \brief Get the approximated inverse square root of number.
    float FastInvSqrt(float rhs);

    /// \brief Get the square root of number.
    float FastSqrt(float rhs);

    /// \brief Get the approximated hyperbolic tangent of rhs.
    constexpr float FastTanh(float rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/



    template <typename TNumber>
    TNumber CeilLog2(TNumber rhs)
    {
        static_assert(std::is_integral_v<TNumber>, "Expected an integral type.");

        auto msb = platform::BuiltIn::GetMostSignificantBit(static_cast<uint64_t>(rhs));

        return msb + ((rhs & (rhs - TNumber(1))) >> msb);                           // Ceiling required for non-power of 2.
    }

    template <typename TNumber>
    TNumber FloorLog2(TNumber rhs)
    {
        static_assert(std::is_integral_v<TNumber>, "Expected an integral type.");

        return platform::BuiltIn::GetMostSignificantBit(static_cast<uint64_t>(rhs));
    }

    template <typename TNumber>
    inline TNumber NextPow2(TNumber number)
    {
        static_assert(std::is_integral_v<TNumber>, "Expected an integral type.");

        if (number > TNumber{ 0 })
        {
            return IsPow2(number) ? number : TNumber(2 << platform::BuiltIn::GetMostSignificantBit(static_cast<uint64_t>(number)));
        }

        return TNumber{ 1 };        // 2^0, assuming integral type.
    }

    template <typename TNumber>
    constexpr TNumber Mod(TNumber lhs, TNumber rhs)
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
    constexpr TNumber Wrap(TNumber lhs, TNumber rhs)
    {
        lhs = Mod(lhs, rhs);
        lhs = (lhs >= TNumber(0)) ? lhs : (lhs + rhs);

        return lhs;
    }

    constexpr bool ApproximatelyEqual(float lhs, float rhs, float epsilon)
    {
        auto abs_lhs = FastAbs(lhs);
        auto abs_rhs = FastAbs(rhs);

        return FastAbs(lhs - rhs) <= ((abs_lhs < abs_rhs ? abs_rhs : abs_lhs) * epsilon);
    }

    constexpr bool EssentiallyEqual(float lhs, float rhs, float epsilon)
    {
        auto abs_lhs = FastAbs(lhs);
        auto abs_rhs = FastAbs(rhs);

        return FastAbs(lhs - rhs) <= ((abs_lhs > abs_rhs ? abs_rhs : abs_lhs) * epsilon);
    }

    template <typename TType>
    constexpr TType Lerp(const TType& lhs, const TType& rhs, float alpha)
    {
        return lhs + (rhs - lhs) * alpha;
    }

    template <typename TNumber>
    constexpr TNumber FastAbs(TNumber rhs)
    {
        return rhs > 0 ? rhs : -rhs;
    }

    inline float FastInvSqrt(float rhs)
    {
        return platform::BuiltIn::GetFastInvSqrt(rhs);
    }

    inline float FastSqrt(float rhs)
    {
        return 1.0f / FastInvSqrt(rhs);
    }

    constexpr float FastTanh(float rhs)
    {
        // Pade approximation.

        if (rhs < -3.0f)
        {
            return -1.0f;
        }

        if (rhs > 3.0f)
        {
            return +1.0f;
        }

        float rhs2 = rhs * rhs;

        return rhs * (27.0f + rhs2) / (27.0f + 9.0f * rhs2);
    }
}