#include "synchrony/platform/windows_udp.h"

#include "syntropy/diagnostics/diagnostics.h"

#include "synchrony/platform/windows_socket.h"

#ifdef _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS UDP SOCKET                                                   */
    /************************************************************************/

    /// \brief Wraps a UDP socket under Windows.
    /// \author Raffaele D. Facendola - September 2019.
    class WindowsUDPSocket : public UDPSocket
    {
    public:

        /// \brief Create a new UDP socket.
        /// \param tcp_socket Underlying UDP socket.
        WindowsUDPSocket(SOCKET udp_socket);

        /// \brief Virtual destructor. Close any open connection.
        virtual ~WindowsUDPSocket();

        virtual bool Send(const NetworkEndpoint& remote, const syntropy::ConstMemoryRange& datagram) override;

        virtual bool Receive(NetworkEndpoint& remote, syntropy::MemoryRange& datagram) override;

    private:

        /// \brief Underlying socket.
        SOCKET udp_socket_{ INVALID_SOCKET };
    };

    /************************************************************************/
    /* WINDOWS UDP CHANNEL                                                  */
    /************************************************************************/

    /// \brief Wraps a UDP channel under Windows.
    /// \author Raffaele D. Facendola - September 2019.
    class WindowsUDPChannel : public UDPChannel
    {
    public:

        /// \brief Create a new UDP channel
        /// \param udp_socket Underlying UDP socket.
        WindowsUDPChannel(SOCKET udp_socket);

        /// \brief Virtual destructor.
        virtual ~WindowsUDPChannel();

        virtual bool Send(const syntropy::ConstMemoryRange& datagram) override;

        virtual bool Receive(syntropy::MemoryRange& datagram) override;

    private:

        /// \brief Underlying socket.
        SOCKET udp_socket_{ INVALID_SOCKET };
    };

}

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS UDP SOCKET                                                   */
    /************************************************************************/

    WindowsUDPSocket::WindowsUDPSocket(SOCKET udp_socket)
        : udp_socket_(udp_socket)
    {

    }

    WindowsUDPSocket::~WindowsUDPSocket()
    {
        closesocket(udp_socket_);
    }

    bool WindowsUDPSocket::Send(const NetworkEndpoint& remote, const syntropy::ConstMemoryRange& datagram)
    {
        auto send_buffer = datagram.Begin().As<char>();
        auto send_size = static_cast<int>(std::size_t(datagram.GetSize()));
        auto send_address = WindowsNetwork::ToSockAddr(remote);

        if (auto sent_amount = sendto(udp_socket_, send_buffer, send_size, 0, reinterpret_cast<SOCKADDR*>(&send_address), sizeof(send_address));
            sent_amount != SOCKET_ERROR)
        {
            SYNTROPY_ASSERT(sent_amount == send_size);          // Datagram-oriented protocols are supposed to preserve datagram boundaries!

            return true;
        }

        return false;
    }

    bool WindowsUDPSocket::Receive(NetworkEndpoint& remote, syntropy::MemoryRange& datagram)
    {
        auto receive_buffer = datagram.Begin().As<char>();
        auto receive_size = int(std::size_t(datagram.GetSize()));
        auto receive_address = SOCKADDR_IN6{};
        auto receive_address_size = int(sizeof(receive_address));

        if (auto receive_amount = recvfrom(udp_socket_, receive_buffer, receive_size, 0, reinterpret_cast<SOCKADDR*>(&receive_address), &receive_address_size);
            receive_amount != SOCKET_ERROR && receive_address.sin6_family == AF_INET6)
        {
            remote = WindowsNetwork::FromSockAddr(receive_address);
            datagram = syntropy::MemoryRange(datagram.Begin(), datagram.Begin() + syntropy::Bytes(receive_amount));

            return true;
        }

        return false;
    }

    /************************************************************************/
    /* WINDOWS UDP CHANNEL                                                  */
    /************************************************************************/

    WindowsUDPChannel::WindowsUDPChannel(SOCKET udp_socket)
        : udp_socket_(udp_socket)
    {

    }

    WindowsUDPChannel::~WindowsUDPChannel()
    {
        closesocket(udp_socket_);
    }

    bool WindowsUDPChannel::Send(const syntropy::ConstMemoryRange& datagram)
    {
        auto send_buffer = datagram.Begin().As<char>();
        auto send_size = static_cast<int>(std::size_t(datagram.GetSize()));

        if (auto sent_amount = send(udp_socket_, send_buffer, send_size, 0); sent_amount != SOCKET_ERROR)
        {
            SYNTROPY_ASSERT(sent_amount == send_size);          // Datagram-oriented protocols are supposed to preserve datagram boundaries!

            return true;
        }

        return false;
    }

    bool WindowsUDPChannel::Receive(syntropy::MemoryRange& datagram)
    {
        auto receive_buffer = datagram.Begin().As<char>();
        auto receive_size = int(std::size_t(datagram.GetSize()));

        if (auto receive_amount = recv(udp_socket_, receive_buffer, receive_size, 0); receive_amount != SOCKET_ERROR)
        {
            datagram = syntropy::MemoryRange(datagram.Begin(), datagram.Begin() + syntropy::Bytes(receive_amount));

            return true;
        }

        return false;
    }

    /************************************************************************/
    /* WINDOWS UDP                                                          */
    /************************************************************************/

    std::unique_ptr<UDPSocket> WindowsUDP::CreatePeer(const NetworkEndpoint& local)
    {
        if (auto udp_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP); udp_socket != INVALID_SOCKET)
        {
            if (WindowsNetwork::Bind(udp_socket, local) != SOCKET_ERROR)
            {
                return std::make_unique<WindowsUDPSocket>(udp_socket);
            }

            closesocket(udp_socket);
        }

        return nullptr;
    }

    std::unique_ptr<UDPChannel> WindowsUDP::CreatePeer(const NetworkEndpoint& local, const NetworkEndpoint& remote)
    {
        if (auto udp_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP); udp_socket != INVALID_SOCKET)
        {
            if (WindowsNetwork::Bind(udp_socket, local) != SOCKET_ERROR &&
                WindowsNetwork::Connect(udp_socket, remote) != SOCKET_ERROR)
            {
                return std::make_unique<WindowsUDPChannel>(udp_socket);
            }

            closesocket(udp_socket);
        }

        return nullptr;
    }

}

#endif
