
/// \file bytes.h
/// \brief This header is part of the Syntropy memory module. It contains the definition of bytes memory unit types and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <ostream>

#include "syntropy/language/foundation.h"
#include "syntropy/memory/memory_types.h"

namespace Syntropy
{
    /************************************************************************/
    /* BYTES                                                                */
    /************************************************************************/

    /// \brief Represents a bytes amount.
    enum class Bytes : Int {};

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Exposes bytes-related functions.
    namespace Memory
    {
        /// \brief Get the size of rhs, in bytes.
        template <typename TType>
        constexpr Bytes SizeOf(const TType& rhs) noexcept;

        /// \brief Get the size of TType, in bytes.
        template <typename TType>
        constexpr Bytes SizeOf() noexcept;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Sum a byte amount to an existing value.
    constexpr Bytes& operator+=(Bytes& lhs, Bytes rhs) noexcept;

    /// \brief Subtract a byte amount from an existing value.
    constexpr Bytes& operator-=(Bytes& lhs, Bytes rhs) noexcept;

    /// \brief Multiply a byte amount by a integer number.
    constexpr Bytes& operator*=(Bytes& lhs, Int rhs) noexcept;

    /// \brief Divide a byte amount by an integer value and round the result towards zero.
    constexpr Bytes& operator/=(Bytes& lhs, Int rhs) noexcept;

    /// \brief Division remainder of a bytes amount by an integer value.
    constexpr Bytes& operator%=(Bytes& lhs, Int rhs) noexcept;

    /// \brief Shift a byte amount right.
    constexpr Bytes& operator>>=(Bytes& lhs, Int rhs) noexcept;

    /// \brief Shift a bytes amount left.
    constexpr Bytes& operator<<=(Bytes& lhs, Int rhs) noexcept;

    /// \brief Bitwise-and operator.
    constexpr Bytes& operator&=(Bytes& lhs, Bytes rhs) noexcept;

    /// \brief Bitwise-or operator.
    constexpr Bytes& operator|=(Bytes& lhs, Bytes rhs) noexcept;

    /// \brief Bitwise-xor operator.
    constexpr Bytes& operator^=(Bytes& lhs, Bytes rhs) noexcept;

    /// \brief Pre-increment a bytes amount by one.
    constexpr Bytes& operator++(Bytes& rhs) noexcept;

    /// \brief Post-increment a bytes amount by one.
    constexpr Bytes operator++(Bytes& rhs, int) noexcept;

    /// \brief Pre-decrement a bytes amount by one.
    constexpr Bytes& operator--(Bytes& rhs) noexcept;

    /// \brief Post-decrement a bytes amount by one.
    constexpr Bytes operator--(Bytes& rhs, int) noexcept;

    /// \brief Sum two bytes amount.
    /// \return Returns a memory amount which is the sum of lhs and rhs.
    constexpr Bytes operator+(Bytes lhs, Bytes rhs) noexcept;

