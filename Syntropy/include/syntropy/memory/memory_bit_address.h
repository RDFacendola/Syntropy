
/// \file memory_bit_address.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of the bit-level address type and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <cstdint>
#include <type_traits>

#include "syntropy/diagnostics/assert.h"

#include "syntropy/memory/bits.h"
#include "syntropy/memory/memory_address.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY BIT ADDRESS                                                   */
    /************************************************************************/

    /// \brief Represents a bit address.
    /// This type is meant to be used when addressing non-byte aligned memory blocks.
    /// \author Raffaele D. Facendola - June 2019
    template <bool is_const>
    class MemoryBitAddressT
    {
    public:

        /// \brief Type alias for the underlying pointer type.
        using TPointer = std::conditional_t<is_const, const void*, void *>;

        /// \brief Create a en empty address.
        constexpr MemoryBitAddressT() = default;

        /// \brief Create a new address.
        /// \param address Base address.
        template <bool is_address_const>
        constexpr MemoryBitAddressT(const MemoryAddressT<is_address_const>& address);

        /// \brief Create a new address.
        /// \param address Base address.
        constexpr MemoryBitAddressT(TPointer address);

        /// \brief Create a new address.
        /// \param address Base address.
        /// \param offset Offset relative to the base address, in bits.
        template <bool is_address_const>
        constexpr MemoryBitAddressT(const MemoryAddressT<is_address_const>& address, Bits offset);

        /// \brief Create a new address.
        /// \param address Base address.
        /// \param offset Offset relative to the base address, in bits.
        constexpr MemoryBitAddressT(TPointer address, Bits offset);

        /// \brief Create a en empty address.
        constexpr MemoryBitAddressT(std::nullptr_t);

        /// \brief Default copy-constructor.
        template <bool is_rhs_const>
        constexpr MemoryBitAddressT(const MemoryBitAddressT<is_rhs_const>& rhs);

        /// \brief Default assignment operator.
        MemoryBitAddressT& operator=(const MemoryBitAddressT&) = default;

        /// \brief Get the memory base address, aligned down to byte boundary.
        constexpr const MemoryAddressT<is_const>& GetBaseAddress() const noexcept;

        /// \brief Get the bit offset, relative to the base address.
        constexpr Bits GetOffset() const noexcept;

        /// \brief Check whether the memory address points to a valid address or to nullptr.
        /// \return Returns true if the stored address is nullptr, returns false otherwise.
        constexpr operator bool() const noexcept;

        /// \brief Move the address forward.
        /// \return Returns a reference to this element.
        constexpr MemoryBitAddressT& operator+=(const Bits& rhs) noexcept;

        /// \brief Move the address backwards.
        /// \return Returns a reference to this element.
        constexpr MemoryBitAddressT& operator-=(const Bits& rhs) noexcept;

    private:

        /// \brief Underlying memory address.
        MemoryAddressT<is_const> address_;

        /// \brief Bit offset, relative to the address.
        Bits offset_;

    };

    /// \brief Type alias for non-const memory bit addresses.
    using MemoryBitAddress = MemoryBitAddressT<false>;

    /// \brief Type alias for const memory bit address.
    using ConstMemoryBitAddress = MemoryBitAddressT<true>;

    /// \brief Equality comparison for MemoryBitAddress.
    /// \return Returns true if lhs and rhs refer to the same address, returns false otherwise.
    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator==(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept;

    /// \brief Inequality comparison for MemoryBitAddress.
    /// \return Returns true if lhs and rhs refer to different address, returns false otherwise.
    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator!=(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept;

    /// \brief Greater-than comparison for MemoryBitAddress.
    /// \return Returns true if lhs refers to an address that is strictly greater than rhs, returns false otherwise.
    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator>(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept;

    /// \brief Less-than comparison for MemoryBitAddress.
    /// \return Returns true if lhs refers to an address that is strictly less than rhs, returns false otherwise.
    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator<(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept;

    /// \brief Greater-or-equal comparison for MemoryBitAddress.
    /// \return Returns true if lhs refers to an address that is equal or greater than rhs, returns false otherwise.
    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator>=(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept;

    /// \brief Less-or-equal comparison for MemoryBitAddress.
    /// \return Returns true if lhs refers to an address that is equal or less than rhs, returns false otherwise.
    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator<=(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept;

    /// \brief Move a bit address forward.
    /// \return Returns a bit address which is equal to lhs moved forward by rhs bits.
    template <bool is_const>
    constexpr MemoryBitAddressT<is_const> operator+(const MemoryBitAddressT<is_const>& lhs, const Bits& rhs) noexcept;

    /// \brief Move a bit address backward.
    /// \return Returns a bit address which is equal to lhs moved backward by rhs bits.
    template <bool is_const>
    constexpr MemoryBitAddressT<is_const> operator-(const MemoryBitAddressT<is_const>& lhs, const Bits& rhs) noexcept;

    /// \brief Stream insertion for MemoryBitAddress.
    template <bool is_const>
    std::ostream& operator<<(std::ostream& lhs, const MemoryBitAddressT<is_const>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryBitAddress.

    template <bool is_const>
    template <bool is_address_const>
    constexpr MemoryBitAddressT<is_const>::MemoryBitAddressT(const MemoryAddressT<is_address_const>& address)
        : address_(address)
    {

    }

    template <bool is_const>
    constexpr MemoryBitAddressT<is_const>::MemoryBitAddressT(MemoryBitAddressT::TPointer address)
        : MemoryBitAddressT(MemoryAddressT<is_const>(address))
    {

    }

    template <bool is_const>
    template <bool is_address_const>
    constexpr MemoryBitAddressT<is_const>::MemoryBitAddressT(const MemoryAddressT<is_address_const>& address, Bits offset)
        : address_(address + ToBytesFloor(offset))
        , offset_(offset % Bits::kByte)
    {

    }

    template <bool is_const>
    constexpr MemoryBitAddressT<is_const>::MemoryBitAddressT(MemoryBitAddressT::TPointer address, Bits offset)
        : MemoryBitAddressT(MemoryAddressT<is_const>(address), offset)
    {

    }

    template <bool is_const>
    constexpr MemoryBitAddressT<is_const>::MemoryBitAddressT(std::nullptr_t)
        : MemoryBitAddressT()
    {

    }

    template <bool is_const>
    template <bool is_rhs_const>
    constexpr MemoryBitAddressT<is_const>::MemoryBitAddressT(const MemoryBitAddressT<is_rhs_const>& rhs)
        : address_(rhs.GetBaseAddress())
        , offset_(rhs.GetOffset())
    {

    }

    template <bool is_const>
    constexpr const MemoryAddressT<is_const>& MemoryBitAddressT<is_const>::GetBaseAddress() const noexcept
    {
        return address_;
    }

    template <bool is_const>
    constexpr Bits MemoryBitAddressT<is_const>::GetOffset() const noexcept
    {
        return offset_;
    }

    template <bool is_const>
    constexpr MemoryBitAddressT<is_const>::operator bool() const noexcept
    {
        return address_ || (offset_ > 0_Bits);
    }

    template <bool is_const>
    constexpr MemoryBitAddressT<is_const>& MemoryBitAddressT<is_const>::operator+=(const Bits& rhs) noexcept
    {
        offset_ += rhs;
        address_ += ToBytesFloor(offset_);
        offset_ %= Bits::kByte;
        return *this;
    }

    template <bool is_const>
    constexpr MemoryBitAddressT<is_const>& MemoryBitAddressT<is_const>::operator-=(const Bits& rhs) noexcept
    {
        offset_ = (Bits(Bits::kByte) - offset_) + rhs;
        address_ -= ToBytesFloor(offset_ - 1_Bits);
        offset_ = (Bits(Bits::kByte) - (offset_ % Bits::kByte)) % Bits::kByte;
        return *this;
    }

    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator==(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept
    {
        return (lhs.GetBaseAddress() == rhs.GetBaseAddress()) && (lhs.GetOffset() == rhs.GetOffset());
    }

    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator!=(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator>(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept
    {
        return (lhs.GetBaseAddress() > rhs.GetBaseAddress()) ||
            (lhs.GetBaseAddress() == rhs.GetBaseAddress() && lhs.GetOffset() > rhs.GetOffset());
    }

    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator<(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept
    {
        return (lhs.GetBaseAddress() < rhs.GetBaseAddress()) ||
            (lhs.GetBaseAddress() == rhs.GetBaseAddress() && lhs.GetOffset() < rhs.GetOffset());
    }

    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator>=(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept
    {
        return (lhs.GetBaseAddress() > rhs.GetBaseAddress()) ||
            (lhs.GetBaseAddress() == rhs.GetBaseAddress() && lhs.GetOffset() >= rhs.GetOffset());
    }

    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator<=(const MemoryBitAddressT<is_lhs_const>& lhs, const MemoryBitAddressT<is_rhs_const>& rhs) noexcept
    {
        return (lhs.GetBaseAddress() < rhs.GetBaseAddress()) ||
            (lhs.GetBaseAddress() == rhs.GetBaseAddress() && lhs.GetOffset() <= rhs.GetOffset());
    }

    template <bool is_const>
    constexpr MemoryBitAddressT<is_const> operator+(const MemoryBitAddressT<is_const>& lhs, const Bits& rhs) noexcept
    {
        return MemoryBitAddressT<is_const>(lhs) += rhs;
    }

    template <bool is_const>
    constexpr MemoryBitAddressT<is_const> operator-(const MemoryBitAddressT<is_const>& lhs, const Bits& rhs) noexcept
    {
        return MemoryBitAddressT<is_const>(lhs) -= rhs;
    }

    template <bool is_const>
    inline std::ostream& operator<<(std::ostream& lhs, const MemoryBitAddressT<is_const>& rhs)
    {
        return lhs << rhs.GetBaseAddress() << ":" << rhs.GetOffset();
    }
}

