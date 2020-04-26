
/// \file x64_endianness.h
/// \brief This header is part of the syntropy hardware abstraction layer. It contains methods to convert byte order.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#ifdef _M_X64

#include <cstdint>

namespace syntropy
{
    /************************************************************************/
    /* X64 ENDIANNESS                                                       */
    /************************************************************************/

    /// \brief Contains method used to convert values to and from little-endian, big-endian and platform-endian.
    /// \author Raffaele D. Facendola - November 2019.
    namespace x64Endianness
    {
        /// \brief Convert a number from platform-endian to big-endian.
        std::uint8_t ToBigEndian(std::uint8_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::uint16_t ToBigEndian(std::uint16_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::uint32_t ToBigEndian(std::uint32_t rhs);

        /// \brief Convert a number from platform-endian to big-endian.
        std::uint64_t ToBigEndian(std::uint64_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::uint8_t FromBigEndian(std::uint8_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::uint16_t FromBigEndian(std::uint16_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::uint32_t FromBigEndian(std::uint32_t rhs);

        /// \brief Convert a number from big-endian to platform-endian.
        std::uint64_t FromBigEndian(std::uint64_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::uint8_t ToLittleEndian(std::uint8_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::uint16_t ToLittleEndian(std::uint16_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::uint32_t ToLittleEndian(std::uint32_t rhs);

        /// \brief Convert a number from platform-endian to little-endian.
        std::uint64_t ToLittleEndian(std::uint64_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::uint8_t FromLittleEndian(std::uint8_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::uint16_t FromLittleEndian(std::uint16_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::uint32_t FromLittleEndian(std::uint32_t rhs);

        /// \brief Convert a number from little-endian to platform-endian.
        std::uint64_t FromLittleEndian(std::uint64_t rhs);

    }

    namespace PlatformEndianness = x64Endianness;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Endianness.

    inline std::uint8_t x64Endianness::ToBigEndian(std::uint8_t rhs)
    {
        return rhs;
    }

    inline std::uint8_t x64Endianness::FromBigEndian(std::uint8_t rhs)
    {
        return ToBigEndian(rhs);
    }

    inline std::uint16_t x64Endianness::FromBigEndian(std::uint16_t rhs)
    {
        return ToBigEndian(rhs);
    }

    inline std::uint32_t x64Endianness::FromBigEndian(std::uint32_t rhs)
    {
        return ToBigEndian(rhs);
    }

    inline std::uint64_t x64Endianness::FromBigEndian(std::uint64_t rhs)
    {
        return ToBigEndian(rhs);
    }

    inline std::uint8_t x64Endianness::ToLittleEndian(std::uint8_t rhs)
    {
        return rhs;
    }

    inline std::uint16_t x64Endianness::ToLittleEndian(std::uint16_t rhs)
    {
        return rhs;
    }

    inline std::uint32_t x64Endianness::ToLittleEndian(std::uint32_t rhs)
    {
        return rhs;
    }

    inline std::uint64_t x64Endianness::ToLittleEndian(std::uint64_t rhs)
    {
        return rhs;
    }

    inline std::uint8_t x64Endianness::FromLittleEndian(std::uint8_t rhs)
    {
        return rhs;
    }

    inline std::uint16_t x64Endianness::FromLittleEndian(std::uint16_t rhs)
    {
        return rhs;
    }

    inline std::uint32_t x64Endianness::FromLittleEndian(std::uint32_t rhs)
    {
        return rhs;
    }

    inline std::uint64_t x64Endianness::FromLittleEndian(std::uint64_t rhs)
    {
        return rhs;
    }
}

#endif