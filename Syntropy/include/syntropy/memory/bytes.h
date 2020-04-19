
/// \file bytes.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of memory unit types and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <cstddef>
#include <ostream>

#include "syntropy/diagnostics/assert.h"
#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* BYTES                                                                */
    /************************************************************************/

    /// \brief Represents a number of bytes.
    /// This type is meant to be a strongly-typed replacement for std::int64_t.
    /// \author Raffaele D. Facendola - July 2018
    class Bytes
    {
    public:

        ///< \brief Number of Bytes in a Byte.
        static constexpr std::int64_t kByte = 0x1;

        ///< \brief Number of Bytes in a KibiByte.
        static constexpr std::int64_t kKibiByte = kByte * 1024;

        ///< \brief Number of Bytes in a MebiByte.
        static constexpr std::int64_t kMebiByte = kKibiByte * 1024;

        ///< \brief Number of Bytes in a GibiByte.
        static constexpr std::int64_t kGibiByte = kMebiByte * 1024;

        ///< \brief Number of Bytes in a TebiByte.
        static constexpr std::int64_t kTebiByte = kGibiByte * 1024;

        ///< \brief Number of Bytes in a KiloByte.
        static constexpr std::int64_t kKiloByte = kByte * 1000;

        ///< \brief Number of Bytes in a MegaByte.
        static constexpr std::int64_t kMegaByte = kKiloByte * 1000;

        ///< \brief Number of Bytes in a GigaByte.
        static constexpr std::int64_t kGigaByte = kMegaByte * 1000;

        ///< \brief Number of Bytes in a TeraByte.
        static constexpr std::int64_t kTeraByte = kGigaByte * 1000;

        /// \brief Create a new bytes count equal to zero.
        constexpr Bytes() = default;

        /// \brief Create a new bytes count.
        constexpr explicit Bytes(std::int64_t bytes);

        /// \brief Default copy-constructor.
        constexpr Bytes(const Bytes&) = default;

        /// \brief Default assignment operator.
        Bytes& operator=(const Bytes&) = default;

        /// \brief Get the number of bytes.
        /// \return Returns the number of bytes.
        constexpr std::int64_t operator*() const;

        /// \brief Add a bytes amount.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator+=(const Bytes& rhs) noexcept;

        /// \brief Subtract a bytes amount.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator-=(const Bytes& rhs) noexcept;

        /// \brief Multiply this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator*=(std::int64_t rhs) noexcept;

        /// \brief Divides this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator/=(std::int64_t rhs) noexcept;

        /// \brief Division remainder of this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator%=(std::int64_t rhs) noexcept;

        /// \brief Right shift operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator>>=(std::int64_t rhs) noexcept;

        /// \brief Left shift operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator<<=(std::int64_t rhs) noexcept;

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

        ///< \brief Number of bytes.
        std::int64_t bytes_ = 0u;

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
    constexpr Bytes operator*(const Bytes& lhs, std::int64_t rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to rhs times lhs.
    constexpr Bytes operator*(std::int64_t lhs, const Bytes& rhs) noexcept;

    /// \brief Divide a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr Bytes operator/(const Bytes& lhs, std::int64_t rhs) noexcept;

    /// \brief Divide a byte amount by another byte amount.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr std::int64_t operator/(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by a number.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    constexpr Bytes operator%(const Bytes& lhs, std::int64_t rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by another byte amount.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    constexpr std::int64_t operator%(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Right-shift a bytes amount.
    /// \return Returns a memory amount which is equal to lhs right-shifted by rhs.
    constexpr Bytes operator>>(const Bytes& lhs, std::int64_t rhs) noexcept;

    /// \brief Left-shift a bytes amount.
    /// \return Returns a memory amount which is equal to lhs left-shifted by rhs.
    constexpr Bytes operator<<(const Bytes& lhs, std::int64_t rhs) noexcept;

    /// \brief Bitwise not of a byte quantity.
    /// \return Returns a memory amount which is the bitwise not of rhs.
    constexpr Bytes operator~(const Bytes& rhs) noexcept;

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

    /// \brief Get the size of rhs, in bytes.
    template <typename TType>
    constexpr Bytes BytesOf(const TType& rhs);

    /// \brief Get the size of TType, in bytes.
    template <typename TType>
    constexpr Bytes BytesOf();

    /************************************************************************/
    /* BITS                                                                 */
    /************************************************************************/

     /// \brief Represents a number of bits.
     /// This type is meant to be a strongly-typed replacement for std::int64_t.
     /// The amount shall never reach a negative amount.
     /// \author Raffaele D. Facendola - June 2019
    class Bits
    {
    public:

        /// \brief Number of bits per Byte.
        static constexpr std::int64_t kByte = 0x8;

        /// \brief Create a new bits count equal to zero.
        constexpr Bits() = default;

        /// \brief Create a new bits count.
        constexpr explicit Bits(std::int64_t bits);

        /// \brief Create a new bits count from a byte amount.
        constexpr Bits(const Bytes& bytes);

        /// \brief Default copy-constructor.
        constexpr Bits(const Bits&) = default;

        /// \brief Default assignment operator.
        Bits& operator=(const Bits&) = default;

        /// \brief Get the number of bits.
        /// \return Returns the number of bits.
        constexpr std::int64_t operator*() const;

        /// \brief Add a bits amount.
        /// \return Returns a reference to this element.
        constexpr Bits& operator+=(const Bits& rhs) noexcept;

        /// \brief Subtract a bits amount.
        /// \return Returns a reference to this element.
        constexpr Bits& operator-=(const Bits& rhs) noexcept;

        /// \brief Multiply this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bits& operator*=(std::int64_t rhs) noexcept;

        /// \brief Divides this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bits& operator/=(std::int64_t rhs) noexcept;

        /// \brief Modulus operator between this element and a number.
        /// \return Returns a reference to this element.
        constexpr Bits& operator%=(std::int64_t rhs) noexcept;

        /// \brief Right shift operator.
        /// \return Returns a reference to this element.
        constexpr Bits& operator>>=(std::int64_t rhs) noexcept;

        /// \brief Left shift operator.
        /// \return Returns a reference to this element.
        constexpr Bits& operator<<=(std::int64_t rhs) noexcept;

        /// \brief Bitwise-and operator.
        /// \return Returns a reference to this element.
        constexpr Bits& operator&=(const Bits& rhs) noexcept;

        /// \brief Bitwise-or operator.
        /// \return Returns a reference to this element.
        constexpr Bits& operator|=(const Bits& rhs) noexcept;

        /// \brief Bitwise-xor operator.
        /// \return Returns a reference to this element.
        constexpr Bits& operator^=(const Bits& rhs) noexcept;

    private:

        /// \brief Number of bits.
        std::int64_t bits_ = 0u;

    };

    /// \brief Equality comparison for Bits.
    /// \return Returns true if lhs and rhs refer to the same amount of memory, returns false otherwise.
    constexpr bool operator==(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Inequality comparison for Bits.
    /// \return Returns true if lhs and rhs refer to different amounts of memory, returns false otherwise.
    constexpr bool operator!=(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Greater-than comparison for Bits.
    /// \return Returns true if lhs refers to a memory amount that is strictly greater than rhs, returns false otherwise.
    constexpr bool operator>(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Less-than comparison for Bits.
    /// \return Returns true if lhs refers to a memory amount that is strictly less than rhs, returns false otherwise.
    constexpr bool operator<(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Greater-or-equal comparison for Bits.
    /// \return Returns true if lhs refers to a memory amount that is equal or greater than rhs, returns false otherwise.
    constexpr bool operator>=(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Less-or-equal comparison for Bits.
    /// \return Returns true if lhs refers to a memory amount that is equal or less than rhs, returns false otherwise.
    constexpr bool operator<=(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Sum two bits amount.
    /// \return Returns a memory amount which is the sum of lhs and rhs.
    constexpr Bits operator+(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Subtract two bits amount.
    /// \return Returns a memory amount which is the difference of lhs and rhs.
    constexpr Bits operator-(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Multiply a bits amount by a number.
    /// \return Returns a memory amount which is equal to lhs times rhs.
    constexpr Bits operator*(const Bits& lhs, std::int64_t rhs) noexcept;

    /// \brief Multiply a bits amount by a number.
    /// \return Returns a memory amount which is equal to rhs times lhs.
    constexpr Bits operator*(std::int64_t lhs, const Bits& rhs) noexcept;

    /// \brief Divide a bits amount by a number.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr Bits operator/(const Bits& lhs, std::int64_t rhs) noexcept;

    /// \brief Divide a bits amount by another bit amount.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr std::int64_t operator/(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Get the remainder of a bits amount divided by a number.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    constexpr Bits operator%(const Bits& lhs, std::int64_t rhs) noexcept;

    /// \brief Get the remainder of a bits amount divided by another bits amount.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    constexpr std::int64_t operator%(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Right-shift a bits amount.
    /// \return Returns a memory amount which is equal to lhs right-shifted by rhs.
    constexpr Bits operator>>(const Bits& lhs, std::int64_t rhs) noexcept;

    /// \brief Left-shift a bits amount.
    /// \return Returns a memory amount which is equal to lhs left-shifted by rhs.
    constexpr Bits operator<<(const Bits& lhs, std::int64_t rhs) noexcept;

    /// \brief Bitwise not of a bits quantity.
    /// \return Returns a memory amount which is the bitwise not of rhs.
    constexpr Bits operator~(const Bits& rhs) noexcept;

    /// \brief Bitwise and between two bits quantities.
    /// \return Returns a memory amount which is equal to the bitwise and between lhs and rhs.
    constexpr Bits operator&(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Bitwise or between two bits quantities.
    /// \return Returns a memory amount which is equal to the bitwise or between lhs and rhs.
    constexpr Bits operator|(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Bitwise xor between two bits quantities.
    /// \return Returns a memory amount which is equal to the bitwise xor between lhs and rhs.
    constexpr Bits operator^(const Bits& lhs, const Bits& rhs) noexcept;

    /// \brief Stream insertion for Bits.
    std::ostream& operator<<(std::ostream& lhs, const Bits& rhs);

    /// \brief User-defined literal used to convert a number to Bits.
    /// \param number Number to convert.
    constexpr Bits operator "" _Bits(std::size_t lhs);

    /// \brief Get the number of bytes in an amount of bits, rounded up.
    /// \param rhs Amount of bits to convert.
    /// \return Returns the number of bytes in the provided amount of bits, rounded up.
    constexpr syntropy::Bytes ToBytesCeil(Bits rhs);

    /// \brief Get the number of bytes in an amount of bits, rounded down.
    /// \param rhs Amount of bits to convert.
    /// \return Returns the number of bytes in the provided amount of bits, rounded down.
    constexpr syntropy::Bytes ToBytesFloor(Bits rhs);

    /// \brief Get the size of rhs, in bits.
    template <typename TType>
    constexpr Bits BitsOf(const TType& rhs);

    /// \brief Get the size of TType, in bits.
    template <typename TType>
    constexpr Bits BitsOf();

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Bytes.

    constexpr Bytes::Bytes(std::int64_t bytes)
        : bytes_(bytes)
    {

    }

    constexpr std::int64_t Bytes::operator*() const
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
        bytes_ -= rhs.bytes_;
        return *this;
    }

    constexpr Bytes& Bytes::operator*=(std::int64_t rhs) noexcept
    {
        bytes_ *= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator/=(std::int64_t rhs) noexcept
    {
        bytes_ /= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator%=(std::int64_t rhs) noexcept
    {
        bytes_ %= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator>>=(std::int64_t rhs) noexcept
    {
        bytes_ >>= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator<<=(std::int64_t rhs) noexcept
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
        return *lhs == *rhs;
    }

    constexpr bool operator!=(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs > *rhs;
    }

    constexpr bool operator<(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs < *rhs;
    }

    constexpr bool operator>=(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs >= *rhs;
    }

    constexpr bool operator<=(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs <= *rhs;
    }

    constexpr Bytes operator+(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return Bytes(lhs) += rhs;
    }

    constexpr Bytes operator-(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return Bytes(lhs) -= rhs;
    }

    constexpr Bytes operator*(const Bytes& lhs, std::int64_t rhs) noexcept
    {
        return Bytes(lhs) *= rhs;
    }

    constexpr Bytes operator*(std::int64_t lhs, const Bytes& rhs) noexcept
    {
        return Bytes(rhs) *= lhs;
    }

    constexpr Bytes operator/(const Bytes& lhs, std::int64_t rhs) noexcept
    {
        return Bytes(lhs) /= rhs;
    }

    constexpr std::int64_t operator/(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs / *rhs;
    }

    constexpr Bytes operator%(const Bytes& lhs, std::int64_t rhs) noexcept
    {
        return Bytes(lhs) %= rhs;
    }

    constexpr std::int64_t operator%(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs % *rhs;
    }

    constexpr Bytes operator>>(const Bytes& lhs, std::int64_t rhs) noexcept
    {
        return Bytes(lhs) >>= rhs;
    }

    constexpr Bytes operator<<(const Bytes& lhs, std::int64_t rhs) noexcept
    {
        return Bytes(lhs) <<= rhs;
    }

    constexpr Bytes operator~(const Bytes& rhs) noexcept
    {
        return Bytes(~(*rhs));
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
        return lhs << *rhs;
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

    template <typename TType>
    constexpr Bytes BytesOf(const TType& rhs)
    {
        return Bytes(sizeof(rhs));
    }

    template <typename TType>
    constexpr Bytes BytesOf()
    {
        return Bytes(sizeof(TType));
    }

    // Bits.

    constexpr Bits::Bits(std::int64_t bits)
        : bits_(bits)
    {

    }

    constexpr Bits::Bits(const Bytes& bytes)
        : bits_(*bytes * kByte)
    {

    }

    constexpr std::int64_t Bits::operator*() const
    {
        return bits_;
    }

    constexpr Bits& Bits::operator+=(const Bits& rhs) noexcept
    {
        bits_ += rhs.bits_;
        return *this;
    }

    constexpr Bits& Bits::operator-=(const Bits& rhs) noexcept
    {
        bits_ -= rhs.bits_;
        return *this;
    }

    constexpr Bits& Bits::operator*=(std::int64_t rhs) noexcept
    {
        bits_ *= rhs;
        return *this;
    }

    constexpr Bits& Bits::operator/=(std::int64_t rhs) noexcept
    {
        bits_ /= rhs;
        return *this;
    }

    constexpr Bits& Bits::operator%=(std::int64_t rhs) noexcept
    {
        bits_ %= rhs;
        return *this;
    }

    constexpr Bits& Bits::operator>>=(std::int64_t rhs) noexcept
    {
        bits_ >>= rhs;
        return *this;
    }

    constexpr Bits& Bits::operator<<=(std::int64_t rhs) noexcept
    {
        bits_ <<= rhs;
        return *this;
    }

    constexpr Bits& Bits::operator&=(const Bits& rhs) noexcept
    {
        bits_ &= rhs.bits_;
        return *this;
    }

    constexpr Bits& Bits::operator|=(const Bits& rhs) noexcept
    {
        bits_ |= rhs.bits_;
        return *this;
    }

    constexpr Bits& Bits::operator^=(const Bits& rhs) noexcept
    {
        bits_ ^= rhs.bits_;
        return *this;
    }

    constexpr bool operator==(const Bits& lhs, const Bits& rhs) noexcept
    {
        return *lhs == *rhs;
    }

    constexpr bool operator!=(const Bits& lhs, const Bits& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(const Bits& lhs, const Bits& rhs) noexcept
    {
        return *lhs > *rhs;
    }

    constexpr bool operator<(const Bits& lhs, const Bits& rhs) noexcept
    {
        return *lhs < *rhs;
    }

    constexpr bool operator>=(const Bits& lhs, const Bits& rhs) noexcept
    {
        return *lhs >= *rhs;
    }

    constexpr bool operator<=(const Bits& lhs, const Bits& rhs) noexcept
    {
        return *lhs <= *rhs;
    }

    constexpr Bits operator+(const Bits& lhs, const Bits& rhs) noexcept
    {
        return Bits(lhs) += rhs;
    }

    constexpr Bits operator-(const Bits& lhs, const Bits& rhs) noexcept
    {
        return Bits(lhs) -= rhs;
    }

    constexpr Bits operator*(const Bits& lhs, std::int64_t rhs) noexcept
    {
        return Bits(lhs) *= rhs;
    }

    constexpr Bits operator*(std::int64_t lhs, const Bits& rhs) noexcept
    {
        return Bits(rhs) *= lhs;
    }

    constexpr Bits operator/(const Bits& lhs, std::int64_t rhs) noexcept
    {
        return Bits(lhs) /= rhs;
    }

    constexpr std::int64_t operator/(const Bits& lhs, const Bits& rhs) noexcept
    {
        return *lhs / *rhs;
    }

    constexpr Bits operator%(const Bits& lhs, std::int64_t rhs) noexcept
    {
        return Bits(lhs) %= rhs;
    }

    constexpr std::int64_t operator%(const Bits& lhs, const Bits& rhs) noexcept
    {
        return *lhs % *rhs;
    }

    constexpr Bits operator>>(const Bits& lhs, std::int64_t rhs) noexcept
    {
        return Bits(lhs) >>= rhs;
    }

    constexpr Bits operator<<(const Bits& lhs, std::int64_t rhs) noexcept
    {
        return Bits(lhs) <<= rhs;
    }

    constexpr Bits operator~(const Bits& rhs) noexcept
    {
        return Bits(~(*rhs));
    }

    constexpr Bits operator&(const Bits& lhs, const Bits& rhs) noexcept
    {
        return Bits(lhs) &= rhs;
    }

    constexpr Bits operator|(const Bits& lhs, const Bits& rhs) noexcept
    {
        return Bits(lhs) |= rhs;
    }

    constexpr Bits operator^(const Bits& lhs, const Bits& rhs) noexcept
    {
        return Bits(lhs) ^= rhs;
    }

    inline std::ostream& operator<<(std::ostream& lhs, const Bits& rhs)
    {
        return lhs << *rhs;
    }

    constexpr Bits operator "" _Bits(std::size_t lhs)
    {
        return Bits(lhs);
    }

    constexpr syntropy::Bytes ToBytesCeil(Bits rhs)
    {
        return Bytes(DivCeil(*rhs, Bits::kByte));
    }

    constexpr syntropy::Bytes ToBytesFloor(Bits rhs)
    {
        return Bytes(DivFloor(*rhs, Bits::kByte));
    }

    template <typename TType>
    constexpr Bits BitsOf(const TType& rhs)
    {
        return BytesOf(rhs);
    }

    template <typename TType>
    constexpr Bits BitsOf()
    {
        return BytesOf<TType>();
    }

}

