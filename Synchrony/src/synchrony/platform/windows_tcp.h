
/// \file windows_tcp.h
/// \brief This header is part of the synchrony socket system. It contains interfaces for Transmission Control Protocol sockets for Windows.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#ifdef _WIN64

#include <memory>

#include "synchrony/network/network_endpoint.h"
#include "synchrony/socket/tcp.h"

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS TCP                                                          */
    /************************************************************************/

    namespace WindowsTCP
    {
        /// \brief Connect to a remote TCP server.
        /// \param local Address used to receive and send data to\from.
        /// \param remote Server address used to receive and send data from\to.
        ///\ return Returns a valid TCP socket if a connection could be established with the remote server, returns false otherwise.
        std::unique_ptr<TCPSocket> Connect(const NetworkEndpoint& local, const NetworkEndpoint& remote);

        /// \brief Start a new TCP server.
        /// \param local Address used to receive and send data to\from.
        /// \param backlog Maximum number of simultaneous connections to the server.
        /// \return Returns a valid TCP server if the server could be created, returns nullptr otherwise.
        std::unique_ptr<TCPServer> StartServer(const NetworkEndpoint& local, std::int32_t backlog);
    }

    namespace PlatformTCP = WindowsTCP;

}

#endif
