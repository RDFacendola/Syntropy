
/// \file ip_address.h
/// \brief This header is part of the synchrony network system. It contains definitions for IP addresses.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <optional>
#include <string>
#include <array>

namespace synchrony
{
    /************************************************************************/
    /* IPv4 ADDRESS                                                         */
    /************************************************************************/

    /// \brief Represents a 32-bit address for the Internet Protocol version 4.
    /// The address is represented in network byte-order.
    /// \author Raffaele D. Facendola - 2019.
    struct IPv4Address
    {
        /// \brief Create an IPv4 address from its standard text presentation form.
        /// \return Returns the IPv4 address if the text could be converted, returns an empty value otherwise.
        static std::optional<IPv4Address> FromString(const std::string& address);

        /// \brief Convert the IP address to its standard text presentation form.
        std::string ToString() const;

        /// \brief Binary address, in network byte-order.
        std::array<std::uint8_t, 4> address_;
    };

    /************************************************************************/
    /* IPv6 ADDRESS                                                         */
    /************************************************************************/

    /// \brief Represents a 128-bit address for the Internet Protocol version 6.
    /// The address is represented in network byte-order.
    /// \author Raffaele D. Facendola - 2019.
    struct IPv6Address
    {
        /// \brief Create an IPv6 address from its standard text presentation form.
        /// \return Returns the IPv6 address if the text could be converted, returns an empty value otherwise.
        static std::optional<IPv6Address> FromString(const std::string& address);

        /// \brief Convert the IP address to its standard text presentation form.
        std::string ToString() const;

        /// \brief Binary address, in network byte-order.
        std::array<std::uint8_t, 16> address_;
    };

}

