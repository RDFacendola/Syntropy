#include "synchrony/platform/windows_tcp.h"

#include "syntropy/diagnostics/diagnostics.h"

#include "synchrony/platform/windows_socket.h"

#ifdef _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>

namespace synchrony
{
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

        virtual TCPSendResult Send(Syntropy::ConstMemoryRange& buffer) override;

        virtual TCPReceiveResult Receive(Syntropy::MemoryRange& buffer) override;

        virtual TCPReceiveResult Receive(Syntropy::MemoryRange& buffer, std::chrono::milliseconds timeout) override;

        virtual NetworkEndpoint GetLocalEndpoint() const override;

        virtual NetworkEndpoint GetRemoteEndpoint() const override;

        virtual bool IsConnected() const override;

    private:

        /// \brief Underlying socket.
        SOCKET tcp_socket_{ INVALID_SOCKET };

        /// \brief Whether the socket is connected.
        bool is_connected_{ true };
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

        virtual std::unique_ptr<TCPSocket> Accept(std::chrono::milliseconds timeout) override;

    private:

        /// \brief Listener socket.
        SOCKET tcp_socket_{ INVALID_SOCKET };

    };

}

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS TCP SOCKET                                                   */
    /************************************************************************/

    WindowsTCPSocket::WindowsTCPSocket(SOCKET tcp_socket)
        : tcp_socket_(tcp_socket)
    {

    }

    WindowsTCPSocket::~WindowsTCPSocket()
    {
        shutdown(tcp_socket_, SD_BOTH);
        closesocket(tcp_socket_);
    }

    TCPSendResult WindowsTCPSocket::Send(Syntropy::ConstMemoryRange& buffer)
    {
        auto send_buffer = buffer.Begin().As<char>();
        auto send_size = static_cast<int>(*buffer.GetSize());

        auto sent_amount = send(tcp_socket_, send_buffer, send_size, 0);

        if (sent_amount != SOCKET_ERROR)
        {
            buffer = Syntropy::ConstMemoryRange(buffer.Begin() + Syntropy::Bytes(sent_amount), buffer.End());

            return TCPSendResult::kOk;
        }
        else if (auto error = WSAGetLastError(); error == WSAECONNABORTED || error == WSAECONNRESET)
        {
            is_connected_ = false;          // Connection was closed either gracefully or abortively.

            return TCPSendResult::kDisconnected;
        }

        return TCPSendResult::kError;
    }

    TCPReceiveResult WindowsTCPSocket::Receive(Syntropy::MemoryRange& buffer)
    {
        auto receive_buffer = buffer.Begin().As<char>();
        auto receive_size = static_cast<int>(*buffer.GetSize());

        auto receive_amount = recv(tcp_socket_, receive_buffer, receive_size, 0);

        if (receive_amount > 0)
        {
            buffer = Syntropy::MemoryRange(buffer.Begin(), buffer.Begin() + Syntropy::Bytes(receive_amount));

            return TCPReceiveResult::kOk;
        }
        else if (auto error = WSAGetLastError(); receive_amount == 0 || error == WSAECONNABORTED || error == WSAECONNRESET)
        {
            is_connected_ = false;          // Connection was closed either gracefully or abortively.

            return TCPReceiveResult::kDisconnected;
        }

        return TCPReceiveResult::kError;
    }

    TCPReceiveResult WindowsTCPSocket::Receive(Syntropy::MemoryRange& buffer, std::chrono::milliseconds timeout)
    {
        return WindowsNetwork::ReadTimeout(tcp_socket_, timeout) ? Receive(buffer) : TCPReceiveResult::kTimeout;
    }

    NetworkEndpoint WindowsTCPSocket::GetLocalEndpoint() const
    {
        return *WindowsNetwork::GetLocalEndpoint(tcp_socket_);
    }

    NetworkEndpoint WindowsTCPSocket::GetRemoteEndpoint() const
    {
        return *WindowsNetwork::GetRemoteEndpoint(tcp_socket_);
    }

    bool WindowsTCPSocket::IsConnected() const
    {
        return is_connected_;
    }

    /************************************************************************/
    /* WINDOWS TCP SERVER                                                   */
    /************************************************************************/

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
        if (auto tcp_socket = accept(tcp_socket_, nullptr, nullptr); tcp_socket != INVALID_SOCKET)
        {
            return std::make_unique<WindowsTCPSocket>(tcp_socket);
        }

        return nullptr;
    }

    std::unique_ptr<TCPSocket> WindowsTCPServer::Accept(std::chrono::milliseconds timeout)
    {
        return WindowsNetwork::ReadTimeout(tcp_socket_, timeout) ? Accept() : nullptr;
    }

    /************************************************************************/
    /* WINDOWS TCP                                                          */
    /************************************************************************/

    std::unique_ptr<TCPSocket> WindowsTCP::Connect(const NetworkEndpoint& local, const NetworkEndpoint& remote)
    {
        if (auto tcp_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); tcp_socket != INVALID_SOCKET)
        {
            if (WindowsNetwork::Bind(tcp_socket, local) != SOCKET_ERROR &&
                WindowsNetwork::Connect(tcp_socket, remote) != SOCKET_ERROR)
            {
                return std::make_unique<WindowsTCPSocket>(tcp_socket);
            }

            closesocket(tcp_socket);
        }

        return nullptr;
    }

    std::unique_ptr<TCPServer> WindowsTCP::StartServer(const NetworkEndpoint& local, std::int32_t backlog)
    {
        if (auto tcp_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); tcp_socket != INVALID_SOCKET)
        {
            auto disable = int(0);

            if (setsockopt(tcp_socket, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char*>(&disable), sizeof(disable)) != SOCKET_ERROR
                && WindowsNetwork::Bind(tcp_socket, local) != SOCKET_ERROR
                && listen(tcp_socket, backlog) != SOCKET_ERROR)
            {
                return std::make_unique<WindowsTCPServer>(tcp_socket);
            }

            closesocket(tcp_socket);
        }

        return nullptr;
    }

}

#endif
