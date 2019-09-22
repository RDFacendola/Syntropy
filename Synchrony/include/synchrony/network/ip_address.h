
/// \file ip_address.h
/// \brief This header is part of the synchrony network system. It contains definitions for IP addresses.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <optional>
#include <string>

namespace synchrony
{
    /************************************************************************/
    /* IPv4 ADDRESS                                                         */
    /************************************************************************/

    /// \brief Represents a 32-bit address for the Internet Protocol version 4.
    /// \author Raffaele D. Facendola - 2019.
    struct IPv4Address
    {
        /// \brief Create the local host \ loopback address.
        static constexpr IPv4Address Loopback();

        /// \brief Create the 'any' address.
        static constexpr IPv4Address Any();

        /// \brief Create an address from its standard text presentation form.
        /// \return Returns the address if the provided string could be converted, returns an empty value otherwise.
        static std::optional<IPv4Address> FromString(const std::string& address);

        /// \brief Convert the address to its standard text presentation form.
        std::string ToString() const;

        std::uint8_t a_;
        std::uint8_t b_;
        std::uint8_t c_;
        std::uint8_t d_;
    };

    /************************************************************************/
    /* IPv6 ADDRESS                                                         */
    /************************************************************************/

    /// \brief Represents a 128-bit address for the Internet Protocol version 6.
    /// \author Raffaele D. Facendola - 2019.
    struct IPv6Address
    {
        /// \brief Create the local host \ loopback address.
        static constexpr IPv6Address Loopback();

        /// \brief Create the 'any' address.
        static constexpr IPv6Address Any();

        /// \brief Create an address from its standard text presentation form.
        /// \return Returns the address if the provided string could be converted, returns an empty value otherwise.
        static std::optional<IPv6Address> FromString(const std::string& address);

        /// \brief Convert the address to its standard text presentation form.
        std::string ToString() const;

        std::uint16_t a_;
        std::uint16_t b_;
        std::uint16_t c_;
        std::uint16_t d_;
        std::uint16_t e_;
        std::uint16_t f_;
        std::uint16_t g_;
        std::uint16_t h_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // IPv4Address.

    constexpr IPv4Address IPv4Address::Loopback()
    {
        return { 127u, 0u, 0u, 1u };
    }

    constexpr IPv4Address IPv4Address::Any()
    {
        return { 0u, 0u, 0u, 0u };
    }

    // IPv6Address.

    constexpr IPv6Address IPv6Address::Loopback()
    {
        return { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 1u };
    }

    constexpr IPv6Address IPv6Address::Any()
    {
        return { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };
    }

}

