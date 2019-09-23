
/// \file network_endpoing.h
/// \brief This header is part of the synchrony network system. It contains definitions for network endpoints.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <optional>
#include <string>

#include "synchrony/network/network_address.h"

namespace synchrony
{
    /************************************************************************/
    /* PORT                                                                 */
    /************************************************************************/

    /// \brief Type alias for network ports.
    using NetworkPort = std::uint16_t;

    /************************************************************************/
    /* NETWORK ENDPOINT                                                     */
    /************************************************************************/

    /// \brief Represents an network endpoint.
    /// \author Raffaele D. Facendola - 2019.
    struct NetworkEndpoint
    {
        /// \brief Create an endpoint from its standard text presentation form.
        /// \return Returns the endpoint if the provided string could be converted, returns an empty value otherwise.
        static std::optional<NetworkEndpoint> FromString(const std::string& address, NetworkPort port);

        /// \brief Convert the endpoint to its standard text presentation form.
        std::string ToString() const;

        /// \brief Endpoint address.
        NetworkAddress address_;

        /// \brief Service port.
        NetworkPort port_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // NetworkEndpoint.

    inline std::optional<NetworkEndpoint> NetworkEndpoint::FromString(const std::string& address, NetworkPort port)
    {
        if (auto network_address = NetworkAddress::FromString(address))
        {
            return NetworkEndpoint{ *network_address, port };
        }

        return {};
    }

    inline std::string NetworkEndpoint::ToString() const
    {
        return "[" + address_.ToString() + "]:" + std::to_string(port_);
    }
}

