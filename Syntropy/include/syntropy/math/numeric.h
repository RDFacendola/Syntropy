
/// \file numeric.h
/// \brief This header is part of the Syntropy math module. It contains numerical functions and algorithms.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/language/type_traits.h"

#include "syntropy/math/arithmetic.h"

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
    }

	/************************************************************************/
	/* IMPLEMENTATION                                                       */
	/************************************************************************/

    // Math.

    template <typename TNumber>
    constexpr bool Math::ApproximatelyEqual(TNumber lhs, TNumber rhs, TNumber epsilon)
    {
        auto abs_lhs = Math::Abs(lhs);
        auto abs_rhs = Math::Abs(rhs);

        return Math::Abs(lhs - rhs) <= ((abs_lhs < abs_rhs ? abs_rhs : abs_lhs) * epsilon);
    }

    template <typename TNumber>
    constexpr bool Math::EssentiallyEqual(TNumber lhs, TNumber rhs, TNumber epsilon)
    {
        auto abs_lhs = Math::Abs(lhs);
        auto abs_rhs = Math::Abs(rhs);

        return Math::Abs(lhs - rhs) <= ((abs_lhs > abs_rhs ? abs_rhs : abs_lhs) * epsilon);
    }

}
