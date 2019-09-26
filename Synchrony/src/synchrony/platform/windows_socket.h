
/// \file windows_socket.h
/// \brief This header is part of the synchrony socket system. It contains basic socket functionalities.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#ifdef _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>

#include "synchrony/network/network_endpoint.h"

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS NETWORK                                                      */
    /************************************************************************/

    /// \brief Contains socket-related functions under Windows.
    /// \author Raffaele D. Facendola - September 2019.
    namespace WindowsNetwork
    {
        /// \brief Convert a NetworkEndpoint to a SOCKADDR_IN6 structure.
        /// \param endpoint Endpoint to convert.
        /// \return Returns the converted endpoint.
        SOCKADDR_IN6 ToSockAddr(const NetworkEndpoint& endpoint);

        /// \brief Convert a SockAddr to a NetworkEndpoint.
        /// \param address Address to convert.
        /// \return Returns the converted address.
        NetworkEndpoint FromSockAddr(const SOCKADDR_IN6& address);

        /// \brief Wraps the bind(socket, name, namelen) method.
        /// \param socket Socket to bind.
        /// \param endpoint Interface to bind to.
        int Bind(SOCKET socket, const NetworkEndpoint& endpoint);

        /// \brief Wraps the connect(socket, name, namelen) method.
        /// \param socket Socket to bind.
        /// \param endpoint Interface to bind to.
        int Connect(SOCKET socket, const NetworkEndpoint& endpoint);
    }

}

#endif