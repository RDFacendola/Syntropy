
/// \file windows_udp.h
/// \brief This header is part of the synchrony socket system. It contains interfaces for User Datagram Protocol sockets for Windows.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#ifdef _WIN64

#include <memory>

#include "synchrony/network/network_endpoint.h"
#include "synchrony/socket/udp.h"

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS UDP                                                          */
    /************************************************************************/

    namespace WindowsUDP
    {
        /// \brief Create a new UDP peer bound to a local interface.
        /// \param local Address used to receive and send datagram to\from.
        /// \return Returns a valid UDP socket if the peer could be created, returns false otherwise.
        std::unique_ptr<UDPSocket> CreatePeer(const NetworkEndpoint& local);

        /// \brief Create a new UDP peer bound to a local interface.
        /// \param local Address used to receive and send datagram to\from.
        /// \param remote Address used to receive and send datagram from\to.
        /// \return Returns a valid UDP socket if the peer could be created, returns false otherwise.
        std::unique_ptr<UDPChannel> CreatePeer(const NetworkEndpoint& local, const NetworkEndpoint& remote);
    }

    namespace PlatformUDP = WindowsUDP;

}

#endif
