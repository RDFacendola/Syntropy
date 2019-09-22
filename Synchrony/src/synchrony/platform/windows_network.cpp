#include "synchrony/platform/windows_network.h"

#ifdef _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>

#include "synchrony/network/ip_address.h"

#pragma comment(lib, "Ws2_32.lib")

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS NETWORK                                                      */
    /************************************************************************/

    bool WindowsNetwork::Startup()
    {
        static const auto WSAVersion = MAKEWORD(2, 2);

        WSADATA wsa_data;

        return WSAStartup(WSAVersion, &wsa_data) == 0;
    }

    bool WindowsNetwork::Shutdown()
    {
        return WSACleanup() == 0;
    }

    std::optional<IPv4Address> WindowsNetwork::StringToIPv4Address(const std::string& address)
    {
        static_assert(sizeof(IN_ADDR) == sizeof(IPv4Address));

        auto in_address = IN_ADDR{};

        if (inet_pton(AF_INET, address.c_str(), &in_address) == 1)
        {
            auto ipv4_address = IPv4Address{};

            memcpy_s(&ipv4_address, sizeof(ipv4_address), &in_address, sizeof(in_address));

            return ipv4_address;
        }

        return {};
    }

    std::optional<IPv6Address> WindowsNetwork::StringToIPv6Address(const std::string& address)
    {
        static_assert(sizeof(IN6_ADDR) == sizeof(IPv6Address));

        auto in_address = IN6_ADDR{};

        if (inet_pton(AF_INET6, address.c_str(), &in_address) == 1)
        {
            auto ipv6_address = IPv6Address{};

            memcpy_s(&ipv6_address, sizeof(ipv6_address), &in_address, sizeof(in_address));

            return ipv6_address;
        }

        return {};
    }

    std::string WindowsNetwork::IPv4AddressToString(const IPv4Address& address)
    {
        static_assert(sizeof(IN_ADDR) == sizeof(IPv4Address));

        char ipv4_address[16];

        inet_ntop(AF_INET, &address, ipv4_address, sizeof(ipv4_address));

        return ipv4_address;
    }

    std::string WindowsNetwork::IPv6AddressToString(const IPv6Address& address)
    {
        static_assert(sizeof(IN6_ADDR) == sizeof(IPv6Address));

        char ipv6_address[46];

        inet_ntop(AF_INET6, &address, ipv6_address, sizeof(ipv6_address));

        return ipv6_address;
    }

}

#endif
