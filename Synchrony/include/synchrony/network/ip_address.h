
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

}

