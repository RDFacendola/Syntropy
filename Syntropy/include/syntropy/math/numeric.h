
/// \file numeric.h
/// \brief This header is part of the Syntropy math module. It contains numerical functions, algorithms and approximations.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/platform/intrinsics.h"
#include "syntropy/math/math.h"

namespace syntropy
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
        constexpr bool ApproximatelyEqual(TNumber lhs, TNumber rhs, TNumber epsilon = TNumber( 0.01 ));

        /// \brief Check whether two numbers are essentially equal.
        /// \param epsilon Error percentage relative to the absolute smaller number.
        template <typename TNumber>
        constexpr bool EssentiallyEqual(TNumber lhs, TNumber rhs, TNumber epsilon = TNumber( 0.01 ));

        /// \brief Get the approximated inverse square root of number.
        template <typename TNumber>
        TNumber FastInvSqrt(TNumber rhs);

        /// \brief Get the approximated square root of number.
        template <typename TNumber>
        TNumber FastSqrt(TNumber rhs);

        /// \brief Get the approximated hyperbolic tangent of rhs.
        constexpr float FastTanh(float rhs);
    }

	/************************************************************************/
	/* IMPLEMENTATION                                                       */
	/************************************************************************/

    // Math.

    template <typename TNumber>
    constexpr bool Math::ApproximatelyEqual(TNumber lhs, TNumber rhs, TNumber epsilon)
    {
        auto abs_lhs = Abs(lhs);
        auto abs_rhs = Abs(rhs);

        return Abs(lhs - rhs) <= ((abs_lhs < abs_rhs ? abs_rhs : abs_lhs) * epsilon);
    }

    template <typename TNumber>
    constexpr bool Math::EssentiallyEqual(TNumber lhs, TNumber rhs, TNumber epsilon)
    {
        auto abs_lhs = Abs(lhs);
        auto abs_rhs = Abs(rhs);

        return Abs(lhs - rhs) <= ((abs_lhs > abs_rhs ? abs_rhs : abs_lhs) * epsilon);
    }

    template <typename TNumber>
    inline TNumber Math::FastInvSqrt(TNumber rhs)
    {
        return static_cast<TNumber>(Intrinsics::GetFastInvSqrt(static_cast<float>(rhs)));
    }

    template <typename TNumber>
    inline TNumber Math::FastSqrt(TNumber rhs)
    {
        return static_cast<TNumber>(1.0f / Intrinsics::GetFastInvSqrt(static_cast<float>(rhs)));
    }

    constexpr float Math::FastTanh(float rhs)
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
            float rhs2 = rhs * rhs;

            return rhs * (27.0f + rhs2) / (27.0f + 9.0f * rhs2);
        }
    }

}
