
/// \file network.h
/// \brief This header is part of the synchrony network system. It contains basic functionalities used to startup and shutdown the networking service.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#ifdef _WIN64

#include <optional>
#include <string>

namespace synchrony
{
    struct IPv4Address;
    struct IPv6Address;

    struct NetworkPort;

    struct IPv4Endpoint;
    struct IPv6Endpoint;

    /************************************************************************/
    /* WINDOWS NETWORK                                                      */
    /************************************************************************/

    /// \brief Contains network-related functions under Windows OS.
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

        /// \brief Create an IPv4 address from its standard text presentation form.
        /// \return Returns the IPv4 address if the text could be converted, returns an empty value otherwise.
        std::optional<IPv4Address> MakeIPv4Address(const std::string& address);

        /// \brief Create an IPv6 address from its standard text presentation form.
        /// \return Returns the IPv6 address if the text could be converted, returns an empty value otherwise.
        std::optional<IPv6Address> MakeIPv6Address(const std::string& address);

        /// \brief Convert an IPv4 address to its standard text presentation form.
        std::string AddressToString(const IPv4Address& address);

        /// \brief Convert an IPv6 address to its standard text presentation form.
        std::string AddressToString(const IPv6Address& address);

    }

    namespace PlatformNetwork = WindowsNetwork;
}

#endif
