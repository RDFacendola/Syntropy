
/// \file tcp.h
/// \brief This header is part of the synchrony socket system. It contains interfaces for Transmission Control Protocol sockets.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <chrono>

#include "syntropy/memory/memory_range.h"

#include "synchrony/network/network_endpoint.h"

namespace synchrony
{
    /************************************************************************/
    /* TCP SOCKET                                                           */
    /************************************************************************/

        /// \brief Result of a TCP send call.
    enum class TCPSendResult : std::uint8_t
    {
        /// \brief Data was sent successfully.
        kOk = 0,

        /// \brief The socket is disconnected.
        kDisconnected = 2,

        /// \brief Send error.
        kError = 3
    };

    /// \brief Result of a TCP receive call.
    enum class TCPReceiveResult : std::uint8_t
    {
        /// \brief Data was received successfully.
        kOk = 0,

        /// \brief Timeout.
        kTimeout = 1,

        /// \brief The socket is disconnected.
        kDisconnected = 2,

        /// \brief Receive error.
        kError = 3
    };

    /// \brief Base interface for TCP sockets.
    /// \author Raffaele D. Facendola - September 2019.
    class TCPSocket
    {
    public:

        /// \brief Virtual destructor.
        virtual ~TCPSocket() = default;

        /// \brief Send data to the connected host.
        /// \param buffer Buffer to send.
        /// \return If data could be sent reduce buffer capacity to fit the unsent data and returns true, otherwise returns false.
        virtual TCPSendResult Send(syntropy::ConstMemoryRange& buffer) = 0;

        /// \brief Receive data from the connected host.
        /// \param buffer Buffer to receive in.
        /// \return If data could be received reduce buffer capacity to fit that amount and returns TCPReceiveResult::kOk, otherwise returns an error.
        /// \remarks This method blocks until data are received or an exception occurs.
        virtual TCPReceiveResult Receive(syntropy::MemoryRange& buffer) = 0;

        /// \brief Receive data from the connected host.
        /// \param buffer Buffer to receive in.
        /// \param timeout Maximum waiting time for this call.
        /// \return If data could be received within the specified timeout reduce buffer capacity to fit that amount and returns TCPReceiveResult::kOk, otherwise returns an error.
        /// \remarks This method blocks until data are received, an exception occurs or the timeout expires.
        virtual TCPReceiveResult Receive(syntropy::MemoryRange& buffer, std::chrono::milliseconds timeout) = 0;

        /// \brief Get the local endpoint the socket is bound to.
        /// \return Returns the local endpoint the socket is bound to.
        virtual NetworkEndpoint GetLocalEndpoint() const = 0;

        /// \brief Get the remote endpoint the socket is bound to.
        /// \return Returns the remote endpoint the socket is bound to.
        virtual NetworkEndpoint GetRemoteEndpoint() const = 0;

        /// \brief Check whether the socket is connected to a remote endpoint.
        /// \return Returns true if the socket is connected to a remote endpoint, returns false otherwise.
        virtual bool IsConnected() const = 0;

        /// \brief Send data to the connected host.
        /// \param buffer Buffer to send.
        /// \return Returns TCPSendResult::kOk if the entire buffer could be sent, returns an error code otherwise.
        TCPSendResult SendAll(syntropy::ConstMemoryRange buffer);
    };

    /************************************************************************/
    /* TCP SERVER                                                           */
    /************************************************************************/

    /// \brief Base interface for TCP server.
    /// This class is used to accept incoming TCP connections.
    /// \author Raffaele D. Facendola - September 2019.
    class TCPServer
    {
    public:

        /// \brief Virtual destructor.
        virtual ~TCPServer() = default;

        /// \brief Accept an incoming TCP connection.
        /// \return Returns a valid TCP socket if a connection could be established, returns nullptr otherwise.
        /// \remarks This method blocks until a connection is established or an exception occurs.
        virtual std::unique_ptr<TCPSocket> Accept() = 0;

        /// \brief Accept an incoming TCP connection.
        /// \param timeout Maximum waiting time for this call.
        /// \return Returns a valid TCP socket if a connection could be established within the specified timeout, returns nullptr otherwise.
        /// \remarks This method blocks until a connection is established, an exception occurs or the timeout expires.
        virtual std::unique_ptr<TCPSocket> Accept(std::chrono::milliseconds timeout) = 0;
    };

    /************************************************************************/
    /* TCP                                                                  */
    /************************************************************************/

    /// \brief Exposes TCP-related functionalities.
    /// \author Raffaele D. Facendola - September 2019.
    namespace TCP
    {
        /// \brief Connect to a remote TCP server.
        /// \param local Address used to receive and send data to\from.
        /// \param remote Server address used to receive and send data from\to.
        ///\ return Returns a valid TCP socket if a connection could be established with the remote server, returns false otherwise.
        std::unique_ptr<TCPSocket> Connect(const NetworkEndpoint& local, const NetworkEndpoint& remote);

        /// \brief Connect to a remote TCP server.
        /// \param local_address Address used to receive and send data to\from.
        /// \param local_port Port used to receive and send data to\from.
        /// \param remote_address Server address used to receive and send data from\to.
        /// \param local_port Port used to receive and send data from\to.
        ///\ return Returns a valid TCP socket if a connection could be established with the remote server, returns false otherwise.
        std::unique_ptr<TCPSocket> Connect(const std::string& local_address, NetworkPort local_port, const std::string& remote_address, NetworkPort remote_port);

        /// \brief Start a new TCP server.
        /// \param local Address used to receive and send data to\from.
        /// \param backlog Maximum number of simultaneous connections to the server.
        /// \return Returns a valid TCP server if the server could be created, returns nullptr otherwise.
        std::unique_ptr<TCPServer> StartServer(const NetworkEndpoint& local, std::int32_t backlog);

        /// \brief Start a new TCP server.
        /// \param local_address Address used to receive and send data to\from.
        /// \param local_port Port to bound the server to.
        /// \param backlog Maximum number of simultaneous connections to the server.
        /// \return Returns a valid TCP server if the server could be created, returns nullptr otherwise.
        std::unique_ptr<TCPServer> StartServer(const std::string& local_address, NetworkPort local_port, std::int32_t backlog);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TCP.

    inline std::unique_ptr<TCPSocket> TCP::Connect(const std::string& local_address, NetworkPort local_port, const std::string& remote_address, NetworkPort remote_port)
    {
        auto local_endpoint = NetworkEndpoint::FromString(local_address, local_port);
        auto remote_endpoint = NetworkEndpoint::FromString(remote_address, remote_port);

        if (local_endpoint && remote_endpoint)
        {
            return TCP::Connect(*local_endpoint, *remote_endpoint);
        }

        return nullptr;
    }

    inline std::unique_ptr<TCPServer> TCP::StartServer(const std::string& local_address, NetworkPort local_port, std::int32_t backlog)
    {
        if (auto local_endpoint = NetworkEndpoint::FromString(local_address, local_port))
        {
            return TCP::StartServer(*local_endpoint, backlog);
        }

        return nullptr;
    }

    // TCPSocket.

    inline TCPSendResult TCPSocket::SendAll(syntropy::ConstMemoryRange buffer)
    {
        while (buffer)
        {
            if (auto send_result = Send(buffer); send_result != TCPSendResult::kOk)
            {
                return send_result;
            }
        }

        return TCPSendResult::kOk;
    }

}
