
/// \file bit.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of the bit types and bit-manipulation functionalities.
///
/// \author Raffaele D. Facendola - June 2019

#pragma once

#include <cstdint>
#include <cstddef>

#include "syntropy/memory/memory_bit_address.h"
#include "syntropy/memory/bits.h"

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
        constexpr explicit Bit(std::int64_t bit);

        /// \brief Default copy-constructor.
        constexpr Bit(const Bit&) = default;

        /// \brief Default assignment operator.
        Bit& operator=(const Bit&) = default;

        /// \brief Get the numeric value of the bit.
        /// \return Returns the numeric value of the bit.
        constexpr std::int64_t operator*() const;

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
        std::int8_t bit_ = 0;

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

    /// \brief User-defined literal used to convert a number to a bit value.
    /// \param number Number to convert.
    constexpr Bit operator "" _Bit(std::size_t lhs);

    /// \brief Template specialization for Bit.
    template <>
    constexpr Bits BitsOf<Bit>(const Bit& rhs);

    /// \brief Template specialization for Bit.
    template <>
    constexpr Bits BitsOf<Bit>();

    /// \brief Copy a number of bits from one bit-addressed memory region to another one.
    /// \remarks If the two memory regions overlap, the behavior of this function is undefined.
    /// \param destination Memory location to copy to.
    /// \param source Memory location to copy from.
    /// \param count Number of bits to copy.
    constexpr void BitMemCopy(MemoryBitAddress destination, ConstMemoryBitAddress source, Bits count);

    /************************************************************************/
    /* BIT REFERENCE                                                        */
    /************************************************************************/

    /// \brief Reference to a single bit.
    /// \author Raffaele D. Facendola - June 2019
    class BitReference
    {
    public:

        /// \brief Create a reference to a bit.
        /// \param address Bit address.
        constexpr BitReference(const MemoryBitAddress& address);

        /// \brief Get the boolean value of the bit.
        /// \return Returns true if the bit is one, returns false otherwise.
        constexpr operator bool() const;

        /// \brief Get the bit value.
        constexpr operator Bit() const;

        /// \brief Assign a new value to the underlying bit.
        BitReference& operator=(const Bit& bit);

    private:

        /// \brief Base address of the bit.
        int8_t* base_address_{ nullptr };

        /// \brief Bit offset relative to the base address.
        int8_t offset_{ 0 };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Bit.

    constexpr Bit::Bit(bool bit)
        : bit_(bit ? 1u : 0u)
    {

    }

    constexpr Bit::Bit(std::int64_t bit)
        : bit_(bit == 0 ? 0 : 1)
    {

    }

    constexpr std::int64_t Bit::operator*() const
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
        return Bit(static_cast<std::int64_t>(lhs));
    }

    template <>
    constexpr Bits BitsOf<Bit>(const Bit& rhs)
    {
        return 1_Bits;
    }

    template <>
    constexpr Bits BitsOf<Bit>()
    {
        return 1_Bits;
    }

    constexpr void BitMemCopy(MemoryBitAddress destination, ConstMemoryBitAddress source, Bits count)
    {
        while (count > 0_Bits)
        {
            auto source_buffer = *source.GetBaseAddress().As<uint8_t>();
            auto& destination_buffer = *destination.GetBaseAddress().As<uint8_t>();

            auto destination_offset = destination.GetOffset();
            auto source_offset = source.GetOffset();

            auto bits = std::min(count, Bits(Bits::kByte) - std::max(destination_offset, source_offset));       // Number of bits to copy this iteration.

            auto source_mask = int8_t((1 << *bits) - 1);
            auto destination_mask = ~int8_t(source_mask << *destination_offset);

            auto chunk = int8_t(source_buffer >> *source_offset);

            chunk &= source_mask;
            chunk <<= *destination_offset;

            destination_buffer &= destination_mask;
            destination_buffer |= chunk;

            destination += bits;
            source += bits;
            count -= bits;
        }
    }

    // BitReference.

    constexpr BitReference::BitReference(const MemoryBitAddress& address)
        : base_address_(address.GetBaseAddress().As<int8_t>())
        , offset_(static_cast<int8_t>(*address.GetOffset()))
    {

    }

    constexpr BitReference::operator bool() const
    {
        return Bit(*this);
    }

    constexpr BitReference::operator Bit() const
    {
        auto bit = !!(*base_address_ & int8_t(1 << offset_));

        return Bit(bit);
    }

    inline BitReference& BitReference::operator=(const Bit& bit)
    {
        *base_address_ &= ~int8_t(1 << offset_);
        *base_address_ |= int8_t(bit << offset_);

        return *this;
    }

}

