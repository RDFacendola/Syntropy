#ifdef _M_X64

#include "syntropy/hal/hal_endianness.h"

namespace Syntropy
{
    /************************************************************************/
    /* HAL ENDIANNESS                                                       */
    /************************************************************************/

    Fix16 HALEndianness::ToBigEndian(Fix16 rhs)
    {
        auto int_rhs = ToInt(rhs);

        auto b0 = (int_rhs & 0xFF00) >> 8u;
        auto b1 = (int_rhs & 0x00FF) >> 0u;

        return ToFix16((b1 << 8u) | (b0 << 0u));
    }

    Fix32 HALEndianness::ToBigEndian(Fix32 rhs)
    {
        auto int_rhs = ToInt(rhs);

        auto b0 = (int_rhs & 0xFF000000) >> 24;
        auto b1 = (int_rhs & 0x00FF0000) >> 16;
        auto b2 = (int_rhs & 0x0000FF00) >> 8;
        auto b3 = (int_rhs & 0x000000FF) >> 0;

        return ToFix32((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0 << 0));
    }

    Fix64 HALEndianness::ToBigEndian(Fix64 rhs)
    {
        auto int_rhs = ToInt(rhs);

        auto b0 = (int_rhs & 0xFF00000000000000ll) >> 56;
        auto b1 = (int_rhs & 0x00FF000000000000ll) >> 48;
        auto b2 = (int_rhs & 0x0000FF0000000000ll) >> 40;
        auto b3 = (int_rhs & 0x000000FF00000000ll) >> 32;
        auto b4 = (int_rhs & 0x00000000FF000000ll) >> 24;
        auto b5 = (int_rhs & 0x0000000000FF0000ll) >> 16;
        auto b6 = (int_rhs & 0x000000000000FF00ll) >> 8;
        auto b7 = (int_rhs & 0x00000000000000FFll) >> 0;

        return ToFix64((b7 << 56) | (b6 << 48) | (b5 << 40) | (b4 << 32) | (b3 << 24) | (b2 << 16) | (b1 << 8) | (b0 << 0));
    }

    Fix16 HALEndianness::FromBigEndian(Fix16 rhs)
    {
        return ToBigEndian(rhs);
    }

    Fix32 HALEndianness::FromBigEndian(Fix32 rhs)
    {
        return ToBigEndian(rhs);
    }

    Fix64 HALEndianness::FromBigEndian(Fix64 rhs)
    {
        return ToBigEndian(rhs);
    }

    Fix16 HALEndianness::ToLittleEndian(Fix16 rhs)
    {
        return rhs;
    }

    Fix32 HALEndianness::ToLittleEndian(Fix32 rhs)
    {
        return rhs;
    }

    Fix64 HALEndianness::ToLittleEndian(Fix64 rhs)
    {
        return rhs;
    }

    Fix16 HALEndianness::FromLittleEndian(Fix16 rhs)
    {
        return rhs;
    }

    Fix32 HALEndianness::FromLittleEndian(Fix32 rhs)
    {
        return rhs;
    }

    Fix64 HALEndianness::FromLittleEndian(Fix64 rhs)
    {
        return rhs;
    }
}

#endif