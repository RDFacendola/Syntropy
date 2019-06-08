
/// \file bit.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of the Bit and BitReference type and related functions.
///
/// \author Raffaele D. Facendola - June 2019

#pragma once

#include <cstdint>
#include <cstddef>

#include "syntropy/memory/memory_address.h"

namespace syntropy
{
    /************************************************************************/
    /* BIT                                                                  */
    /************************************************************************/

    /// \brief Represents a single bit.
    /// This type is meant to be a strongly-typed replacement for bool.
    /// \author Raffaele D. Facendola - June 2019
    class Bit
    {
    public:

        /// \brief Create a new bit equal to zero.
        constexpr Bit() = default;

        /// \brief Create a new bit from a boolean.
        constexpr Bit(bool bit);

        /// \brief Create a new bit from a value.
        /// Values other than zero are considered one.
        constexpr explicit Bit(std::size_t bit);

        /// \brief Default copy-constructor.
        constexpr Bit(const Bit&) = default;

        /// \brief Default assignment operator.
        Bit& operator=(const Bit&) = default;

        /// \brief Get the numeric value of the bit.
        /// \return Returns the numeric value of the bit.
        constexpr explicit operator std::size_t() const;

        /// \brief Get the boolean value of the bit.
        /// \return Returns true if the bit is one, returns false otherwise.
        constexpr operator bool() const;

        /// \brief Bitwise-and operator.
        /// \return Returns a reference to this element.
        constexpr Bit& operator&=(const Bit& rhs) noexcept;

        /// \brief Bitwise-or operator.
        /// \return Returns a reference to this element.
        constexpr Bit& operator|=(const Bit& rhs) noexcept;

        /// \brief Bitwise-xor operator.
        /// \return Returns a reference to this element.
        constexpr Bit& operator^=(const Bit& rhs) noexcept;

    private:

        /// \brief Bit value.
        uint8_t bit_ = 0u;

    };

    /// \brief Equality comparison for Bits.
    /// \return Returns true if lhs and rhs have the same value, returns false otherwise.
    constexpr bool operator==(const Bit& lhs, const Bit& rhs) noexcept;

    /// \brief Inequality comparison for Bits.
    /// \return Returns true if lhs and rhs have different values, returns false otherwise.
    constexpr bool operator!=(const Bit& lhs, const Bit& rhs) noexcept;

    /// \brief Bitwise not of a bit.
    /// \return Returns the bitwise not of rhs.
    constexpr Bit operator~(const Bit& rhs) noexcept;

    /// \brief Bitwise and between two bits.
    /// \return Returns the bitwise and between lhs and rhs.
    constexpr Bit operator&(const Bit& lhs, const Bit& rhs) noexcept;

    /// \brief Bitwise and between two bits.
    /// \return Returns the bitwise and between lhs and rhs.
    constexpr Bit operator|(const Bit& lhs, const Bit& rhs) noexcept;

    /// \brief Bitwise and between two bits.
    /// \return Returns the bitwise and between lhs and rhs.
    constexpr Bit operator^(const Bit& lhs, const Bit& rhs) noexcept;

    /// \brief User-defined literal used to convert a number to a bit.
    /// \param lhs Number to convert.
    constexpr Bit operator "" _Bit(std::size_t lhs);
    
    /************************************************************************/
    /* BIT REFERENCE                                                        */
    /************************************************************************/

    /// \brief Reference to a bit in a byte.
    /// \author Raffaele D. Facendola - June 2019
    class BitReference
    {
    public:

        /// \brief Create a reference to a bit.
        /// \param address Base address.
        /// \param offset Offset relative to the base address, in bits.
        constexpr BitReference(const MemoryAddress& address, std::size_t offset);

        /// \brief Get the boolean value of the bit.
        /// \return Returns true if the bit is one, returns false otherwise.
        constexpr operator bool() const;

        /// \brief Get the bit value.
        constexpr operator Bit() const;

        /// \brief Assign a new value to the underlying bit.
        BitReference& operator=(const Bit& bit);

    private:

        /// \brief Pointer of the byte holding the bit.
        uint8_t* byte_{ nullptr };

        /// \brief Index of the bit.
        uint8_t index_{ 0 };
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Bit.

    constexpr Bit::Bit(bool bit)
        : bit_(bit ? 1u : 0u)
    {

    }

    constexpr Bit::Bit(std::size_t bit)
        : bit_(bit == 0 ? 0 : 1)
    {

    }

    constexpr Bit::operator std::size_t() const
    {
        return bit_;
    }

    constexpr Bit::operator bool() const
    {
        return (bit_ != 0);
    }

    constexpr Bit& Bit::operator&=(const Bit& rhs) noexcept
    {
        bit_ &= rhs.bit_;
        return *this;
    }

    constexpr Bit& Bit::operator|=(const Bit& rhs) noexcept
    {
        bit_ |= rhs.bit_;
        return *this;
    }

    constexpr Bit& Bit::operator^=(const Bit& rhs) noexcept
    {
        bit_ ^= rhs.bit_;
        return *this;
    }

    constexpr bool operator==(const Bit& lhs, const Bit& rhs) noexcept
    {
        return bool(lhs) == bool(rhs);
    }

    constexpr bool operator!=(const Bit& lhs, const Bit& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr Bit operator~(const Bit& rhs) noexcept
    {
        return Bit(!bool(rhs));
    }

    constexpr Bit operator&(const Bit& lhs, const Bit& rhs) noexcept
    {
        return Bit(lhs) &= rhs;
    }

    constexpr Bit operator|(const Bit& lhs, const Bit& rhs) noexcept
    {
        return Bit(lhs) |= rhs;
    }

    constexpr Bit operator^(const Bit& lhs, const Bit& rhs) noexcept
    {
        return Bit(lhs) ^= rhs;
    }

    constexpr Bit operator "" _Bit(std::size_t lhs)
    {
        return Bit(lhs);
    }

    // BitReference.

    constexpr BitReference::BitReference(const MemoryAddress& address, std::size_t offset)
        : byte_((address + Bytes(offset / 8)).As<uint8_t>())
        , index_(offset % 8)
    {

    }

    constexpr BitReference::operator bool() const
    {
        return Bit(*this);
    }

    constexpr BitReference::operator Bit() const
    {
        auto bit = !!(*byte_ & (1u << index_));

        return Bit(bit);
    }

    inline BitReference& BitReference::operator=(const Bit& bit)
    {
        *byte_ &= ~uint8_t(1u << index_);
        *byte_ |= uint8_t(bit << index_);

        return *this;
    }

}

