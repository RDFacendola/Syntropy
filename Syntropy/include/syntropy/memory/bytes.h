
/// \file bytes.h
/// \brief This header is part of the Syntropy memory module. It contains the definition of bytes memory unit types and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <ostream>

#include "syntropy/core/types.h"

namespace syntropy
{
    /************************************************************************/
    /* BYTES                                                                */
    /************************************************************************/

    /// \brief Represents a number of bytes.
    /// \author Raffaele D. Facendola - July 2018
    class Bytes
    {
    public:

        /// \brief Create a new bytes count equal to zero.
        constexpr Bytes() = default;

        /// \brief Create a new bytes count.
        constexpr explicit Bytes(Int bytes);

        /// \brief Default copy-constructor.
        constexpr Bytes(const Bytes&) = default;

        /// \brief Default assignment operator.
        Bytes& operator=(const Bytes&) = default;

        /// \brief Get the number of bytes.
        /// \return Returns the number of bytes.
        constexpr Int operator*() const;

        /// \brief Get the number of bytes.
        /// \return Returns the number of bytes.
        constexpr Int GetCount() const;

        /// \brief Add a bytes amount.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator+=(const Bytes& rhs) noexcept;

        /// \brief Subtract a bytes amount.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator-=(const Bytes& rhs) noexcept;

        /// \brief Multiply this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator*=(Int rhs) noexcept;

        /// \brief Divides this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator/=(Int rhs) noexcept;

        /// \brief Division remainder of this element by a number.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator%=(Int rhs) noexcept;

        /// \brief Right shift operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator>>=(Int rhs) noexcept;

        /// \brief Left shift operator.
        /// \return Returns a reference to this element.
        constexpr Bytes& operator<<=(Int rhs) noexcept;

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

        ///< \brief Number of bytes. Can be negative.
        Int bytes_ = 0;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Increment the value by one Byte. Prefix.
    constexpr Bytes& operator++(Bytes& rhs);

    /// \brief Increment the value by one Byte. Postfix.
    constexpr Bytes operator++(Bytes& rhs, int);

    /// \brief Decrement the value by one Byte. Prefix.
    constexpr Bytes& operator--(Bytes& rhs);

    /// \brief Decrement the value by one Byte. Postfix.
    constexpr Bytes operator--(Bytes& rhs, int);

