#include "syntropy/platform/architecture/x64_endianness.h"

#ifdef _M_X64

#include <algorithm>

namespace syntropy
{
    /************************************************************************/
    /* X64 ENDIANNESS                                                       */
    /************************************************************************/

    std::uint16_t x64Endianness::ToBigEndian(std::uint16_t rhs)
    {
        auto b0 = (rhs & 0xFF00) >> 8u;
        auto b1 = (rhs & 0x00FF) >> 0u;

        return std::uint16_t((b1 << 8u) | (b0 << 0u));
    }

    std::uint32_t x64Endianness::ToBigEndian(std::uint32_t rhs)
    {
        auto b0 = (rhs & 0xFF000000) >> 24u;
        auto b1 = (rhs & 0x00FF0000) >> 16u;
        auto b2 = (rhs & 0x0000FF00) >> 8u;
        auto b3 = (rhs & 0x000000FF) >> 0u;

        return std::uint32_t((b3 << 24u) | (b2 << 16u) | (b1 << 8u) | (b0 << 0u));
    }

    std::uint64_t x64Endianness::ToBigEndian(std::uint64_t rhs)
    {
        auto b0 = (rhs & 0xFF00000000000000) >> 56u;
        auto b1 = (rhs & 0x00FF000000000000) >> 48u;
        auto b2 = (rhs & 0x0000FF0000000000) >> 40;
        auto b3 = (rhs & 0x000000FF00000000) >> 32u;
        auto b4 = (rhs & 0x00000000FF000000) >> 24u;
        auto b5 = (rhs & 0x0000000000FF0000) >> 16u;
        auto b6 = (rhs & 0x000000000000FF00) >> 8u;
        auto b7 = (rhs & 0x00000000000000FF) >> 0u;

        return std::uint64_t((b7 << 56u) | (b6 << 48u) | (b5 << 40u) | (b4 << 32u) | (b3 << 24u) | (b2 << 16u) | (b1 << 8u) | (b0 << 0u));
    }

}

#endif