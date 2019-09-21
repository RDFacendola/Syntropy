
/// \file network_port.h
/// \brief This header is part of the synchrony network system. It contains definitions for network ports.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>

namespace synchrony
{
    /************************************************************************/
    /* NETWORK PORT                                                         */
    /************************************************************************/

    /// \brief Represents a network endpoint port.
    /// \author Raffaele D. Facendola - September 2019.
    class NetworkPort
    {
    public:

        /// \brief Create a new network port.
        constexpr explicit NetworkPort(std::uint16_t port);

        /// \brief Access the port number.
        constexpr std::uint16_t operator*() const;

    private:

        /// \brief Port number.
        std::uint16_t port_;
    };

    /// \brief Check whether two network ports are equal.
    /// \return Returns true if lhs and rhs both refer to the same port number, returns false otherwise.
    constexpr bool operator==(const NetworkPort& lhs, const NetworkPort& rhs);

    /// \brief Check whether two network ports are different.
    /// \return Returns true if lhs and rhs refer to different ports, returns false otherwise.
    constexpr bool operator!=(const NetworkPort& lhs, const NetworkPort& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr NetworkPort::NetworkPort(std::uint16_t port)
        : port_(port)
    {

    }

    constexpr std::uint16_t NetworkPort::operator*() const
    {
        return port_;
    }

    constexpr bool operator==(const NetworkPort& lhs, const NetworkPort& rhs)
    {
        return *lhs == *rhs;
    }

    constexpr bool operator!=(const NetworkPort& lhs, const NetworkPort& rhs)
    {
        return !(lhs == rhs);
    }

}