    /// \brief Equality comparison for Bytes.
    /// \return Returns true if lhs and rhs refer to the same amount of memory, returns false otherwise.
    constexpr Bool operator==(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Inequality comparison for Bytes.
    /// \return Returns true if lhs and rhs refer to different amounts of memory, returns false otherwise.
    constexpr Bool operator!=(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Greater-than comparison for Bytes.
    /// \return Returns true if lhs refers to a memory amount that is strictly greater than rhs, returns false otherwise.
    constexpr Bool operator>(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Less-than comparison for Bytes.
    /// \return Returns true if lhs refers to a memory amount that is strictly less than rhs, returns false otherwise.
    constexpr Bool operator<(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Greater-or-equal comparison for Bytes.
    /// \return Returns true if lhs refers to a memory amount that is equal or greater than rhs, returns false otherwise.
    constexpr Bool operator>=(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Less-or-equal comparison for Bytes.
    /// \return Returns true if lhs refers to a memory amount that is equal or less than rhs, returns false otherwise.
    constexpr Bool operator<=(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Sum two bytes amount.
    /// \return Returns a memory amount which is the sum of lhs and rhs.
    constexpr Bytes operator+(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Subtract two bytes amount.
    /// \return Returns a memory amount which is the difference of lhs and rhs.
    constexpr Bytes operator-(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs times rhs.
    constexpr Bytes operator*(const Bytes& lhs, Int rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to rhs times lhs.
    constexpr Bytes operator*(Int lhs, const Bytes& rhs) noexcept;

    /// \brief Divide a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr Bytes operator/(const Bytes& lhs, Int rhs) noexcept;

    /// \brief Divide a byte amount by another byte amount.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr Int operator/(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by a number.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    constexpr Bytes operator%(const Bytes& lhs, Int rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by another byte amount.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    constexpr Int operator%(const Bytes& lhs, const Bytes& rhs) noexcept;

    /// \brief Right-shift a bytes amount.
    /// \return Returns a memory amount which is equal to lhs right-shifted by rhs.
    constexpr Bytes operator>>(const Bytes& lhs, Int rhs) noexcept;

    /// \brief Left-shift a bytes amount.
    /// \return Returns a memory amount which is equal to lhs left-shifted by rhs.
    constexpr Bytes operator<<(const Bytes& lhs, Int rhs) noexcept;

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

    /// \brief Explicitly convert a number to a Bytes amount.
    template <typename TType>
    constexpr Bytes ToBytes(const TType& rhs) noexcept;

    /// \brief Get the size of rhs, in bytes.
    template <typename TType>
    constexpr Bytes BytesOf(const TType& rhs);

    /// \brief Get the size of TType, in bytes.
    template <typename TType>
    constexpr Bytes BytesOf();

    /// \brief Stream insertion for Bytes.
    std::ostream& operator<<(std::ostream& lhs, const Bytes& rhs);

    /// \brief Exposes memory unit literals.
    namespace Literals
    {
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
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Bytes.

    constexpr Bytes::Bytes(Int bytes)
        : bytes_(bytes)
    {

    }

    constexpr Int Bytes::operator*() const
    {
        return bytes_;
    }

    constexpr Int Bytes::GetCount() const
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

    constexpr Bytes& Bytes::operator*=(Int rhs) noexcept
    {
        bytes_ *= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator/=(Int rhs) noexcept
    {
        bytes_ /= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator%=(Int rhs) noexcept
    {
        bytes_ %= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator>>=(Int rhs) noexcept
    {
        bytes_ >>= rhs;
        return *this;
    }

    constexpr Bytes& Bytes::operator<<=(Int rhs) noexcept
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

    // Non-member functions.

    constexpr Bytes& operator++(Bytes& rhs)
    {
        rhs += Bytes{ 1 };

        return rhs;
    }

    constexpr Bytes operator++(Bytes& rhs, int)
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    constexpr Bytes& operator--(Bytes& rhs)
    {
        rhs -= Bytes{ 1 };

        return rhs;
    }

    constexpr Bytes operator--(Bytes& rhs, int)
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    constexpr Bool operator==(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs == *rhs;
    }

    constexpr Bool operator!=(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr Bool operator>(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs > *rhs;
    }

    constexpr Bool operator<(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs < *rhs;
    }

    constexpr Bool operator>=(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs >= *rhs;
    }

    constexpr Bool operator<=(const Bytes& lhs, const Bytes& rhs) noexcept
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

    constexpr Bytes operator*(const Bytes& lhs, Int rhs) noexcept
    {
        return Bytes(lhs) *= rhs;
    }

    constexpr Bytes operator*(Int lhs, const Bytes& rhs) noexcept
    {
        return Bytes(rhs) *= lhs;
    }

    constexpr Bytes operator/(const Bytes& lhs, Int rhs) noexcept
    {
        return Bytes(lhs) /= rhs;
    }

    constexpr Int operator/(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs / *rhs;
    }

    constexpr Bytes operator%(const Bytes& lhs, Int rhs) noexcept
    {
        return Bytes(lhs) %= rhs;
    }

    constexpr Int operator%(const Bytes& lhs, const Bytes& rhs) noexcept
    {
        return *lhs % *rhs;
    }

    constexpr Bytes operator>>(const Bytes& lhs, Int rhs) noexcept
    {
        return Bytes(lhs) >>= rhs;
    }

    constexpr Bytes operator<<(const Bytes& lhs, Int rhs) noexcept
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

    template <typename TType>
    constexpr Bytes ToBytes(const TType& rhs) noexcept
    {
        return Bytes(static_cast<Int>(rhs));
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

    namespace Literals
    {
        constexpr Bytes operator "" _Bytes(std::size_t lhs)
        {
            return Bytes(lhs);
        }

        constexpr Bytes operator "" _KiBytes(std::size_t lhs)
        {
            return Bytes(lhs * 0x400ull);
        }

        constexpr Bytes operator "" _MiBytes(std::size_t lhs)
        {
            return Bytes(lhs * 0x400ull * 0x400ull);
        }

        constexpr Bytes operator "" _GiBytes(std::size_t lhs)
        {
            return Bytes(lhs * 0x400ull * 0x400ull * 0x400ull);
        }

        constexpr Bytes operator "" _TiBytes(std::size_t lhs)
        {
            return Bytes(lhs * 0x400ull * 0x400ull * 0x400ull * 0x400ull);
        }
    }

}

