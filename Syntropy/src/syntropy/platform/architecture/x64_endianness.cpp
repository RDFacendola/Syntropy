#include "syntropy/platform/architecture/x64_endianness.h"

#ifdef _M_X64

#include <algorithm>

namespace syntropy
{
    /************************************************************************/
    /* X64 ENDIANNESS                                                       */
    /************************************************************************/

    /// \brief Utility used to convert endianness.
    union EndianBytes
    {
        /// \brief Value to encode\decode.
        std::uint64_t value_;

        /// \brief Value bytes.
        std::uint8_t bytes_[8];
    };

    std::uint16_t x64Endianness::ToBigEndian(std::uint16_t rhs)
    {
        auto bytes = EndianBytes{rhs};

        std::swap(bytes.bytes_[0], bytes.bytes_[1]);

        return std::uint16_t(bytes.value_);
    }

    std::uint32_t x64Endianness::ToBigEndian(std::uint32_t rhs)
    {
        auto bytes = EndianBytes{ rhs };

        std::swap(bytes.bytes_[0], bytes.bytes_[3]);
        std::swap(bytes.bytes_[1], bytes.bytes_[2]);

        return std::uint32_t(bytes.value_);
    }

    std::uint64_t x64Endianness::ToBigEndian(std::uint64_t rhs)
    {
        auto bytes = EndianBytes{ rhs };

        std::swap(bytes.bytes_[0], bytes.bytes_[7]);
        std::swap(bytes.bytes_[1], bytes.bytes_[6]);
        std::swap(bytes.bytes_[2], bytes.bytes_[5]);
        std::swap(bytes.bytes_[3], bytes.bytes_[4]);

        return bytes.value_;
    }

}

#endif