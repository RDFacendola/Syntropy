/// \file hal_intrinsics.h
/// \brief This header is part of the Syntropy hardware abstraction module. It contains system intrinsics functions.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

namespace Syntropy
{
    /************************************************************************/
    /* INTRINSICS                                                           */
    /************************************************************************/

    /// \brief Exposes intrinsic functionalities.
    /// \author Raffaele D. Facendola - May 2020
    namespace HALIntrinsics
    {
        /// \brief Get the index of the most significant bit set or undefined if rhs is 0.
        Int GetMostSignificantBit(Int rhs);

        /// \brief Get the index of the least significant bit set or undefined if rhs is 0.
        Int GetLeastSignificantBit(Int rhs);

        /// \brief Get the inverse square root of a number.
        Float GetFastInvSqrt(Float rhs);
    };

}