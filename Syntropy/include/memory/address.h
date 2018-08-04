
/// \file address.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of the Address type and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <cstdint>

#include "diagnostics/assert.h"
#include "memory/bytes.h"
#include "memory/alignment.h"

namespace syntropy
{
    /************************************************************************/
    /* ADDRESS                                                              */
    /************************************************************************/

    /// \brief Represents an address.
    /// This type is meant to be a strongly-typed replacement for void*, uintptr_t and intptr_t.
    /// \author Raffaele D. Facendola - August 2018
    class Address
    {
    public:

        /// \brief Create a en empty address.
        constexpr Address() = default;

        /// \brief Create a new address.
        constexpr Address(void* address);

        /// \brief Create a new address.
        constexpr Address(uintptr_t address);

        /// \brief Create a new address.
        constexpr Address(intptr_t address);

        /// \brief Default copy-constructor.
        constexpr Address(const Address&) = default;

        /// \brief Default assignment operator.
        Address& operator=(const Address&) = default;

        /// \brief Get the underlying pointer.
        /// \return Returns the underlying pointer.
        constexpr operator void*() const noexcept;

        /// \brief Convert the address to an unsigned numeric value.
        /// \return Returns an unsigned number that represents the underlying address.
        constexpr operator uintptr_t() const noexcept;

        /// \brief Convert the address to an signed numeric value.
        /// \return Returns an signed number that represents the underlying address.
        constexpr operator intptr_t() const noexcept;

        /// \brief Move the address forward.
        /// \return Returns a reference to this element.
        constexpr Address& operator+=(const Bytes& rhs) noexcept;

        /// \brief Move the address backwards.
        /// \return Returns a reference to this element.
        constexpr Address& operator-=(const Bytes& rhs) noexcept;

        /// \brief Check whether this address is aligned to the provided amount.
        /// \param Returns true if the address is aligned to alignment, returns false otherwise.
        constexpr bool IsAlignedTo(Alignment alignment) const noexcept;

        /// \brief Align this address up to the provided alignment.
        /// \return Returns a new address which is guaranteed to be aligned to alignment and equal or greater than this address.
        constexpr Address GetAligned(Alignment alignment) noexcept;

        /// \brief Align this address down to the provided alignment.
        /// \return Returns a new address which is guaranteed to be aligned to alignment and equal or less than this address.
        constexpr Address GetAlignedDown(Alignment alignment) noexcept;

    private:

        void* address_ = nullptr;               ///< \brief Underlying address.

    };

    /// \brief Equality comparison for Address.
    /// \return Returns true if lhs and rhs refer to the same alignment, returns false otherwise.
    constexpr bool operator==(const Address& lhs, const Address& rhs) noexcept;

    /// \brief Inequality comparison for Alignment.
    /// \return Returns true if lhs and rhs refer to different alignments, returns false otherwise.
    constexpr bool operator!=(const Address& lhs, const Address& rhs) noexcept;

    /// \brief Greater-than comparison for Alignment.
    /// \return Returns true if lhs refers to an alignment that is strictly greater than rhs, returns false otherwise.
    constexpr bool operator>(const Address& lhs, const Address& rhs) noexcept;

    /// \brief Less-than comparison for Alignment.
    /// \return Returns true if lhs refers to an alignment that is strictly less than rhs, returns false otherwise.
    constexpr bool operator<(const Address& lhs, const Address& rhs) noexcept;

    /// \brief Greater-or-equal comparison for Alignment.
    /// \return Returns true if lhs refers to an alignment that is equal or greater than rhs, returns false otherwise.
    constexpr bool operator>=(const Address& lhs, const Address& rhs) noexcept;

    /// \brief Less-or-equal comparison for Alignment.
    /// \return Returns true if lhs refers to an alignment that is equal or less than rhs, returns false otherwise.
    constexpr bool operator<=(const Address& lhs, const Address& rhs) noexcept;

