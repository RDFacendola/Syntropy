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
            if (WindowsNetwork::Bind(tcp_socket, local) != SOCKET_ERROR &&
                listen(tcp_socket, backlog) != SOCKET_ERROR)
            {
                return std::make_unique<WindowsTCPServer>(tcp_socket);
            }

            closesocket(tcp_socket);
        }

        return nullptr;
    }

}

#endif
