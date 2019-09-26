#include "synchrony/platform/windows_socket.h"

#ifdef _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>

#include "syntropy/diagnostics/diagnostics.h"

namespace synchrony
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

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

    /************************************************************************/
    /* WINDOWS TCP SOCKET                                                   */
    /************************************************************************/

    /// \brief Wraps a TCP socket under Windows.
    /// \author Raffaele D. Facendola - September 2019.
    class WindowsTCPSocket : public TCPSocket
    {
    public:

        /// \brief Create a new TCP socket.
        /// \param tcp_socket Underlying TCP socket.
        WindowsTCPSocket(SOCKET tcp_socket);

        /// \brief Virtual destructor. Close any open connection.
        virtual ~WindowsTCPSocket();

        virtual bool Send(syntropy::ConstMemoryRange& buffer) override;

        virtual bool Receive(syntropy::MemoryRange& buffer) override;

    private:

        /// \brief Underlying socket.
        SOCKET tcp_socket_{ INVALID_SOCKET };
    };

    /************************************************************************/
    /* WINDOWS TCP SERVER                                                   */
    /************************************************************************/

    /// \brief Represents a TCP server under Windows.
    class WindowsTCPServer : public TCPServer
    {
    public:

        /// \brief Create a new TCP server.
        /// \param tcp_socket Underlying TCP socket.
        WindowsTCPServer(SOCKET tcp_socket);

        /// \brief Virtual destructor. Close any open connection.
        virtual ~WindowsTCPServer();

        virtual std::unique_ptr<TCPSocket> Accept() override;

    private:

        /// \brief Listener socket.
        SOCKET tcp_socket_{ INVALID_SOCKET };

    };

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

        virtual bool Send(const NetworkEndpoint& peer, const syntropy::ConstMemoryRange& datagram) override;

        virtual bool Receive(NetworkEndpoint& peer, syntropy::MemoryRange& datagram) override;

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

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    SOCKADDR_IN6 ToSockAddr(const NetworkEndpoint& endpoint)
    {
        auto address = SOCKADDR_IN6{};

        address.sin6_family = AF_INET6;
        address.sin6_port = htons(endpoint.port_);

        address.sin6_addr.u.Word[0] = htons(endpoint.address_.a_);
        address.sin6_addr.u.Word[1] = htons(endpoint.address_.b_);
        address.sin6_addr.u.Word[2] = htons(endpoint.address_.c_);
        address.sin6_addr.u.Word[3] = htons(endpoint.address_.d_);
        address.sin6_addr.u.Word[4] = htons(endpoint.address_.e_);
        address.sin6_addr.u.Word[5] = htons(endpoint.address_.f_);
        address.sin6_addr.u.Word[6] = htons(endpoint.address_.g_);
        address.sin6_addr.u.Word[7] = htons(endpoint.address_.h_);

        return address;
    }

    NetworkEndpoint FromSockAddr(const SOCKADDR_IN6& address)
    {
        auto endpoint = NetworkEndpoint{};

        endpoint.port_ = ntohs(address.sin6_port);

        endpoint.address_.a_ = ntohs(address.sin6_addr.u.Word[0]);
        endpoint.address_.b_ = ntohs(address.sin6_addr.u.Word[1]);
        endpoint.address_.c_ = ntohs(address.sin6_addr.u.Word[2]);
        endpoint.address_.d_ = ntohs(address.sin6_addr.u.Word[3]);
        endpoint.address_.e_ = ntohs(address.sin6_addr.u.Word[4]);
        endpoint.address_.f_ = ntohs(address.sin6_addr.u.Word[5]);
        endpoint.address_.g_ = ntohs(address.sin6_addr.u.Word[6]);
        endpoint.address_.h_ = ntohs(address.sin6_addr.u.Word[7]);

        return endpoint;
    }

    int Bind(SOCKET socket, const NetworkEndpoint& endpoint)
    {
        auto address = ToSockAddr(endpoint);

        return bind(socket, reinterpret_cast<SOCKADDR*>(&address), sizeof(address));
    }

    int Connect(SOCKET socket, const NetworkEndpoint& endpoint)
    {
        auto address = ToSockAddr(endpoint);

        return connect(socket, reinterpret_cast<SOCKADDR*>(&address), sizeof(address));
    }

    // WindowsTCPSocket.

    WindowsTCPSocket::WindowsTCPSocket(SOCKET tcp_socket)
        : tcp_socket_(tcp_socket)
    {

    }

    WindowsTCPSocket::~WindowsTCPSocket()
    {
        shutdown(tcp_socket_, SD_BOTH);
        closesocket(tcp_socket_);
    }

    bool WindowsTCPSocket::Send(syntropy::ConstMemoryRange& buffer)
    {
        auto send_buffer = buffer.Begin().As<char>();
        auto send_size = static_cast<int>(std::size_t(buffer.GetSize()));

        if (auto sent_amount = send(tcp_socket_, send_buffer, send_size, 0); sent_amount != SOCKET_ERROR)
        {
            buffer = syntropy::ConstMemoryRange(buffer.Begin() + syntropy::Bytes(sent_amount), buffer.End());

            return true;
        }

        return false;
    }

    bool WindowsTCPSocket::Receive(syntropy::MemoryRange& buffer)
    {
        auto receive_buffer = buffer.Begin().As<char>();
        auto receive_size = static_cast<int>(std::size_t(buffer.GetSize()));

        if (auto receive_amount = recv(tcp_socket_, receive_buffer, receive_size, 0); receive_amount != SOCKET_ERROR)
        {
            buffer = syntropy::MemoryRange(buffer.Begin(), buffer.Begin() + syntropy::Bytes(receive_amount));

            return true;
        }

        return false;
    }

    // WindowsTCPServer.

    WindowsTCPServer::WindowsTCPServer(SOCKET tcp_socket)
        : tcp_socket_(tcp_socket)
    {

    }

    WindowsTCPServer::~WindowsTCPServer()
    {
        closesocket(tcp_socket_);
    }

    std::unique_ptr<TCPSocket> WindowsTCPServer::Accept()
    {
        if (auto server_socket = accept(tcp_socket_, nullptr, nullptr); server_socket != INVALID_SOCKET)
        {
            return std::make_unique<WindowsTCPSocket>(server_socket);
        }

        return nullptr;
    }

    // WindowsUDPSocket.

    WindowsUDPSocket::WindowsUDPSocket(SOCKET udp_socket)
        : udp_socket_(udp_socket)
    {

    }

    WindowsUDPSocket::~WindowsUDPSocket()
    {
        closesocket(udp_socket_);
    }

    bool WindowsUDPSocket::Send(const NetworkEndpoint& peer, const syntropy::ConstMemoryRange& datagram)
    {
        auto send_buffer = datagram.Begin().As<char>();
        auto send_size = static_cast<int>(std::size_t(datagram.GetSize()));
        auto send_address = ToSockAddr(peer);

        if (auto sent_amount = sendto(udp_socket_, send_buffer, send_size, 0, reinterpret_cast<SOCKADDR*>(&send_address), sizeof(send_address));
            sent_amount != SOCKET_ERROR)
        {
            SYNTROPY_ASSERT(sent_amount == send_size);          // Datagram-oriented protocols are supposed to preserve datagram boundaries!

            return true;
        }

        return false;
    }

    bool WindowsUDPSocket::Receive(NetworkEndpoint& peer, syntropy::MemoryRange& datagram)
    {
        auto receive_buffer = datagram.Begin().As<char>();
        auto receive_size = int(std::size_t(datagram.GetSize()));
        auto receive_address = SOCKADDR_IN6{};
        auto receive_address_size = int(sizeof(receive_address));

        if (auto receive_amount = recvfrom(udp_socket_, receive_buffer, receive_size, 0, reinterpret_cast<SOCKADDR*>(&receive_address), &receive_address_size);
            receive_amount != SOCKET_ERROR && receive_address.sin6_family == AF_INET6)
        {
            peer = FromSockAddr(receive_address);
            datagram = syntropy::MemoryRange(datagram.Begin(), datagram.Begin() + syntropy::Bytes(receive_amount));

            return true;
        }

        return false;
    }

    // WindowsUDPChannel.

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

}

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS TCP                                                          */
    /************************************************************************/

    std::unique_ptr<TCPSocket> WindowsTCP::Connect(const NetworkEndpoint& server)
    {
        if (auto tcp_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); tcp_socket != INVALID_SOCKET)
        {
            if (Connect(tcp_socket, server) != SOCKET_ERROR)
            {
                return std::make_unique<WindowsTCPSocket>(tcp_socket);
            }

            closesocket(tcp_socket);
        }

        return nullptr;
    }

    std::unique_ptr<TCPServer> WindowsTCP::StartServer(const NetworkEndpoint& listen_interface, std::int32_t backlog)
    {
        if (auto tcp_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); tcp_socket != INVALID_SOCKET)
        {
            if (Bind(tcp_socket, listen_interface) != SOCKET_ERROR &&
                listen(tcp_socket, backlog) != SOCKET_ERROR)
            {
                return std::make_unique<WindowsTCPServer>(tcp_socket);
            }

            closesocket(tcp_socket);
        }

        return nullptr;
    }

    /************************************************************************/
    /* WINDOWS UDP                                                          */
    /************************************************************************/

    std::unique_ptr<UDPSocket> WindowsUDP::CreatePeer(const NetworkEndpoint& local)
    {
        if (auto udp_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP); udp_socket != INVALID_SOCKET)
        {
            if (Bind(udp_socket, local) != SOCKET_ERROR)
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
            if (Bind(udp_socket, local) != SOCKET_ERROR &&
                Connect(udp_socket, remote) != SOCKET_ERROR)
            {
                return std::make_unique<WindowsUDPChannel>(udp_socket);
            }

            closesocket(udp_socket);
        }

        return nullptr;
    }

}

#endif
