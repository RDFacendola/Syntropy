#include "syntropy/platform/endianness.h"

#include "syntropy/hal/hal_endianness.h"

namespace syntropy
{
    /************************************************************************/
    /* ENDIANNESS                                                           */
    /************************************************************************/

    Fix16 Endianness::ToBigEndian(Fix16 rhs)
    {
        return HALEndianness::ToBigEndian(rhs);
    }

    Fix32 Endianness::ToBigEndian(Fix32 rhs)
    {
        return HALEndianness::ToBigEndian(rhs);
    }

    Fix64 Endianness::ToBigEndian(Fix64 rhs)
    {
        return HALEndianness::ToBigEndian(rhs);
    }

    Fix16 Endianness::FromBigEndian(Fix16 rhs)
    {
        return HALEndianness::FromBigEndian(rhs);
    }

    Fix32 Endianness::FromBigEndian(Fix32 rhs)
    {
        return HALEndianness::FromBigEndian(rhs);
    }

    Fix64 Endianness::FromBigEndian(Fix64 rhs)
    {
        return HALEndianness::FromBigEndian(rhs);
    }

    Fix16 Endianness::ToLittleEndian(Fix16 rhs)
    {
        return HALEndianness::ToLittleEndian(rhs);
    }

    Fix32 Endianness::ToLittleEndian(Fix32 rhs)
    {
        return HALEndianness::ToLittleEndian(rhs);
    }

    Fix64 Endianness::ToLittleEndian(Fix64 rhs)
    {
        return HALEndianness::ToLittleEndian(rhs);
    }

    Fix16 Endianness::FromLittleEndian(Fix16 rhs)
    {
        return HALEndianness::FromLittleEndian(rhs);
    }

    Fix32 Endianness::FromLittleEndian(Fix32 rhs)
    {
        return HALEndianness::FromLittleEndian(rhs);
    }

    Fix64 Endianness::FromLittleEndian(Fix64 rhs)
    {
        return HALEndianness::FromLittleEndian(rhs);
    }

}