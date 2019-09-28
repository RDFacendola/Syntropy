
/// \file network_address.h
/// \brief This header is part of the synchrony network system. It contains definitions for network addresses.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <optional>
#include <string>
#include <ostream>

namespace synchrony
{
    /************************************************************************/
    /* NETWORK ADDRESS                                                      */
    /************************************************************************/

    /// \brief Represents a network address.
    /// The address refers to the the Internet Protocol version 6 and supports IPv4-mapped addresses.
    /// \author Raffaele D. Facendola - 2019.
    struct NetworkAddress
    {
        /// \brief Create the local host \ loopback address.
        static constexpr NetworkAddress Loopback();

        /// \brief Create the 'any' address.
        static constexpr NetworkAddress Any();

        /// \brief Create an address from its standard text presentation form.
        /// \remarks This method supports IPv4-mapped IPv6 addresses.
        /// \return Returns the address if the provided string could be converted, returns an empty value otherwise.
        static std::optional<NetworkAddress> FromString(const std::string& address);

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

    /// \brief Stream insertion for NetworkAddress.
    std::ostream& operator<<(std::ostream& out, const NetworkAddress& address);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // NetworkAddress.

    constexpr NetworkAddress NetworkAddress::Loopback()
    {
        return { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 1u };
    }

    constexpr NetworkAddress NetworkAddress::Any()
    {
        return { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };
    }

    inline std::ostream& operator<<(std::ostream& out, const NetworkAddress& address)
    {
        out << address.ToString();
        return out;
    }

}

