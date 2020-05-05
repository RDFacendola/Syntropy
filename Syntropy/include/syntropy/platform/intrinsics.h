/// \file intrinsics.h
/// \brief This header is part of the Syntropy platform module. It contains system intrinsics functions.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>

namespace syntropy
{
    /************************************************************************/
    /* INTRINSICS                                                           */
    /************************************************************************/

    /// \brief Exposes intrinsic functionalities.
    /// \author Raffaele D. Facendola - November 2017
    namespace Intrinsics
    {
        /// \brief Get the index of the most significant bit set or undefined if rhs is 0.
        std::int64_t GetMostSignificantBit(std::int64_t rhs);

        /// \brief Get the index of the least significant bit set or undefined if rhs is 0.
        std::int64_t GetLeastSignificantBit(std::int64_t rhs);

        /// \brief Get the inverse square root of a number.
        float GetFastInvSqrt(float rhs);
    };

}