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

    Int Endianness::ToBigEndian(Int rhs)
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

    Int Endianness::FromBigEndian(Int rhs)
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

    Int Endianness::ToLittleEndian(Int rhs)
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

    Int Endianness::FromLittleEndian(Int rhs)
    {
        return HALEndianness::FromLittleEndian(rhs);
    }

}