    /// \brief Sum two bytes amount.
    /// \return Returns a memory amount which is the sum of lhs and rhs.
    constexpr Address operator+(const Address& lhs, const Bytes& rhs) noexcept;

    /// \brief Subtract two bytes amount.
    /// \return Returns a memory amount which is the difference of lhs and rhs.
    constexpr Address operator-(const Address& lhs, const Bytes& rhs) noexcept;

    /// \brief Subtract two bytes amount.
    /// \return Returns a memory amount which is the difference of lhs and rhs.
    constexpr ptrdiff_t operator-(const Address& lhs, const Address& rhs) noexcept;

    /// \brief Stream insertion for Address.
    std::ostream& operator<<(std::ostream& lhs, const Address& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr Address::Address(void* address)
        : address_(address)
    {

    }

    constexpr Address::Address(uintptr_t address)
        : address_(reinterpret_cast<void*>(address))
    {

    }

    constexpr Address::Address(intptr_t address)
        : address_(reinterpret_cast<void*>(address))
    {

    }

    constexpr Address::operator void*() const noexcept
    {
        return address_;
    }

    constexpr Address::operator uintptr_t() const noexcept
    {
        return reinterpret_cast<uintptr_t>(address_);
    }

    constexpr Address::operator intptr_t() const noexcept
    {
        return reinterpret_cast<intptr_t>(address_);
    }

    constexpr Address& Address::operator+=(const Bytes& rhs) noexcept
    {
        address_ = reinterpret_cast<int8_t*>(address_) + std::size_t(rhs);
        return *this;
    }

    constexpr Address& Address::operator-=(const Bytes& rhs) noexcept
    {
        address_ = reinterpret_cast<int8_t*>(address_) - std::size_t(rhs);
        return *this;
    }

    constexpr bool Address::IsAlignedTo(Alignment alignment) const noexcept
    {
        return (uintptr_t(*this) & (std::size_t(alignment) - 1u)) == 0u;
    }

    constexpr Address Address::GetAligned(Alignment alignment) noexcept
    {
        auto alignment_mask = std::size_t(alignment) - 1u;

        return (uintptr_t(*this) + alignment_mask) & !alignment_mask;
    }

    constexpr Address Address::GetAlignedDown(Alignment alignment) noexcept
    {
        auto alignment_mask = std::size_t(alignment) - 1u;

        return uintptr_t(*this) & !alignment_mask;
    }

    constexpr bool operator==(const Address& lhs, const Address& rhs) noexcept
    {
        return uintptr_t(lhs) == uintptr_t(rhs);
    }

    constexpr bool operator!=(const Address& lhs, const Address& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(const Address& lhs, const Address& rhs) noexcept
    {
        return uintptr_t(lhs) > uintptr_t(rhs);
    }

    constexpr bool operator<(const Address& lhs, const Address& rhs) noexcept
    {
        return uintptr_t(lhs) < uintptr_t(rhs);
    }

    constexpr bool operator>=(const Address& lhs, const Address& rhs) noexcept
    {
        return uintptr_t(lhs) >= uintptr_t(rhs);
    }

    constexpr bool operator<=(const Address& lhs, const Address& rhs) noexcept
    {
        return uintptr_t(lhs) <= uintptr_t(rhs);
    }

    constexpr Address operator+(const Address& lhs, const Bytes& rhs) noexcept
    {
        return Address(lhs) += rhs;
    }

    constexpr Address operator-(const Address& lhs, const Bytes& rhs) noexcept
    {
        return Address(lhs) -= rhs;
    }

    constexpr ptrdiff_t operator-(const Address& lhs, const Address& rhs) noexcept
    {
        return intptr_t(lhs) - intptr_t(rhs);
    }

    inline std::ostream& operator<<(std::ostream& lhs, const Address& rhs)
    {
        return lhs << uintptr_t(rhs);
    }
}

