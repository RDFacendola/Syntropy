#ifdef _M_X64

#include "syntropy/hal/hal_endianness.h"

namespace syntropy
{
    /************************************************************************/
    /* HAL ENDIANNESS                                                       */
    /************************************************************************/

    std::int16_t HALEndianness::ToBigEndian(std::int16_t rhs)
    {
        auto b0 = (rhs & 0xFF00) >> 8u;
        auto b1 = (rhs & 0x00FF) >> 0u;

        return std::int16_t((b1 << 8u) | (b0 << 0u));
    }

    std::int32_t HALEndianness::ToBigEndian(std::int32_t rhs)
    {
        auto b0 = (rhs & 0xFF000000) >> 24;
        auto b1 = (rhs & 0x00FF0000) >> 16;
        auto b2 = (rhs & 0x0000FF00) >> 8;
        auto b3 = (rhs & 0x000000FF) >> 0;

        return std::int32_t((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0 << 0));
    }

    std::int64_t HALEndianness::ToBigEndian(std::int64_t rhs)
    {
        auto b0 = (rhs & 0xFF00000000000000ll) >> 56;
        auto b1 = (rhs & 0x00FF000000000000ll) >> 48;
        auto b2 = (rhs & 0x0000FF0000000000ll) >> 40;
        auto b3 = (rhs & 0x000000FF00000000ll) >> 32;
        auto b4 = (rhs & 0x00000000FF000000ll) >> 24;
        auto b5 = (rhs & 0x0000000000FF0000ll) >> 16;
        auto b6 = (rhs & 0x000000000000FF00ll) >> 8;
        auto b7 = (rhs & 0x00000000000000FFll) >> 0;

        return std::int64_t((b7 << 56) | (b6 << 48) | (b5 << 40) | (b4 << 32) | (b3 << 24) | (b2 << 16) | (b1 << 8) | (b0 << 0));
    }

    std::int16_t HALEndianness::FromBigEndian(std::int16_t rhs)
    {
        return ToBigEndian(rhs);
    }

    std::int32_t HALEndianness::FromBigEndian(std::int32_t rhs)
    {
        return ToBigEndian(rhs);
    }

    std::int64_t HALEndianness::FromBigEndian(std::int64_t rhs)
    {
        return ToBigEndian(rhs);
    }

    std::int16_t HALEndianness::ToLittleEndian(std::int16_t rhs)
    {
        return rhs;
    }

    std::int32_t HALEndianness::ToLittleEndian(std::int32_t rhs)
    {
        return rhs;
    }

    std::int64_t HALEndianness::ToLittleEndian(std::int64_t rhs)
    {
        return rhs;
    }

    std::int16_t HALEndianness::FromLittleEndian(std::int16_t rhs)
    {
        return rhs;
    }

    std::int32_t HALEndianness::FromLittleEndian(std::int32_t rhs)
    {
        return rhs;
    }

    std::int64_t HALEndianness::FromLittleEndian(std::int64_t rhs)
    {
        return rhs;
    }
}

#endif