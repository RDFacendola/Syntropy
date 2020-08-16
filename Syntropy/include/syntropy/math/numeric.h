
/// \file numeric.h
/// \brief This header is part of the Syntropy math module. It contains numerical functions, algorithms and approximations.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cmath>

#include "syntropy/platform/intrinsics.h"
#include "syntropy/math/math.h"
#include "syntropy/language/language_types.h"

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes numerical functions and algorithms.
    /// \author Raffaele D. Facendola - May 2020.
    namespace Math
    {
        /// \brief Check whether two numbers are approximately equal.
        /// \param epsilon Error percentage relative to the absolute larger number.
        template <typename TNumber>
        constexpr Bool ApproximatelyEqual(TNumber lhs, TNumber rhs, TNumber epsilon = TNumber( 0.01 ));

        /// \brief Check whether two numbers are essentially equal.
        /// \param epsilon Error percentage relative to the absolute smaller number.
        template <typename TNumber>
        constexpr Bool EssentiallyEqual(TNumber lhs, TNumber rhs, TNumber epsilon = TNumber( 0.01 ));

        /// \brief Get the smaller value greater than rhs.
        template <typename TNumber>
        TNumber Next(TNumber rhs);

        /// \brief Get the bigger value smaller than rhs.
        template <typename TNumber>
        TNumber Previous(TNumber rhs);

        /// \brief Get the approximated inverse square root of number.
        template <typename TNumber>
        TNumber FastInvSqrt(TNumber rhs);

        /// \brief Get the approximated square root of number.
        template <typename TNumber>
        TNumber FastSqrt(TNumber rhs);

        /// \brief Get the approximated hyperbolic tangent of rhs.
        constexpr Float FastTanh(Float rhs);
    }

	/************************************************************************/
	/* IMPLEMENTATION                                                       */
	/************************************************************************/

    // Math.

    template <typename TNumber>
    constexpr Bool Math::ApproximatelyEqual(TNumber lhs, TNumber rhs, TNumber epsilon)
    {
        auto abs_lhs = Abs(lhs);
        auto abs_rhs = Abs(rhs);

        return Abs(lhs - rhs) <= ((abs_lhs < abs_rhs ? abs_rhs : abs_lhs) * epsilon);
    }

    template <typename TNumber>
    constexpr Bool Math::EssentiallyEqual(TNumber lhs, TNumber rhs, TNumber epsilon)
    {
        auto abs_lhs = Abs(lhs);
        auto abs_rhs = Abs(rhs);

        return Abs(lhs - rhs) <= ((abs_lhs > abs_rhs ? abs_rhs : abs_lhs) * epsilon);
    }

    template <typename TNumber>
    inline TNumber Math::Next(TNumber rhs)
    {
        return std::nextafter(rhs, rhs + TNumber(1));
    }

    template <typename TNumber>
    inline TNumber Math::Previous(TNumber rhs)
    {
        return std::nextafter(rhs, rhs - TNumber(1));
    }

    template <typename TNumber>
    inline TNumber Math::FastInvSqrt(TNumber rhs)
    {
        return static_cast<TNumber>(Intrinsics::GetFastInvSqrt(static_cast<Float>(rhs)));
    }

    template <typename TNumber>
    inline TNumber Math::FastSqrt(TNumber rhs)
    {
        return static_cast<TNumber>(1.0f / Intrinsics::GetFastInvSqrt(static_cast<Float>(rhs)));
    }

    constexpr Float Math::FastTanh(Float rhs)
    {
        // Pade approximation.

        if (rhs < -3.0f)
        {
            return -1.0f;
        }
        else if (rhs > 3.0f)
        {
            return +1.0f;
        }
        else
        {
            Float rhs2 = rhs * rhs;

            return rhs * (27.0f + rhs2) / (27.0f + 9.0f * rhs2);
        }
    }

}
