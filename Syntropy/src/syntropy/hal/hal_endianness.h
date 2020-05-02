
/// \file hal_endianness.h
/// \brief This header is part of the Syntropy hardware abstraction module. It contains methods to convert byte order.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>

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
        std::int16_t ToBigEndian(std::int16_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::int32_t ToBigEndian(std::int32_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::int64_t ToBigEndian(std::int64_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int16_t FromBigEndian(std::int16_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int32_t FromBigEndian(std::int32_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int64_t FromBigEndian(std::int64_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int16_t ToLittleEndian(std::int16_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int32_t ToLittleEndian(std::int32_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int64_t ToLittleEndian(std::int64_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int16_t FromLittleEndian(std::int16_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int32_t FromLittleEndian(std::int32_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int64_t FromLittleEndian(std::int64_t rhs);
    }

}