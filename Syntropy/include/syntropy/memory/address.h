
/// \file address.h
/// \brief This header is part of Syntropy memory module. It contains definitions for numeric address values.
///
/// \author Raffaele D. Facendola - August 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    class ImmutableAddress;
    class MutableAddress;

    /************************************************************************/
    /* IMMUTABLE ADDRESS                                                    */
    /************************************************************************/

    /// \brief Numeric representation of an address to an immutable memory location.
    /// \author Raffaele D. Facendola - August 2020.
    class ImmutableAddress
    {
        friend constexpr Int ToInt(ImmutableAddress rhs) noexcept;

    public:

        /// \brief Create an immutable address from a numeric value.
        explicit constexpr ImmutableAddress(Int address) noexcept;

        /// \brief Access the numeric address value.
        constexpr explicit operator Int() const noexcept;

        /// \brief Check whether the address refers to a valid location.
        constexpr operator Bool() const noexcept;

    private:

        // Numeric address value.
        Int address_;
    };

    /************************************************************************/
    /* MUTABLE ADDRESS                                                      */
    /************************************************************************/

    /// \brief Numeric representation of an address to a mutable memory location.
    /// \author Raffaele D. Facendola - August 2020.
    class MutableAddress
    {
    public:

        /// \brief Create an immutable address from a numeric value.
        explicit constexpr MutableAddress(Int address) noexcept;

        /// \brief Access the numeric address value.
        constexpr explicit operator Int() const noexcept;

        /// \brief Implicit conversion to ImmutableAddress.
        constexpr operator ImmutableAddress() const noexcept;

        /// \brief Check whether the address refers to a valid location.
        constexpr operator Bool() const noexcept;

    private:

        // Numeric address value.
        Int address_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Arithmetic.
    // ===========

    /// \brief Move the address forwards.
    constexpr ImmutableAddress operator+(ImmutableAddress lhs, Int rhs) noexcept;

    /// \brief Move the address backwards
    constexpr ImmutableAddress operator-(ImmutableAddress lhs, Int rhs) noexcept;

    /// \brief Bit-wise mask of a numeric address to an immutable memory location.
    constexpr ImmutableAddress operator&(ImmutableAddress lhs, Int rhs) noexcept;

    /// \brief Move the address forwards.
    constexpr MutableAddress operator+(MutableAddress lhs, Int rhs) noexcept;

    /// \brief Move the address backwards
    constexpr MutableAddress operator-(MutableAddress lhs, Int rhs) noexcept;

    /// \brief Bit-wise mask of a numeric address to a mutable memory location.
    constexpr MutableAddress operator&(MutableAddress lhs, Int rhs) noexcept;

    // Conversions.
    // ============

    /// \brief Get the address of a pointer to an immutable memory location.
    ImmutableAddress ToAddress(ImmutableTypelessPtr rhs) noexcept;

    /// \brief Get the address of a pointer to a mutable memory location.
    MutableAddress ToAddress(MutableTypelessPtr rhs) noexcept;

    /// \brief Convert an address to an immutable memory location to a strongly-typed pointer to an immutable instance of type TType.
    /// If the memory location doesn't refer to an instance of TType, accessing the returned value results in undefined behavior.
    template <typename TType = Byte>
    ImmutablePtr<TType> FromAddress(ImmutableAddress rhs) noexcept;

    /// \brief Convert an address to a mutable memory location to a strongly-typed pointer to a mutable instance of type TType.
    /// If the memory location doesn't refer to a mutable instance of TType, accessing the returned value results in undefined behavior.
    template <typename TType = Byte>
    MutablePtr<TType> FromAddress(MutableAddress rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to an address to an immutable memory location.
    constexpr ImmutableAddress ToImmutable(ImmutableAddress rhs) noexcept;

    /// \brief Convert rhs to an address to a mutable memory location.
    /// \remarks If rhs doesn't refer to a mutable memory location, accessing the result of this method results in undefined behavior.
    constexpr MutableAddress ToMutable(ImmutableAddress rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ImmutableAddress.
    // =================

    constexpr  ImmutableAddress::ImmutableAddress(Int address) noexcept
        : address_(address)
    {

    }

    constexpr ImmutableAddress::operator Int() const noexcept
    {
        return address_;
    }

    constexpr ImmutableAddress::operator Bool() const noexcept
    {
        return address_ != ToInt(0);
    }

    // MutableAddress.
    // ===============

    constexpr  MutableAddress::MutableAddress(Int address) noexcept
        : address_(address)
    {

    }

    constexpr MutableAddress::operator Int() const noexcept
    {
        return address_;
    }

    constexpr  MutableAddress::operator ImmutableAddress() const noexcept
    {
        return ImmutableAddress(address_);
    }

    constexpr MutableAddress::operator Bool() const noexcept
    {
        return address_ != ToInt(0);
    }

    // Arithmetic.
    // ===========

    constexpr ImmutableAddress operator+(ImmutableAddress lhs, Int rhs) noexcept
    {
        return ImmutableAddress{ ToInt(lhs) + rhs };
    }

    constexpr ImmutableAddress operator-(ImmutableAddress lhs, Int rhs) noexcept
    {
        return ImmutableAddress{ ToInt(lhs) - rhs };
    }

    constexpr ImmutableAddress operator&(ImmutableAddress lhs, Int rhs) noexcept
    {
        return ImmutableAddress{ ToInt(lhs) & rhs };
    }

    constexpr MutableAddress operator+(MutableAddress lhs, Int rhs) noexcept
    {
        return MutableAddress{ ToInt(lhs) + rhs };
    }

    constexpr MutableAddress operator-(MutableAddress lhs, Int rhs) noexcept
    {
        return MutableAddress{ ToInt(lhs) - rhs };
    }

    constexpr MutableAddress operator&(MutableAddress lhs, Int rhs) noexcept
    {
        return MutableAddress{ ToInt(lhs) & rhs };
    }

    // Conversions.

    constexpr Int ToInt(ImmutableAddress rhs) noexcept
    {
        return rhs.address_;
    }

    inline ImmutableAddress ToAddress(ImmutableTypelessPtr rhs) noexcept
    {
        auto address = reinterpret_cast<std::intptr_t>(rhs);

        return static_cast<ImmutableAddress>(address);
    }

    inline MutableAddress ToAddress(MutableTypelessPtr rhs) noexcept
    {
        auto address = reinterpret_cast<std::intptr_t>(rhs);

        return static_cast<MutableAddress>(address);
    }

    template <typename TType>
    inline ImmutablePtr<TType> FromAddress(ImmutableAddress rhs) noexcept
    {
        auto address = ToInt(rhs);

        return reinterpret_cast<ImmutablePtr<TType>>(address);
    }

    template <typename TType>
    inline MutablePtr<TType> FromAddress(MutableAddress rhs) noexcept
    {
        auto address = ToInt(rhs);

        return reinterpret_cast<MutablePtr<TType>>(address);
    }

    // Access.

    constexpr ImmutableAddress ToImmutable(ImmutableAddress rhs) noexcept
    {
        return rhs;
    }

    constexpr MutableAddress ToMutable(ImmutableAddress rhs) noexcept
    {
        return static_cast<MutableAddress>(ToInt(rhs));
    }

}

// ===========================================================================
