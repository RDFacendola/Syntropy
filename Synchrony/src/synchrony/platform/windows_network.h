
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

        /// \brief Converts an IPv4 Internet network address in its standard text presentation form into its numeric binary form.
        /// \param address Address to convert.
        /// \return Returns the IPv4 address if the address could be converted, returns false otherwise.
        std::optional<IPv4Address> StringToIPv4Address(const std::string& address);

        /// \brief Converts an IPv6 Internet network address in its standard text presentation form into its numeric binary form.
        /// \param address Address to convert.
        /// \return Returns the IPv6 address if the address could be converted, returns false otherwise.
        std::optional<IPv6Address> StringToIPv6Address(const std::string& address);

        /// \brief Convert the IP address to its standard text presentation form.
        std::string IPv4AddressToString(const IPv4Address& address);

        /// \brief Convert the IP address to its standard text presentation form.
        std::string IPv6AddressToString(const IPv6Address& address);
    }

    namespace PlatformNetwork = WindowsNetwork;
}

#endif
