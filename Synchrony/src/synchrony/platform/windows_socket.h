
/// \file network.h
/// \brief This header is part of the synchrony network system. It contains basic functionalities used to startup and shutdown the networking service.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#ifdef _WIN64

#include <memory>

#include "synchrony/network/network_endpoint.h"
#include "synchrony/socket/tcp_socket.h"
#include "synchrony/socket/udp_socket.h"

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS TCP                                                          */
    /************************************************************************/

    /// \brief Contains network-related functions under Windows OS.
    /// \author Raffaele D. Facendola - September 2019.
    namespace WindowsTCP
    {
        /// \brief Connect to a remote TCP server.
        /// \param server Server interface to connect to.
        ///\ return Returns a valid TCP socket if a connection could be established with the remote server, returns false otherwise.
        std::unique_ptr<TCPSocket> Connect(const NetworkEndpoint& server);

        /// \brief Start a new TCP server.
        /// \param listen_interface Interface to listen to.
        /// \param backlog Maximum number of simultaneous connections to the server.
        /// \return Returns a valid TCP server if the server could be created, returns nullptr otherwise.
        std::unique_ptr<TCPServer> StartServer(const NetworkEndpoint& listen_interface, std::int32_t backlog);
    }

    namespace PlatformTCP = WindowsTCP;

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
