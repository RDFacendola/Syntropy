
/// \file constants.h
/// \brief This header is part of the Syntropy math module. It contains math constants.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/core/types.h"

namespace syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes mathematical constants.
    /// \author Raffaele D. Facendola - 2019.
    namespace Math
    {
        /// \brief Archimedes' constant.
        constexpr Float kPi = 3.14159f;

        /// \brief Two times the Archimedes' constant.
        constexpr Float kTau = 2.0f * kPi;

        /// \brief Euler's number.
        constexpr Float kE = 2.71828f;

        /// \brief Golden ratio.
        constexpr Float kPhi = 1.61803f;
    }

}