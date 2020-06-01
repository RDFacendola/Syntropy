
/// \file endianness.h
/// \brief This header is part of the Syntropy platform module. It contains methods to convert byte order.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/core/types.h"

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
        Byte ToBigEndian(Byte rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::int16_t ToBigEndian(std::int16_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::int32_t ToBigEndian(std::int32_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        Int ToBigEndian(Int rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        Byte FromBigEndian(Byte rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int16_t FromBigEndian(std::int16_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::int32_t FromBigEndian(std::int32_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        Int FromBigEndian(Int rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        Byte ToLittleEndian(Byte rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int16_t ToLittleEndian(std::int16_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::int32_t ToLittleEndian(std::int32_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        Int ToLittleEndian(Int rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        Byte FromLittleEndian(Byte rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int16_t FromLittleEndian(std::int16_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::int32_t FromLittleEndian(std::int32_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        Int FromLittleEndian(Int rhs);

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Endianness.

    inline Byte Endianness::ToBigEndian(Byte rhs)
    {
        return rhs;
    }

    inline Byte Endianness::FromBigEndian(Byte rhs)
    {
        return rhs;
    }

    inline Byte Endianness::ToLittleEndian(Byte rhs)
    {
        return rhs;
    }

    inline Byte Endianness::FromLittleEndian(Byte rhs)
    {
        return rhs;
    }

}