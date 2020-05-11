
/// \file approximations.h
/// \brief This header is part of the Syntropy math module. It contains fast numeric functions approximations.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/platform/intrinsics.h"
#include "syntropy/math/arithmetic.h"

namespace syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes approximation functions.
    /// \author Raffaele D. Facendola - May 2020.
    namespace Math
    {
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

        if (rhs > 3.0f)
        {
            return +1.0f;
        }

        float rhs2 = rhs * rhs;

        return rhs * (27.0f + rhs2) / (27.0f + 9.0f * rhs2);
    }

}