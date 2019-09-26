
/// \file windows_network.h
/// \brief This header is part of the synchrony network system. It contains basic functionalities used to startup and shutdown the networking service.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#ifdef _WIN64

#include <optional>
#include <string>

#include "synchrony/network/network_address.h"

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS NETWORK                                                      */
    /************************************************************************/

    /// \brief Contains network-related functions under Windows.
    /// \author Raffaele D. Facendola - September 2019.
    namespace WindowsNetwork
    {
        /// \brief Startup the networking service.
        /// \return Returns true if the networking service could be initialized successfully, returns false otherwise.
        bool Startup();

        /// \brief Shutdown the networking service.
        /// If the networking service wasn't running this method does nothing.
        /// \return Returns true if the networking service could be shutdown successfully, returns false otherwise.
        bool Shutdown();

        /// \brief Create a network address from its standard text presentation form.
        /// \remarks This method supports IPv4-mapped IPv6 addresses.
        /// \return Returns the network address if the text could be converted, returns an empty value otherwise.
        std::optional<NetworkAddress> MakeNetworkAddress(const std::string& address);

        /// \brief Convert a network address to its standard text presentation form.
        std::string NetworkAddressToString(const NetworkAddress& address);

    }

    namespace PlatformNetwork = WindowsNetwork;
}

#endif
