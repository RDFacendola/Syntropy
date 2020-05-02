
/// \file endianness.h
/// \brief This header is part of the Syntropy platform module. It contains methods to convert byte order.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>

namespace syntropy
{
    /************************************************************************/
    /* ENDIANNESS                                                           */
    /************************************************************************/

    /// \brief Contains methods used to convert values to and from little-endian, big-endian and platform-endian.
    /// \author Raffaele D. Facendola - November 2019.
    namespace Endianness
    {
        /// \brief Convert a number from platform-endian to big-endian.
        std::int8_t ToBigEndian(std::int8_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::int16_t ToBigEndian(std::int16_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::int32_t ToBigEndian(std::int32_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::int64_t ToBigEndian(std::int64_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int8_t FromBigEndian(std::int8_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int16_t FromBigEndian(std::int16_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int32_t FromBigEndian(std::int32_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int64_t FromBigEndian(std::int64_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int8_t ToLittleEndian(std::int8_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int16_t ToLittleEndian(std::int16_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int32_t ToLittleEndian(std::int32_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int64_t ToLittleEndian(std::int64_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int8_t FromLittleEndian(std::int8_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int16_t FromLittleEndian(std::int16_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int32_t FromLittleEndian(std::int32_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int64_t FromLittleEndian(std::int64_t rhs);

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Endianness.

    inline std::int8_t Endianness::ToBigEndian(std::int8_t rhs)
    {
        return rhs;
    }

    inline std::int8_t Endianness::FromBigEndian(std::int8_t rhs)
    {
        return rhs;
    }

    inline std::int8_t Endianness::ToLittleEndian(std::int8_t rhs)
    {
        return rhs;
    }

    inline std::int8_t Endianness::FromLittleEndian(std::int8_t rhs)
    {
        return rhs;
    }

}