
/// \file memory_address.h
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
    /* MEMORY ADDRESS                                                       */
    /************************************************************************/

    /// \brief Represents an address.
    /// This type is meant to be a strongly-typed replacement for void*, uintptr_t and intptr_t.
    /// \author Raffaele D. Facendola - August 2018
    class MemoryAddress
    {
    public:

        /// \brief Create a en empty address.
        constexpr MemoryAddress() = default;

        /// \brief Create a new address.
        constexpr MemoryAddress(void* address);

        /// \brief Create a en empty address.
        constexpr MemoryAddress(std::nullptr_t);

        /// \brief Create a new address.
        constexpr MemoryAddress(uintptr_t address);

        /// \brief Create a new address.
        constexpr MemoryAddress(intptr_t address);

        /// \brief Default copy-constructor.
        constexpr MemoryAddress(const MemoryAddress&) = default;

        /// \brief Default assignment operator.
        MemoryAddress& operator=(const MemoryAddress&) = default;

        /// \brief Get the underlying pointer.
        /// \return Returns the underlying pointer.
        constexpr operator void*() const noexcept;

        /// \brief Convert the address to an unsigned numeric value.
        /// \return Returns an unsigned number that represents the underlying address.
        constexpr operator uintptr_t() const noexcept;

        /// \brief Convert the address to an signed numeric value.
        /// \return Returns an signed number that represents the underlying address.
        constexpr operator intptr_t() const noexcept;

        /// \brief Get the underlying pointer.
        /// \return Returns the underlying pointer.
        constexpr void* operator*() const noexcept;

        /// \brief Get the underlying strongly-type pointer.
        /// \return Returns the underlying strongly-type pointer.
        template <typename TType>
        constexpr TType* As() const noexcept;

        /// \brief Move the address forward.
        /// \return Returns a reference to this element.
        constexpr MemoryAddress& operator+=(const Bytes& rhs) noexcept;

        /// \brief Move the address backwards.
        /// \return Returns a reference to this element.
        constexpr MemoryAddress& operator-=(const Bytes& rhs) noexcept;

        /// \brief Check whether this address is aligned to the provided amount.
        /// \param Returns true if the address is aligned to alignment, returns false otherwise.
        constexpr bool IsAlignedTo(Alignment alignment) const noexcept;

        /// \brief Align this address up to the provided alignment.
        /// \return Returns a new address which is guaranteed to be aligned to alignment and equal or greater than this address.
        constexpr MemoryAddress GetAligned(Alignment alignment) const noexcept;

        /// \brief Align this address down to the provided alignment.
        /// \return Returns a new address which is guaranteed to be aligned to alignment and equal or less than this address.
        constexpr MemoryAddress GetAlignedDown(Alignment alignment) const noexcept;

    private:

        void* address_ = nullptr;               ///< \brief Underlying address.

    };

    /// \brief Equality comparison for MemoryAddress.
    /// \return Returns true if lhs and rhs refer to the same address, returns false otherwise.
    constexpr bool operator==(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept;

    /// \brief Inequality comparison for MemoryAddress.
    /// \return Returns true if lhs and rhs refer to different address, returns false otherwise.
    constexpr bool operator!=(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept;

    /// \brief Greater-than comparison for MemoryAddress.
    /// \return Returns true if lhs refers to an address that is strictly greater than rhs, returns false otherwise.
    constexpr bool operator>(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept;

    /// \brief Less-than comparison for MemoryAddress.
    /// \return Returns true if lhs refers to an address that is strictly less than rhs, returns false otherwise.
    constexpr bool operator<(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept;

    /// \brief Greater-or-equal comparison for MemoryAddress.
    /// \return Returns true if lhs refers to an address that is equal or greater than rhs, returns false otherwise.
    constexpr bool operator>=(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept;

    /// \brief Less-or-equal comparison for MemoryAddress.
    /// \return Returns true if lhs refers to an address that is equal or less than rhs, returns false otherwise.
    constexpr bool operator<=(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept;

    /// \brief Move an address forward.
    /// \return Returns an address which is equal to lhs moved forward by rhs bytes.
    constexpr MemoryAddress operator+(const MemoryAddress& lhs, const Bytes& rhs) noexcept;

    /// \brief Move an address backward.
    /// \return Returns an address which is equal to lhs moved backward by rhs bytes.
    constexpr MemoryAddress operator-(const MemoryAddress& lhs, const Bytes& rhs) noexcept;

    /// \brief Get the difference of two memory addresses.
    /// \return Returns a the signed difference between lhs and rhs.
    constexpr ptrdiff_t operator-(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept;

    /// \brief Stream insertion for MemoryAddress.
    std::ostream& operator<<(std::ostream& lhs, const MemoryAddress& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr MemoryAddress::MemoryAddress(void* address)
        : address_(address)
    {

    }

    constexpr MemoryAddress::MemoryAddress(std::nullptr_t)
        : MemoryAddress()
    {

    }

    constexpr MemoryAddress::MemoryAddress(uintptr_t address)
        : address_(reinterpret_cast<void*>(address))
    {

    }

    constexpr MemoryAddress::MemoryAddress(intptr_t address)
        : address_(reinterpret_cast<void*>(address))
    {

    }

    constexpr MemoryAddress::operator void*() const noexcept
    {
        return address_;
    }

    constexpr MemoryAddress::operator uintptr_t() const noexcept
    {
        return reinterpret_cast<uintptr_t>(address_);
    }

    constexpr MemoryAddress::operator intptr_t() const noexcept
    {
        return reinterpret_cast<intptr_t>(address_);
    }

    constexpr void* MemoryAddress::operator*() const noexcept
    {
        return address_;
    }

    template <typename TType>
    constexpr TType* MemoryAddress::As() const noexcept
    {
        return reinterpret_cast<TType*>(address_);
    }

    constexpr MemoryAddress& MemoryAddress::operator+=(const Bytes& rhs) noexcept
    {
        address_ = reinterpret_cast<int8_t*>(address_) + std::size_t(rhs);
        return *this;
    }

    constexpr MemoryAddress& MemoryAddress::operator-=(const Bytes& rhs) noexcept
    {
        address_ = reinterpret_cast<int8_t*>(address_) - std::size_t(rhs);
        return *this;
    }

    constexpr bool MemoryAddress::IsAlignedTo(Alignment alignment) const noexcept
    {
        return (uintptr_t(*this) & (std::size_t(alignment) - 1u)) == 0u;
    }

    constexpr MemoryAddress MemoryAddress::GetAligned(Alignment alignment) const noexcept
    {
        auto alignment_mask = std::size_t(alignment) - 1u;
        return (uintptr_t(*this) + alignment_mask) & ~alignment_mask;
    }

    constexpr MemoryAddress MemoryAddress::GetAlignedDown(Alignment alignment) const noexcept
    {
        auto alignment_mask = std::size_t(alignment) - 1u;

        return uintptr_t(*this) & ~alignment_mask;
    }

    constexpr bool operator==(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept
    {
        return uintptr_t(lhs) == uintptr_t(rhs);
    }

    constexpr bool operator!=(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept
    {
        return uintptr_t(lhs) > uintptr_t(rhs);
    }

    constexpr bool operator<(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept
    {
        return uintptr_t(lhs) < uintptr_t(rhs);
    }

    constexpr bool operator>=(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept
    {
        return uintptr_t(lhs) >= uintptr_t(rhs);
    }

    constexpr bool operator<=(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept
    {
        return uintptr_t(lhs) <= uintptr_t(rhs);
    }

    constexpr MemoryAddress operator+(const MemoryAddress& lhs, const Bytes& rhs) noexcept
    {
        return MemoryAddress(lhs) += rhs;
    }

    constexpr MemoryAddress operator-(const MemoryAddress& lhs, const Bytes& rhs) noexcept
    {
        return MemoryAddress(lhs) -= rhs;
    }

    constexpr ptrdiff_t operator-(const MemoryAddress& lhs, const MemoryAddress& rhs) noexcept
    {
        return intptr_t(lhs) - intptr_t(rhs);
    }

    inline std::ostream& operator<<(std::ostream& lhs, const MemoryAddress& rhs)
    {
        return lhs << uintptr_t(rhs);
    }
}

