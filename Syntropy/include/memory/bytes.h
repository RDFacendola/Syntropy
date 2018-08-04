
/// \file bytes.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of the Bytes type and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <cstddef>
#include <ostream>
#include <algorithm>

#include "diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* BYTES                                                                */
    /************************************************************************/

    /// \brief Represents a number of bytes.
    /// This type is meant to be a strongly-typed replacement for std::size_t.
    /// The amount shall never reach a negative amount.
    /// \author Raffaele D. Facendola - July 2018
    class Bytes
    {
    public:

        static constexpr std::size_t kByte = 0x1;                           ///< \brief Number of Bytes in a Byte.

        static constexpr std::size_t kKibiByte = kByte << 10;               ///< \brief Number of Bytes in a KibiByte.

        static constexpr std::size_t kMebiByte = kKibiByte << 10;           ///< \brief Number of Bytes in a MebiByte.

        static constexpr std::size_t kGibiByte = kMebiByte << 10;           ///< \brief Number of Bytes in a GibiByte.

        static constexpr std::size_t kTebiByte = kGibiByte << 10;           ///< \brief Number of Bytes in a TebiByte.

        static constexpr std::size_t kKiloByte = kByte * 1000;              ///< \brief Number of Bytes in a KiloByte.

        static constexpr std::size_t kMegaByte = kKiloByte * 1000;          ///< \brief Number of Bytes in a MegaByte.

        static constexpr std::size_t kGigaByte = kMegaByte * 1000;          ///< \brief Number of Bytes in a GigaByte.

        static constexpr std::size_t kTeraByte = kGigaByte * 1000;          ///< \brief Number of Bytes in a TeraByte.

        /// \brief Create a new bytes count equal to zero.
        constexpr Bytes() = default;

        /// \brief Create a new bytes count.
        constexpr explicit Bytes(std::size_t bytes);

        /// \brief Default copy-constructor.
        constexpr Bytes(const Bytes&) = default;

        /// \brief Default assignment operator.
        Bytes& operator=(const Bytes&) = default;

        /// \brief Get the number of bytes.
        /// \return Returns the number of bytes.
        constexpr explicit operator std::size_t() const;

        /// \brief Add a bytes amount.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator+=(const Bytes& rhs) noexcept;

        /// \brief Subtract a bytes amount.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator-=(const Bytes& rhs) noexcept;

        /// \brief Multiply this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator*=(std::size_t rhs) noexcept;

        /// \brief Divides this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator/=(std::size_t rhs) noexcept;

        /// \brief Right shift operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator>>=(std::size_t rhs) noexcept;

        /// \brief Left shift operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator<<=(std::size_t rhs) noexcept;

        /// \brief Bitwise-and operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator&=(const Bytes& rhs) noexcept;

        /// \brief Bitwise-or operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator|=(const Bytes& rhs) noexcept;

        /// \brief Bitwise-xor operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator^=(const Bytes& rhs) noexcept;

    private:

        std::size_t bytes_ = 0;                 ///< \brief Number of bytes.

    };

    /// \brief Equality comparison for Bytes.
    /// \return Returns true if lhs and rhs refer to the same amount of memory, returns false otherwise.
    constexpr bool operator==(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Inequality comparison for Bytes.
    /// \return Returns true if lhs and rhs refer to different amounts of memory, returns false otherwise.
    constexpr bool operator!=(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Greater-than comparison for Bytes.
    /// \return Returns true if lhs refers to a memory amount that is strictly greater than rhs, returns false otherwise.
    constexpr bool operator>(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Less-than comparison for Bytes.
    /// \return Returns true if lhs refers to a memory amount that is strictly less than rhs, returns false otherwise.
    constexpr bool operator<(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Greater-or-equal comparison for Bytes.
    /// \return Returns true if lhs refers to a memory amount that is equal or greater than rhs, returns false otherwise.
    constexpr bool operator>=(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Less-or-equal comparison for Bytes.
    /// \return Returns true if lhs refers to a memory amount that is equal or less than rhs, returns false otherwise.
    constexpr bool operator<=(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Sum two bytes amount.
    /// \return Returns a memory amount which is the sum of lhs and rhs.
    constexpr Bytes operator+(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Subtract two bytes amount.
    /// \return Returns a memory amount which is the difference of lhs and rhs.
    constexpr Bytes operator-(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs times rhs.
    constexpr Bytes operator*(const Bytes& lhs, std::size_t rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to rhs times lhs.
    constexpr Bytes operator*(std::size_t lhs, const Bytes& rhs) noexcept;

    /// \brief Divide a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr Bytes operator/(const Bytes& lhs, std::size_t rhs) noexcept;

    /// \brief Right-shift a bytes amount.
    /// \return Returns a memory amount which is equal to lhs right-shifted by rhs.
    constexpr Bytes operator>>(const Bytes& lhs, std::size_t rhs) noexcept;

    /// \brief Left-shift a bytes amount.
    /// \return Returns a memory amount which is equal to lhs left-shifted by rhs.
    constexpr Bytes operator<<(const Bytes& lhs, std::size_t rhs) noexcept;

    /// \brief Bitwise not of a byte quantity.
    /// \return Returns a memory amount which is the bitwise not of rhs.
    constexpr Bytes operator!(const Bytes& rhs) noexcept;

    /// \brief Bitwise and between two bytes quantities.
    /// \return Returns a memory amount which is equal to the bitwise and between lhs and rhs.
    constexpr Bytes operator&(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Bitwise or between two bytes quantities.
    /// \return Returns a memory amount which is equal to the bitwise or between lhs and rhs.
    constexpr Bytes operator|(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Bitwise xor between two bytes quantities.
    /// \return Returns a memory amount which is equal to the bitwise xor between lhs and rhs.
    constexpr Bytes operator^(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Stream insertion for Bytes.
    std::ostream& operator<<(std::ostream& lhs, const Bytes& rhs);

    /// \brief User-defined literal used to convert a number from Bytes to Bytes.
    /// This method is only used for clarity: Foo(78_Bytes) is better than Foo(78).
    /// \param number Number to convert.
    constexpr Bytes operator "" _Bytes(std::size_t lhs);

    /// \brief User-defined literal used to convert a number from KibiBytes to Bytes.
    /// \param number Number to convert.
    constexpr Bytes operator "" _KiBytes(std::size_t lhs);

    /// \brief User-defined literal used to convert a number from MebiBytes to Bytes.
    /// \param number Number to convert.
    constexpr Bytes operator "" _MiBytes(std::size_t lhs);

    /// \brief User-defined literal used to convert a number from GibiBytes to Bytes.
    /// \param number Number to convert.
    constexpr Bytes operator "" _GiBytes(std::size_t lhs);

    /// \brief User-defined literal used to convert a number from TebiBytes to Bytes.
    /// \param number Number to convert.
    constexpr Bytes operator "" _TiBytes(std::size_t lhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr Bytes::Bytes(std::size_t bytes)
        : bytes_(bytes)
    {

    }

    constexpr Bytes::operator std::size_t() const
    {
        return bytes_;
    }

    constexpr Bytes& Bytes::operator+=(const Bytes& rhs) noexcept
    {
        bytes_ += rhs.bytes_;
        return *this;
    }

    constexpr Bytes& Bytes::operator-=(const Bytes& rhs) noexcept
    {
        SYNTROPY_ASSERT(*this >= rhs);

        bytes_ -= rhs.bytes_;
        return *this;
    }

    constexpr Bytes& Bytes::operator*=(std::size_t rhs) noexcept
    {
        bytes_ *= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator/=(std::size_t rhs) noexcept
    {
        bytes_ /= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator>>=(std::size_t rhs) noexcept
    {
        bytes_ >>= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator<<=(std::size_t rhs) noexcept
    {
        bytes_ <<= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator&=(const Bytes& rhs) noexcept
    {
        bytes_ &= rhs.bytes_;
        return *this;
    }

    constexpr Bytes& Bytes::operator|=(const Bytes& rhs) noexcept
    {
        bytes_ |= rhs.bytes_;
        return *this;
    }

    constexpr Bytes& Bytes::operator^=(const Bytes& rhs) noexcept
    {
        bytes_ ^= rhs.bytes_;
        return *this;
    }

    constexpr bool operator==(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return std::size_t(lhs) == std::size_t(rhs);
    }

    constexpr bool operator!=(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return std::size_t(lhs) > std::size_t(rhs);
    }

    constexpr bool operator<(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return std::size_t(lhs) < std::size_t(rhs);
    }

    constexpr bool operator>=(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return std::size_t(lhs) >= std::size_t(rhs);
    }

    constexpr bool operator<=(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return std::size_t(lhs) <= std::size_t(rhs);
    }

    constexpr Bytes operator+(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return Bytes(lhs) += rhs;
    }

    constexpr Bytes operator-(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return Bytes(lhs) -= rhs;
    }

    constexpr Bytes operator*(const Bytes& lhs, std::size_t rhs) noexcept
    {
        return Bytes(lhs) *= rhs;
    }

    constexpr Bytes operator*(std::size_t lhs, const Bytes& rhs) noexcept
    {
        return Bytes(rhs) *= lhs;
    }

    constexpr Bytes operator/(const Bytes& lhs, std::size_t rhs) noexcept
    {
        return Bytes(lhs) /= rhs;
    }

    constexpr std::size_t operator/(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return std::size_t(lhs) / std::size_t(rhs);
    }

    constexpr Bytes operator>>(const Bytes& lhs, std::size_t rhs) noexcept
    {
        return Bytes(lhs) >>= rhs;
    }

    constexpr Bytes operator<<(const Bytes& lhs, std::size_t rhs) noexcept
    {
        return Bytes(lhs) <<= rhs;
    }

    constexpr Bytes operator!(const Bytes& rhs) noexcept
    {
        return Bytes(!std::size_t(rhs));
    }

    constexpr Bytes operator&(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return Bytes(lhs) &= rhs;
    }

    constexpr Bytes operator|(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return Bytes(lhs) |= rhs;
    }

    constexpr Bytes operator^(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return Bytes(lhs) ^= rhs;
    }

    inline std::ostream& operator<<(std::ostream& lhs, const Bytes& rhs)
    {
        return lhs << std::size_t(rhs);
    }

    constexpr Bytes operator "" _Bytes(std::size_t lhs)
    {
        return Bytes(lhs * Bytes::kByte);
    }

    constexpr Bytes operator "" _KiBytes(std::size_t lhs)
    {
        return Bytes(lhs * Bytes::kKibiByte);
    }

    constexpr Bytes operator "" _MiBytes(std::size_t lhs)
    {
        return Bytes(lhs * Bytes::kMebiByte);
    }

    constexpr Bytes operator "" _GiBytes(std::size_t lhs)
    {
        return Bytes(lhs * Bytes::kGibiByte);
    }

    constexpr Bytes operator "" _TiBytes(std::size_t lhs)
    {
        return Bytes(lhs * Bytes::kTebiByte);
    }
}

