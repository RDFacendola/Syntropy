#include "synchrony/platform/windows_socket.h"

#ifdef _WIN64

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS NETWORK                                                      */
    /************************************************************************/

    SOCKADDR_IN6 WindowsNetwork::ToSockAddr(const NetworkEndpoint& endpoint)
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

    NetworkEndpoint WindowsNetwork::FromSockAddr(const SOCKADDR_IN6& address)
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

    std::optional<NetworkEndpoint> WindowsNetwork::GetRemoteEndpoint(SOCKET socket)
    {
        auto remote_address = SOCKADDR_IN6{};
        auto remote_size = int(sizeof(remote_address));

        if (getpeername(socket, reinterpret_cast<SOCKADDR*>(&remote_address), &remote_size) != SOCKET_ERROR &&
            remote_address.sin6_family == AF_INET6)
        {
            return FromSockAddr(remote_address);
        }

        return {};
    }

    std::optional<NetworkEndpoint> WindowsNetwork::GetLocalEndpoint(SOCKET socket)
    {
        auto local_address = SOCKADDR_IN6{};
        auto local_size = int(sizeof(local_address));

        if (getsockname(socket, reinterpret_cast<SOCKADDR*>(&local_address), &local_size) != SOCKET_ERROR &&
            local_address.sin6_family == AF_INET6)
        {
            return FromSockAddr(local_address);
        }

        return {};
    }

    int WindowsNetwork::Bind(SOCKET socket, const NetworkEndpoint& endpoint)
    {
        auto address = ToSockAddr(endpoint);

        return bind(socket, reinterpret_cast<SOCKADDR*>(&address), sizeof(address));
    }

    int WindowsNetwork::Connect(SOCKET socket, const NetworkEndpoint& endpoint)
    {
        auto address = ToSockAddr(endpoint);

        return connect(socket, reinterpret_cast<SOCKADDR*>(&address), sizeof(address));
    }
}

#endif