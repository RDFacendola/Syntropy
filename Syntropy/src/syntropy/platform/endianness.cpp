#include "syntropy/platform/endianness.h"

#include "syntropy/hal/hal_endianness.h"

namespace syntropy
{
    /************************************************************************/
    /* ENDIANNESS                                                           */
    /************************************************************************/

    std::int16_t Endianness::ToBigEndian(std::int16_t rhs)
    {
        return HALEndianness::ToBigEndian(rhs);
    }

    std::int32_t Endianness::ToBigEndian(std::int32_t rhs)
    {
        return HALEndianness::ToBigEndian(rhs);
    }

    std::int64_t Endianness::ToBigEndian(std::int64_t rhs)
    {
        return HALEndianness::ToBigEndian(rhs);
    }

    std::int16_t Endianness::FromBigEndian(std::int16_t rhs)
    {
        return HALEndianness::FromBigEndian(rhs);
    }

    std::int32_t Endianness::FromBigEndian(std::int32_t rhs)
    {
        return HALEndianness::FromBigEndian(rhs);
    }

    std::int64_t Endianness::FromBigEndian(std::int64_t rhs)
    {
        return HALEndianness::FromBigEndian(rhs);
    }

    std::int16_t Endianness::ToLittleEndian(std::int16_t rhs)
    {
        return HALEndianness::ToLittleEndian(rhs);
    }

    std::int32_t Endianness::ToLittleEndian(std::int32_t rhs)
    {
        return HALEndianness::ToLittleEndian(rhs);
    }

    std::int64_t Endianness::ToLittleEndian(std::int64_t rhs)
    {
        return HALEndianness::ToLittleEndian(rhs);
    }

    std::int16_t Endianness::FromLittleEndian(std::int16_t rhs)
    {
        return HALEndianness::FromLittleEndian(rhs);
    }

    std::int32_t Endianness::FromLittleEndian(std::int32_t rhs)
    {
        return HALEndianness::FromLittleEndian(rhs);
    }

    std::int64_t Endianness::FromLittleEndian(std::int64_t rhs)
    {
        return HALEndianness::FromLittleEndian(rhs);
    }

}