    /// \brief Subtract two bytes amount.
    /// \return Returns a memory amount which is the difference of lhs and rhs.
    constexpr Bytes operator-(Bytes lhs, Bytes rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs times rhs.
    constexpr Bytes operator*(Bytes lhs, Int rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to rhs times lhs.
    constexpr Bytes operator*(Int lhs, Bytes rhs) noexcept;

    /// \brief Divide a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr Bytes operator/(Bytes lhs, Int rhs) noexcept;

    /// \brief Divide a byte amount by another byte amount.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    constexpr Int operator/(Bytes lhs, Bytes rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by a number.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    constexpr Bytes operator%(Bytes lhs, Int rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by another byte amount.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    constexpr Int operator%(Bytes lhs, Bytes rhs) noexcept;

    /// \brief Right-shift a bytes amount.
    /// \return Returns a memory amount which is equal to lhs right-shifted by rhs.
    constexpr Bytes operator>>(Bytes lhs, Int rhs) noexcept;

    /// \brief Left-shift a bytes amount.
    /// \return Returns a memory amount which is equal to lhs left-shifted by rhs.
    constexpr Bytes operator<<(Bytes lhs, Int rhs) noexcept;

    /// \brief Bitwise not of a byte quantity.
    /// \return Returns a memory amount which is the bitwise not of rhs.
    constexpr Bytes operator~(Bytes rhs) noexcept;

    /// \brief Bitwise and between two bytes quantities.
    /// \return Returns a memory amount which is equal to the bitwise and between lhs and rhs.
    constexpr Bytes operator&(Bytes lhs, Bytes rhs) noexcept;

    /// \brief Bitwise or between two bytes quantities.
    /// \return Returns a memory amount which is equal to the bitwise or between lhs and rhs.
    constexpr Bytes operator|(Bytes lhs, Bytes rhs) noexcept;

    /// \brief Bitwise xor between two bytes quantities.
    /// \return Returns a memory amount which is equal to the bitwise xor between lhs and rhs.
    constexpr Bytes operator^(Bytes lhs, Bytes rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    constexpr RWBytePtr& operator+=(RWBytePtr& lhs, Bytes rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    constexpr RWBytePtr& operator-=(RWBytePtr& lhs, Bytes rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    constexpr RWBytePtr operator+(RWBytePtr lhs, Bytes rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    constexpr RWBytePtr operator-(RWBytePtr lhs, Bytes rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    constexpr BytePtr& operator+=(BytePtr& lhs, Bytes rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    constexpr BytePtr& operator-=(BytePtr& lhs, Bytes rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    constexpr BytePtr operator+(BytePtr lhs, Bytes rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    constexpr BytePtr operator-(BytePtr lhs, Bytes rhs) noexcept;

    /// \brief Stream insertion for Bytes.
    std::ostream& operator<<(std::ostream& lhs, Bytes rhs);

    /************************************************************************/
    /* TYPE CAST                                                            */
    /************************************************************************/

    /// \brief Convert an amount of bytes to integer.
    constexpr Int ToInt(Bytes lhs) noexcept;

    /// \brief Convert an integer number to a bytes amount.
    constexpr Bytes ToBytes(Int lhs) noexcept;

    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief Exposes memory unit literals.
    namespace Literals
    {
        /// \brief User-defined literal used to convert a number from Bytes to Bytes.
        /// \param number Number to convert.
        constexpr Bytes operator "" _Bytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to convert a number from KibiBytes to Bytes.
        /// \param number Number to convert.
        constexpr Bytes operator "" _KiBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to convert a number from MebiBytes to Bytes.
        /// \param number Number to convert.
        constexpr Bytes operator "" _MiBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to convert a number from GibiBytes to Bytes.
        /// \param number Number to convert.
        constexpr Bytes operator "" _GiBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to convert a number from TebiBytes to Bytes.
        /// \param number Number to convert.
        constexpr Bytes operator "" _TiBytes(std::size_t lhs) noexcept;
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Memory.
    // =======

    template <typename TType>
    constexpr Bytes Memory::SizeOf(const TType& rhs) noexcept
    {
        return Bytes{ sizeof(rhs) };
    }

    template <typename TType>
    constexpr Bytes Memory::SizeOf() noexcept
    {
        return Bytes{ sizeof(TType) };
    }

    // Non-member functions.
    // =====================

    constexpr Bytes& operator+=(Bytes& lhs, Bytes rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    constexpr Bytes& operator-=(Bytes& lhs, Bytes rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    constexpr Bytes& operator*=(Bytes& lhs, Int rhs) noexcept
    {
        lhs = lhs * rhs;

        return lhs;
    }

    constexpr Bytes& operator/=(Bytes& lhs, Int rhs) noexcept
    {
        lhs = lhs / rhs;

        return lhs;
    }

    constexpr Bytes& operator%=(Bytes& lhs, Int rhs) noexcept
    {
        lhs = lhs % rhs;

        return lhs;
    }

    constexpr Bytes& operator>>=(Bytes& lhs, Int rhs) noexcept
    {
        lhs = lhs >> rhs;

        return lhs;
    }

    constexpr Bytes& operator<<=(Bytes& lhs, Int rhs) noexcept
    {
        lhs = lhs << rhs;

        return lhs;
    }

    constexpr Bytes& operator&=(Bytes& lhs, Bytes rhs) noexcept
    {
        lhs = lhs & rhs;

        return lhs;
    }

    constexpr Bytes& operator|=(Bytes& lhs, Bytes rhs) noexcept
    {
        lhs = lhs | rhs;

        return lhs;
    }

    constexpr Bytes& operator^=(Bytes& lhs, Bytes rhs) noexcept
    {
        lhs = lhs ^ rhs;

        return lhs;
    }

    constexpr Bytes& operator++(Bytes& rhs) noexcept
    {
        using namespace Literals;

        rhs += 1_Bytes;

        return rhs;
    }

    constexpr Bytes operator++(Bytes& rhs, int) noexcept
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    constexpr Bytes& operator--(Bytes& rhs) noexcept
    {
        using namespace Literals;

        rhs -= 1_Bytes;

        return rhs;
    }

    constexpr Bytes operator--(Bytes& rhs, int) noexcept
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    constexpr Bytes operator+(Bytes lhs, Bytes rhs) noexcept
    {
        return ToBytes(ToInt(lhs) + ToInt(rhs));
    }

    constexpr Bytes operator-(Bytes lhs, Bytes rhs) noexcept
    {
        return ToBytes(ToInt(lhs) - ToInt(rhs));
    }

    constexpr Bytes operator*(Bytes lhs, Int rhs) noexcept
    {
        return ToBytes(ToInt(lhs) * rhs);
    }

    constexpr Bytes operator*(Int lhs, Bytes rhs) noexcept
    {
        return ToBytes(lhs * ToInt(rhs));
    }

    constexpr Bytes operator/(Bytes lhs, Int rhs) noexcept
    {
        return ToBytes(ToInt(lhs) / rhs);
    }

    constexpr Int operator/(Bytes lhs, Bytes rhs) noexcept
    {
        return ToInt(lhs) / ToInt(rhs);
    }

    constexpr Bytes operator%(Bytes lhs, Int rhs) noexcept
    {
        return ToBytes(ToInt(lhs) % rhs);
    }

    constexpr Int operator%(Bytes lhs, Bytes rhs) noexcept
    {
        return ToInt(lhs) % ToInt(rhs);
    }

    constexpr Bytes operator>>(Bytes lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToBytes(ToInt(lhs) >> rhs) : (lhs << -rhs);
    }

    constexpr Bytes operator<<(Bytes lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToBytes(ToInt(lhs) << rhs) : (lhs >> -rhs);
    }

    constexpr Bytes operator~(Bytes rhs) noexcept
    {
        return ToBytes(~ToInt(rhs));
    }

    constexpr Bytes operator&(Bytes lhs, Bytes rhs) noexcept
    {
        return ToBytes(ToInt(lhs) & ToInt(rhs));
    }

    constexpr Bytes operator|(Bytes lhs, Bytes rhs) noexcept
    {
        return ToBytes(ToInt(lhs) | ToInt(rhs));
    }

    constexpr Bytes operator^(Bytes lhs, Bytes rhs) noexcept
    {
        return ToBytes(ToInt(lhs) ^ ToInt(rhs));
    }

    constexpr RWBytePtr& operator+=(RWBytePtr& lhs, Bytes rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    constexpr RWBytePtr& operator-=(RWBytePtr& lhs, Bytes rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    constexpr RWBytePtr operator+(RWBytePtr lhs, Bytes rhs) noexcept
    {
        return lhs + ToInt(rhs);
    }

    constexpr RWBytePtr operator-(RWBytePtr lhs, Bytes rhs) noexcept
    {
        return lhs - ToInt(rhs);
    }

    constexpr BytePtr& operator+=(BytePtr& lhs, Bytes rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    constexpr BytePtr& operator-=(BytePtr& lhs, Bytes rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    constexpr BytePtr operator+(BytePtr lhs, Bytes rhs) noexcept
    {
        return lhs + ToInt(rhs);
    }

    constexpr BytePtr operator-(BytePtr lhs, Bytes rhs) noexcept
    {
        return lhs - ToInt(rhs);
    }

    inline std::ostream& operator<<(std::ostream& lhs, Bytes rhs)
    {
        return lhs << ToInt(rhs);
    }

    // Type cast.

    constexpr Int ToInt(Bytes lhs) noexcept
    {
        return static_cast<Int>(lhs);
    }

    constexpr Bytes ToBytes(Int lhs) noexcept
    {
        return Bytes{ lhs };
    }

    // Literals.

    constexpr Bytes Literals::operator "" _Bytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs);
    }

    constexpr Bytes Literals::operator "" _KiBytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs * 0x400ull);
    }

    constexpr Bytes Literals::operator "" _MiBytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs * 0x400ull * 0x400ull);
    }

    constexpr Bytes Literals::operator "" _GiBytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs * 0x400ull * 0x400ull * 0x400ull);
    }

    constexpr Bytes Literals::operator "" _TiBytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs * 0x400ull * 0x400ull * 0x400ull * 0x400ull);
    }

}

