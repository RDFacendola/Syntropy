#include "syntropy/platform/endianness.h"

#include "syntropy/platform/architecture/x64_endianness.h"

namespace syntropy
{
    /************************************************************************/
    /* ENDIANNESS                                                           */
    /************************************************************************/

    std::uint8_t Endianness::ToBigEndian(std::uint8_t rhs)
    {
        return PlatformEndianness::ToBigEndian(rhs);
    }

    std::uint16_t Endianness::ToBigEndian(std::uint16_t rhs)
    {
        return PlatformEndianness::ToBigEndian(rhs);
    }

    std::uint32_t Endianness::ToBigEndian(std::uint32_t rhs)
    {
        return PlatformEndianness::ToBigEndian(rhs);
    }

    std::uint64_t Endianness::ToBigEndian(std::uint64_t rhs)
    {
        return PlatformEndianness::ToBigEndian(rhs);
    }

    std::uint8_t Endianness::FromBigEndian(std::uint8_t rhs)
    {
        return PlatformEndianness::FromBigEndian(rhs);
    }

    std::uint16_t Endianness::FromBigEndian(std::uint16_t rhs)
    {
        return PlatformEndianness::FromBigEndian(rhs);
    }

    std::uint32_t Endianness::FromBigEndian(std::uint32_t rhs)
    {
        return PlatformEndianness::FromBigEndian(rhs);
    }

    std::uint64_t Endianness::FromBigEndian(std::uint64_t rhs)
    {
        return PlatformEndianness::FromBigEndian(rhs);
    }

    std::uint8_t Endianness::ToLittleEndian(std::uint8_t rhs)
    {
        return PlatformEndianness::ToLittleEndian(rhs);
    }

    std::uint16_t Endianness::ToLittleEndian(std::uint16_t rhs)
    {
        return PlatformEndianness::ToLittleEndian(rhs);
    }

    std::uint32_t Endianness::ToLittleEndian(std::uint32_t rhs)
    {
        return PlatformEndianness::ToLittleEndian(rhs);
    }

    std::uint64_t Endianness::ToLittleEndian(std::uint64_t rhs)
    {
        return PlatformEndianness::ToLittleEndian(rhs);
    }

    std::uint8_t Endianness::FromLittleEndian(std::uint8_t rhs)
    {
        return PlatformEndianness::FromLittleEndian(rhs);
    }

    std::uint16_t Endianness::FromLittleEndian(std::uint16_t rhs)
    {
        return PlatformEndianness::FromLittleEndian(rhs);
    }

    std::uint32_t Endianness::FromLittleEndian(std::uint32_t rhs)
    {
        return PlatformEndianness::FromLittleEndian(rhs);
    }

    std::uint64_t Endianness::FromLittleEndian(std::uint64_t rhs)
    {
        return PlatformEndianness::FromLittleEndian(rhs);
    }

}