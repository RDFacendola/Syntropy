
/// \file ip_endpoint.h
/// \brief This header is part of the synchrony network system. It contains definitions for IP endpoints.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <optional>
#include <string>

#include "synchrony/network/ip_address.h"

namespace synchrony
{
    /************************************************************************/
    /* PORT                                                                 */
    /************************************************************************/

    /// \brief Type alias for network ports.
    using Port = std::uint16_t;

    /************************************************************************/
    /* IPv4 ENDPOINT                                                        */
    /************************************************************************/

    /// \brief Represents an IPv4 endpoint.
    /// \author Raffaele D. Facendola - 2019.
    struct IPv4Endpoint
    {
        /// \brief Create an endpoint from its standard text presentation form.
        /// \return Returns the endpoint if the provided string could be converted, returns an empty value otherwise.
        static std::optional<IPv4Endpoint> FromString(const std::string& address, Port port);

        /// \brief Convert the endpoint to its standard text presentation form.
        std::string ToString() const;

        /// \brief Endpoint address.
        IPv4Address address_;

        /// \brief Service port.
        Port port_;
    };

    /************************************************************************/
    /* IPv6 ENDPOINT                                                        */
    /************************************************************************/

    /// \brief Represents an IPv6 endpoint.
    /// \author Raffaele D. Facendola - 2019.
    struct IPv6Endpoint
    {
        /// \brief Create an endpoint from its standard text presentation form.
        /// \return Returns the endpoint if the provided string could be converted, returns an empty value otherwise.
        static std::optional<IPv6Endpoint> FromString(const std::string& address, Port port);

        /// \brief Convert the endpoint to its standard text presentation form.
        std::string ToString() const;

        /// \brief Endpoint address.
        IPv6Address address_;

        /// \brief Service port.
        Port port_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // IPv4Endpoint.

    inline std::optional<IPv4Endpoint> IPv4Endpoint::FromString(const std::string& address, Port port)
    {
        if (auto ipv4 = IPv4Address::FromString(address))
        {
            return IPv4Endpoint{ *ipv4, port };
        }

        return {};
    }

    inline std::string IPv4Endpoint::ToString() const
    {
        return address_.ToString() + ":" + std::to_string(port_);
    }

    // IPv6Endpoint.

    inline std::optional<IPv6Endpoint> IPv6Endpoint::FromString(const std::string& address, Port port)
    {
        if (auto ipv6 = IPv6Address::FromString(address))
        {
            return IPv6Endpoint{ *ipv6, port };
        }

        return {};
    }

    inline std::string IPv6Endpoint::ToString() const
    {
        return "[" + address_.ToString() + "]:" + std::to_string(port_);
    }
}

