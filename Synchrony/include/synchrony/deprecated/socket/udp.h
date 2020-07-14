
/// \file udp.h
/// \brief This header is part of the synchrony socket system. It contains interfaces for User Datagram Protocol sockets.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/memory/memory_span.h"

#include "synchrony/network/network_endpoint.h"

namespace synchrony
{
    /************************************************************************/
    /* UDP SOCKET                                                           */
    /************************************************************************/

    /// \brief Base interface for UDP sockets.
    /// \author Raffaele D. Facendola - September 2019.
    class UDPSocket
    {
    public:

        /// \brief Virtual destructor.
        virtual ~UDPSocket() = default;

        /// \brief Send a datagram to a remote peer.
        /// \param remote Address of the peer to send the data to.
        /// \param datagram Datagram to send.
        /// \return Returns true if the datagram could be sent entirely, returns false otherwise.
        virtual bool Send(const NetworkEndpoint& remote, const syntropy::ConstMemoryRange& datagram) = 0;

        /// \brief Receive a datagram from a remote peer.
        /// This method will block until some data is received from the remote peer.
        /// \param remote If the method succeeds contains the address of the peer the datagram originated from.
        /// \param datagram Datagram buffer to receive in.
        /// \return If a datagram could be received reduce datagram capacity to fit that amount and returns true, otherwise return false.
        virtual bool Receive(NetworkEndpoint& remote, syntropy::MemoryRange& datagram) = 0;

        /// \brief Get the local endpoint the socket is bound to.
        /// \return Returns the local endpoint the socket is bound to.
        virtual NetworkEndpoint GetLocalEndpoint() const = 0;

    };

    /************************************************************************/
    /* UDP CHANNEL                                                          */
    /************************************************************************/

    /// \brief Base interface for UDP channels.
    /// UDP channels are UDP sockets that are virtually connected to a single remote host chosen during initialization.
    /// \author Raffaele D. Facendola - September 2019.
    class UDPChannel
    {
    public:

        /// \brief Virtual destructor.
        virtual ~UDPChannel() = default;

        /// \brief Send a datagram to the remote peer.
        /// \param datagram Datagram to send.
        /// \return Returns true if the datagram could be sent entirely, returns false otherwise.
        virtual bool Send(const syntropy::ConstMemoryRange& datagram) = 0;

        /// \brief Receive a datagram from the remote peer.
        /// This method will block until some data is received from the remote peer.
        /// \param datagram Datagram buffer to receive in.
        /// \return If a datagram could be received reduce datagram capacity to fit that amount and returns true, otherwise return false.
        virtual bool Receive(syntropy::MemoryRange& datagram) = 0;

        /// \brief Get the local endpoint the socket is bound to.
        /// \return Returns the local endpoint the socket is bound to.
        virtual NetworkEndpoint GetLocalEndpoint() const = 0;

        /// \brief Get the remote endpoint the socket is bound to.
        /// \return Returns the remote endpoint the socket is bound to.
        virtual NetworkEndpoint GetRemoteEndpoint() const = 0;

    };

    /************************************************************************/
    /* UDP                                                                  */
    /************************************************************************/

    /// \brief Exposes UDP-related functionalities.
    /// \author Raffaele D. Facendola - September 2019.
    namespace UDP
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
}
