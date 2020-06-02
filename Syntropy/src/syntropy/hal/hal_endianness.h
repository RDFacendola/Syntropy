
/// \file hal_endianness.h
/// \brief This header is part of the Syntropy hardware abstraction layer module. It contains methods to convert byte order.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/core/types.h"

namespace syntropy
{
    /************************************************************************/
    /* HAL ENDIANNESS                                                       */
    /************************************************************************/

    /// \brief Contains methods used to convert values to and from little-endian, big-endian and platform-endian.
    /// \author Raffaele D. Facendola - November 2019.
    namespace HALEndianness
    {
        /// \brief Convert a number from platform-endian to big-endian.
        Fix16 ToBigEndian(Fix16 rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        Fix32 ToBigEndian(Fix32 rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        Fix64 ToBigEndian(Fix64 rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        Fix16 FromBigEndian(Fix16 rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        Fix32 FromBigEndian(Fix32 rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        Fix64 FromBigEndian(Fix64 rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        Fix16 ToLittleEndian(Fix16 rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        Fix32 ToLittleEndian(Fix32 rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        Fix64 ToLittleEndian(Fix64 rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        Fix16 FromLittleEndian(Fix16 rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        Fix32 FromLittleEndian(Fix32 rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        Fix64 FromLittleEndian(Fix64 rhs);
    }

}